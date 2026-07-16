using System;
using System.IO;

namespace ROC.Engine.Managers
{
    public sealed class LogManager : Manager
    {
        StreamWriter m_log = null;

        internal LogManager()
        {
        }

        internal override void Start()
        {
            if(m_active)
                return;

            try
            {
                m_log = new StreamWriter("log.txt", false);
                m_active = true;
            }
            catch(Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        internal override void Stop()
        {
            if(!m_active)
                return;

            m_log.Flush();
            m_log.Close();

            m_active = false;
        }

        public void Log(string p_msg)
        {
            if(!m_active)
                return;

            string l_msg = string.Format("[{0}] {1}", DateTime.Now.ToString("HH:mm:ss"), p_msg);
            m_log.WriteLine(l_msg);
            Console.WriteLine(l_msg);
        }
    }
}
