using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

namespace network
{
    public class Message
    {
        public const int kHeaderSize = 16;
        public const int kBodySize = 1024 * 64;

        static public bool DecodeHeader(Message message, List<byte> data)
        {
            // 不满足消息头大小，直接返回
            if (data.Count < Message.kHeaderSize)
            {
                return false;
            }

            MemoryStream stream = new MemoryStream(data.ToArray());
            BinaryReader reader = new BinaryReader(stream);

            // 解析消息头
            message.Session = reader.ReadUInt32();
            message.MoudleId = reader.ReadUInt16();
            message.AreaId = reader.ReadUInt16();
            message.ProtoId = reader.ReadUInt32();
            message.Size = reader.ReadInt32();

            stream.Close();
            reader.Close();

            return true;
        }

        static public byte[] Encode(Message message)
        {
            byte[] data = new byte[message.Size + Message.kHeaderSize];
            MemoryStream stream = new MemoryStream(data);
            BinaryWriter writer = new BinaryWriter(stream);
            writer.Write(message.Session);
            writer.Write(message.MoudleId);
            writer.Write(message.AreaId);
            writer.Write(message.ProtoId);
            writer.Write(message.Size);

            byte[] body_data = new byte[message.Size];
            message.Read(ref body_data, message.Size);
            writer.Write(body_data);

            return data;
        }


        public Message()
        {
            Session = 0;
            MoudleId = 0;
            AreaId = 0;
            ProtoId = 0;
            Size = 0;
            CurSize = 0;
            data_ = new List<byte>();
        }

        public void Write(byte[] data)
        {
            if (data.Length + Size > kBodySize)
            {
                return;
            }

            data_.AddRange(data);
            Size += data.Length;
        }

        public void Read(ref byte[] data, int size)
        {
            if (CurSize + size > Size)
            {
                return;
            }

            data = data_.GetRange(CurSize, size).ToArray();
            CurSize += size;
        }

        public uint Session { get; set; }
        public ushort MoudleId { get; set; }
        public ushort AreaId { get; set; }
        public uint ProtoId { get; set; }
        public int Size { get; set; }
        public int CurSize { get; set; }

        List<byte> data_;
    }
}