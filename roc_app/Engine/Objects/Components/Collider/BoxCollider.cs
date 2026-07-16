using BulletSharp;
using GlmSharp;
using ROC.Engine.Managers;

namespace ROC.Engine.Objects.Components
{
    public sealed class BoxCollider : Collider
    {
        BoxShape m_shape = null;
        vec3 m_size = vec3.Ones;

        public override int ComponentPriority => 3;

        internal BoxCollider()
        {
            m_componentType = ComponentType.BoxCollider;

            m_shape = new BoxShape(1.0f);
            m_shape.CalculateLocalInertia(1f, out var l_inertia);
            m_rigidBody = new RigidBody(new RigidBodyConstructionInfo(
                1f,
                new DefaultMotionState(BulletSharp.Math.Matrix.Identity),
                m_shape,
                l_inertia
            ));
            m_rigidBody.UserObject = this;
            TypeOfMotion = MotionType.Default;

            PhysicsManager.AddRigidBody(m_rigidBody);
        }

        internal override void Destroy()
        {
            if(m_shape != null)
            {
                m_shape.Dispose();
                m_shape = null;
            }

            base.Destroy();
        }

        public vec3 Size
        {
            get => m_size;
            set
            {
                if(m_rigidBody == null)
                    return;

                m_size = vec3.Max(vec3.Zero, value);

                PhysicsManager.RemoveRigidBody(m_rigidBody);

                m_shape.LocalScaling = new BulletSharp.Math.Vector3(m_size.Values);

                float l_mass = Mass;
                if(l_mass > 0f)
                {
                    m_shape.CalculateLocalInertia(l_mass, out var l_inertia);
                    m_rigidBody.SetMassProps(l_mass, l_inertia);
                }

                m_rigidBody.UpdateInertiaTensor();

                PhysicsManager.AddRigidBody(m_rigidBody);
            }
        }
    }
}
