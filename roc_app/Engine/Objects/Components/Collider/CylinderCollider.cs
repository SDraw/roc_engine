using BulletSharp;
using GlmSharp;
using ROC.Engine.Managers;

namespace ROC.Engine.Objects.Components
{
    public sealed class CylinderCollider : Collider
    {
        CylinderShape m_shape = null;
        vec3 m_size = vec3.Ones;

        public override int ComponentPriority => 3;

        internal CylinderCollider()
        {
            m_componentType = ComponentType.CylinderCollider;

            m_shape = new CylinderShape(new BulletSharp.Math.Vector3(0.5f, 0.5f, 0.5f));
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

        protected override void DestroyInternal()
        {
            m_shape?.Dispose();
            m_shape = null;

            base.DestroyInternal();
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
