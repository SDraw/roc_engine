using BulletSharp;
using ROC.Engine.Managers;

namespace ROC.Engine.Objects.Components
{
    public sealed class CapsuleCollider : Collider
    {
        CapsuleShape m_shape = null;
        float m_radius = 1f;
        float m_height = 1f;

        public override int ComponentPriority => 3;

        internal CapsuleCollider()
        {
            m_componentType = ComponentType.CapsuleCollider;

            m_shape = new CapsuleShape(m_radius, m_height);
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

        public float Radius
        {
            get => m_radius;
            set
            {
                if(m_rigidBody == null)
                    return;

                m_radius = System.Math.Max(0f, value);

                PhysicsManager.RemoveRigidBody(m_rigidBody);

                m_shape.LocalScaling = new BulletSharp.Math.Vector3(m_radius, m_height, m_radius);

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

        public float Height
        {
            get => m_height;
            set
            {
                if(m_rigidBody == null)
                    return;

                m_height = System.Math.Max(0f, value);

                PhysicsManager.RemoveRigidBody(m_rigidBody);

                m_shape.LocalScaling = new BulletSharp.Math.Vector3(m_radius, m_height, m_radius);

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
