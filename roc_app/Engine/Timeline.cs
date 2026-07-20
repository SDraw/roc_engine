using System.Collections.Generic;

namespace ROC.Engine
{
    internal class Timeline<T>
    {
        readonly List<(uint, T)> m_keyframes = null;
        int m_keyframesCount = 0;

        public Timeline()
        {
            m_keyframes = new List<(uint, T)>();
        }

        public void Add(uint p_time, T p_value)
        {
            m_keyframes.Add((p_time, p_value));
            m_keyframesCount++;
        }

        public bool Get(uint p_time, out T p_valueA, out T p_valueB, out float p_blend)
        {
            p_valueA = default(T);
            p_valueB = default(T);
            p_blend = 0f;

            if(m_keyframesCount == 0)
                return false;

            if(m_keyframesCount == 1)
            {
                p_valueA = m_keyframes[0].Item2;
                p_valueB = m_keyframes[0].Item2;
                p_blend = 0f;
                return true;
            }

            // Just simple loop for searching frame, no search optimizations
            bool l_result = false;
            for(int i1 = 0, j = m_keyframesCount; i1 < j; i1++)
            {
                if(m_keyframes[i1].Item1 >= p_time)
                {
                    if(i1 > 0)
                    {
                        int i0 = i1 - 1;
                        p_blend = Utils.EaseInOut(Utils.InverseLerp(m_keyframes[i0].Item1, m_keyframes[i1].Item1, p_time));
                        p_valueA = m_keyframes[i0].Item2;
                        p_valueB = m_keyframes[i1].Item2;
                    }
                    else
                    {
                        p_blend = 0f;
                        p_valueA = m_keyframes[i1].Item2;
                        p_valueB = m_keyframes[i1].Item2;
                    }

                    l_result = true;
                    break;
                }
            }

            return l_result;
        }
    }
}
