using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace network
{
    public class Responser
    {
        public class Response
        {
            public delegate void ResponseHandler(byte[] data);

            public Response(ushort session, ResponseHandler handler)
            {
                Session = session;
                Handler = handler;
            }

            public ushort Session
            {
                get;
                set;
            }

            public ResponseHandler Handler
            {
                get;
                set;
            }
        }

        private List<Response> responses_ = new List<Response>();

        private static Responser instance_;

        public static Responser Instance
        {
            get
            {
                if (instance_ == null)
                {
                    instance_ = new Responser();
                }
                return instance_;
            }
        }

        public void PushResponse(ushort session, Response.ResponseHandler handler)
        {
            responses_.Add(new Response(session, handler));
        }

        public void MessageHandler(Message message)
        {
            for (int i = 0; i < responses_.Count; ++i)
            {
                Response response = responses_[i];
                if (response.Session == message.Session)
                {

                    // 把protobuf定义的消息返回
                    byte[] data = new byte[message.Size];
                    message.Read(ref data, data.Length);
                    response.Handler(data);

                    responses_.RemoveAt(i);
                    break;
                }
            }

        }


    }
}