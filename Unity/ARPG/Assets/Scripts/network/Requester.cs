using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace network
{
    public class Requester
    {
        private static Requester instance_;

        private ushort session_ = 0;

        public static Requester Instance
        {
            get
            {
                if (instance_ == null)
                {
                    instance_ = new Requester();
                }

                return instance_;
            }
        }

        public void Request(ushort moudle_id, ushort area_id, uint proto_id, byte[] data, Responser.Response.ResponseHandler handler)
        {
            ushort session = GenSession();
            //responsers_.Add(new Responser(session, handler));
            Responser.Instance.PushResponse(session, handler);
            NetClient.Instance.Send(session, moudle_id, area_id, proto_id, data);
        }

        private ushort GenSession()
        {
            return ++session_;
        }

        void OnNetMessageHandler(Message message)
        {
            //
        }
    }
}