using UnityEngine;
using System.Collections;
using System.Net;
using System.Net.Sockets;
using System;
using System.Collections.Generic;
using System.IO;

namespace network
{
    public class NetClient
    {
        public delegate void NetMessageHandler(Message message);
        public delegate void NetResponseHandler(Message message);

        private static NetClient instance_;

        private Socket socket_;
        private string ip_ = "127.0.0.1";
        private int port_ = 5010;
        private byte[] data_ = new byte[Message.kHeaderSize + Message.kBodySize];

        // 用来存储临时的数据,处理粘包
        private List<byte> cache_ = new List<byte>();

        private Message message_ = new Message();

        private Queue<Message> message_queue_ = new Queue<Message>();

        public NetMessageHandler MessageHandler
        {
            get;
            set;
        }
        public NetResponseHandler ResponseHandler
        {
            get;
            set;
        }

        /// <summary>
        /// 单例对象
        /// </summary>
        public static NetClient Instance
        {
            get
            {
                if (instance_ == null)
                {
                    instance_ = new NetClient();
                }
                return instance_;
            }
        }

        private NetClient()
        {
        }

        public bool Initalized()
        {
            //创建客户端连接对象
            socket_ = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            //连接到服务器
            socket_.Connect(IPAddress.Parse(ip_), port_);
            AsyncReceive();
            return true;
        }

        #region 网络数据收发
        private void AsyncReceive()
        {
            //开启异步消息接收 消息到达后会直接写入 缓冲区data_
            socket_.BeginReceive(data_, 0, Message.kBodySize + Message.kHeaderSize, SocketFlags.None, ReceiveData, data_);
        }

        private void ReceiveData(IAsyncResult ar)
        {
            try
            {
                //获取当前收到的消息长度()
                int length = socket_.EndReceive(ar);
                byte[] message = new byte[length];
                Buffer.BlockCopy(data_, 0, message, 0, length);
                cache_.AddRange(message);

                ReceiveHeader();

                //接续接受消息
                AsyncReceive();
            }
            catch (Exception e)
            {
                Debug.Log("远程服务器主动断开连接 : " + e.ToString());
                
                socket_.Close();
            }
        }

        private void ReceiveHeader()
        {
            if (Message.DecodeHeader(message_, cache_))
            {
                cache_.RemoveRange(0, Message.kHeaderSize);

                ReceiveBody(message_.Size);
            }
            else
            {
                return;
            }

            //递归继续读取消息
            ReceiveHeader();
        }

        private void ReceiveBody(int body_size)
        {
            // 消息体足够
            if (cache_.Count >= body_size)
            {
                message_.Size = 0;
                try
                {
                    message_.Write(cache_.GetRange(0, body_size).ToArray());
                }
                catch(Exception e)
                {
                    Debug.Log("远程服务器主动断开连接 : " + e.ToString());
                }

                cache_.RemoveRange(0, body_size);

                message_queue_.Enqueue(message_);
                message_ = new Message();
            }
        }

        public void Send(ushort session, ushort moudle_id, ushort area_id, uint proto_id, byte[] data)
        {
            Message message = new Message();
            message.Session = session;
            message.MoudleId = moudle_id;
            message.AreaId = area_id;
            message.ProtoId = proto_id;
            message.Write(data);

            Send(message);
        }

        public void Send(Message message)
        {
            byte[] data = Message.Encode(message);

            Debug.Log("session : " + message.Session + "MoudleId : " + message.MoudleId + "AreaID : " + message.AreaId + "ProtoID : " + message.ProtoId);
            socket_.Send(data);
        }
        #endregion

        #region 网络消息更新，每帧接收
        // 主游戏循环
        public void Update()
        {
            while (message_queue_.Count > 0)
            {
                Message message = message_queue_.Dequeue();
                if (message != null)
                {
                    // 服务器推送的消息
                    if(message.Session  == 0)
                    {
                        if(MessageHandler != null)
                        {
                            MessageHandler(message);
                        }
                    }
                    // 请求的返回
                    else
                    {
                        if (ResponseHandler != null)
                        {
                            ResponseHandler(message);
                        }
                    }                    
                }
            }
        }
        #endregion
    }
}