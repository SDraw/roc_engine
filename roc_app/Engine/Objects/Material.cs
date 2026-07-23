using GlmSharp;
using ROC.Engine.Objects.Resources;

namespace ROC.Engine.Objects
{
    public sealed class Material : Object
    {
        public enum RenderMode
        {
            Opaque = 0,
            Cutout,
            Transparent
        }

        public RenderMode Mode
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
            Mode = RenderMode.Opaque;
            DoubleSided = false;
            Unlit = true;
            Color = vec4.Ones;
            Params = vec4.Zero;
            DiffuseTexture = null;
        }

        public Material(Material p_source)
        {
            Mode = p_source.Mode;
            DoubleSided = p_source.DoubleSided;
            Unlit = p_source.Unlit;
            Color = p_source.Color;
            Params = p_source.Params;
            DiffuseTexture = p_source.DiffuseTexture;
        }

        // API
        public static Material Create() => new Material();
    }
}
