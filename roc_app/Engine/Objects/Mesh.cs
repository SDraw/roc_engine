using OpenGL;
using ROC.Engine.OGL;

namespace ROC.Engine.Objects
{
    public sealed class Mesh : Object
    {
        bool m_generated = false;

        GLArrayBuffer m_vertexBuffer = null;
        GLArrayBuffer m_normalBuffer = null;
        GLArrayBuffer m_uvBuffer = null;
        GLArrayBuffer m_weightBuffer = null;
        GLArrayBuffer m_indexBuffer = null;
        GLVertexArray m_vertexArray = null;

        int m_trianglesCount = 0;

        public bool IsGenerated => m_generated;

        internal Mesh()
        {
        }

        public void Generate(float[] p_vertex, float[] p_normal, float[] p_uv, float[] p_weights, int[] p_index)
        {
            if(m_generated)
                return;

            m_vertexBuffer = new GLArrayBuffer();
            m_vertexBuffer.Create(p_vertex.Length * sizeof(float), p_vertex, BufferUsage.StaticDraw);

            m_normalBuffer = new GLArrayBuffer();
            m_normalBuffer.Create(p_normal.Length * sizeof(float), p_normal, BufferUsage.StaticDraw);

            m_uvBuffer = new GLArrayBuffer();
            m_uvBuffer.Create(p_uv.Length * sizeof(float), p_uv, BufferUsage.StaticDraw);

            if(p_weights != null)
            {
                m_weightBuffer = new GLArrayBuffer();
                m_weightBuffer.Create(p_weights.Length * sizeof(float), p_weights, BufferUsage.StaticDraw);
            }

            if(p_index != null)
            {
                m_indexBuffer = new GLArrayBuffer();
                m_indexBuffer.Create(p_index.Length * sizeof(int), p_index, BufferUsage.StaticDraw);
            }

            m_vertexArray = new GLVertexArray();
            m_vertexArray.Create();
            m_vertexArray.Bind();

            m_vertexBuffer.Bind();
            m_vertexArray.EnableAttribute(0, 3, Gl.FLOAT);

            m_normalBuffer.Bind();
            m_vertexArray.EnableAttribute(1, 3, Gl.FLOAT);

            m_uvBuffer.Bind();
            m_vertexArray.EnableAttribute(2, 2, Gl.FLOAT);

            if(m_weightBuffer != null)
            {
                m_weightBuffer.Bind();
                m_vertexArray.EnableAttribute(3, 4, Gl.FLOAT);
            }

            if(m_indexBuffer != null)
            {
                m_indexBuffer.Bind();
                m_vertexArray.EnableAttribute(4, 4, Gl.INT);
            }

            m_trianglesCount = p_vertex.Length / 3;
            m_generated = true;
        }

        public void Destroy()
        {
            if(!m_generated)
                return;

            m_vertexBuffer.Destroy();
            m_vertexBuffer = null;

            m_normalBuffer.Destroy();
            m_normalBuffer = null;

            m_uvBuffer.Destroy();
            m_uvBuffer = null;

            m_weightBuffer?.Destroy();
            m_weightBuffer = null;

            m_indexBuffer?.Destroy();
            m_indexBuffer = null;

            m_trianglesCount = 0;
            m_generated = false;
        }

        internal void Draw()
        {
            if(!m_generated)
                return;

            m_vertexArray.Bind();
            m_vertexArray.Draw(PrimitiveType.Triangles, m_trianglesCount);
        }
    }
}
