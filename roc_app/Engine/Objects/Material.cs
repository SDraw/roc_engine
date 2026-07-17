using GlmSharp;
using ROC.Engine.Objects.Resources;

namespace ROC.Engine.Objects
{
    public sealed class Material : Object
    {
        public bool Transparency
        {
            get;
            set;
        }

        public bool DepthWrite
        {
            get;
            set;
        }

        public bool DoubleSided
        {
            get;
            set;
        }

        public bool Unlit
        {
            get;
            set;
        }

        public vec4 Color
        {
            get;
            set;
        }

        public Texture DiffuseTexture
        {
            get;
            set;
        }

        public vec4 Params
        {
            get;
            set;
        }

        internal Material()
        {
            Transparency = false;
            DepthWrite = true;
            DoubleSided = false;
            Unlit = true;
            Color = vec4.Ones;
            DiffuseTexture = null;
            Params = vec4.Zero;
        }

        public Material(Material p_source)
        {
            Transparency = p_source.Transparency;
            DepthWrite = p_source.DepthWrite;
            DoubleSided = p_source.DoubleSided;
            Unlit = p_source.Unlit;
            Color = p_source.Color;
            DiffuseTexture = p_source.DiffuseTexture;
            Params = p_source.Params;
        }

        // API
        public static Material Create() => new Material();
    }
}
