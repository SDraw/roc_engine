using BulletSharp;
using GlmSharp;
using ROC.Engine.Core;
using ROC.Engine.Managers;
using ROC.Engine.Objects.Resources;

namespace ROC.Engine.Objects.Components
{
    public class Collider : Component
    {
        public enum MotionType
        {
            Default = 0,
            Static,
            Kinematic
        }

        protected RigidBody m_rigidBody = null;
        MotionType m_motionType = MotionType.Default;
        bool m_internalUpdate = false;

        ulong m_lastFrame = 0UL;

        protected Collider()
        {
        }

        protected override void DestroyInternal()
        {
            if(m_rigidBody != null)
            {
                PhysicsManager.RemoveRigidBody(m_rigidBody);
                m_rigidBody.MotionState.Dispose();
                m_rigidBody.Dispose();
                m_rigidBody = null;
            }

            base.DestroyInternal();
        }

        // Component events
        internal override void OnTransform()
        {
            if(m_rigidBody == null || m_internalUpdate)
                return;

            vec3 l_pos = GameObject.Position;
            quat l_rot = GameObject.Rotation;

            m_rigidBody.WorldTransform =
                BulletSharp.Math.Matrix.RotationQuaternion(
                    new BulletSharp.Math.Quaternion(l_rot.x, l_rot.y, l_rot.z, l_rot.w)
                ) * BulletSharp.Math.Matrix.Translation(l_pos.x, l_pos.y, l_pos.z);
            m_rigidBody.Activate(true);
        }

        internal override void OnShadowPass(Shader p_shader, Frustum p_frustum)
        {
            if(m_lastFrame == DeltaTick.FrameIndex || m_rigidBody == null || m_motionType != MotionType.Default)
                return;

            m_lastFrame = DeltaTick.FrameIndex;
            Update();
        }

        internal override void OnRenderPass(Shader p_shader, Camera p_camera)
        {
            // Will set rigid body transform to GameObject
            if(m_lastFrame == DeltaTick.FrameIndex || m_rigidBody == null || m_motionType != MotionType.Default)
                return;

            m_lastFrame = DeltaTick.FrameIndex;
            Update();
        }

        void Update()
        {
            m_internalUpdate = true;

            m_rigidBody.WorldTransform.Decompose(out _, out var l_rot, out var l_pos);
            GameObject.Position = new vec3(l_pos.ToArray());
            GameObject.Rotation = new quat(l_rot.X, l_rot.Y, l_rot.Z, l_rot.W).NormalizedSafe;

            m_internalUpdate = false;
        }

        internal override void OnDisable()
        {
            if(!IsValid)
                return;

            m_rigidBody.ForceActivationState(ActivationState.DisableSimulation);
            m_rigidBody.CollisionFlags = CollisionFlags.NoContactResponse;

        }

        internal override void OnEnable()
        {
            if(!IsValid)
                return;

            UpdateMotionType();
            m_rigidBody.Activate(true);
        }

        // Properties
        public float Mass
        {
            get => ((m_rigidBody == null) || m_rigidBody.InvMass.IsEpsilonEqual(0f)) ? 0f : (1f / m_rigidBody.InvMass);
            set
            {
                if(m_rigidBody == null)
                    return;

                PhysicsManager.RemoveRigidBody(m_rigidBody);

                var l_inertia = BulletSharp.Math.Vector3.Zero;
                if(value > 0f)
                    m_rigidBody.CollisionShape.CalculateLocalInertia(value, out l_inertia);

                m_rigidBody.SetMassProps(value, l_inertia);
                m_rigidBody.UpdateInertiaTensor();
                m_rigidBody.Activate(true);

                PhysicsManager.AddRigidBody(m_rigidBody);
            }
        }

        public MotionType TypeOfMotion
        {
            get => m_motionType;
            set
            {
                if(m_rigidBody == null)
                    return;

                m_motionType = value;
                UpdateMotionType();
            }
        }

        public vec3 Velocity
        {
            get => (m_rigidBody != null) ? new vec3(m_rigidBody.LinearVelocity.ToArray()) : vec3.Zero;
            set
            {
                if(m_rigidBody == null)
                    return;

                m_rigidBody.LinearVelocity = new BulletSharp.Math.Vector3(value.Values);
                m_rigidBody.Activate(true);
            }
        }

        public vec3 AngularVelocity
        {
            get => (m_rigidBody != null) ? new vec3(m_rigidBody.AngularVelocity.ToArray()) : vec3.Zero;
            set
            {
                if(m_rigidBody == null)
                    return;

                m_rigidBody.AngularVelocity = new BulletSharp.Math.Vector3(value.Values);
                m_rigidBody.Activate(true);
            }
        }

        public vec3 LinearFactor
        {
            get => (m_rigidBody != null) ? new vec3(m_rigidBody.LinearFactor.ToArray()) : vec3.Zero;
            set
            {
                if(m_rigidBody == null)
                    return;

                m_rigidBody.LinearFactor = new BulletSharp.Math.Vector3(value.Values);
                m_rigidBody.Activate(true);
            }
        }

        public vec3 AngularFactor
        {
            get => (m_rigidBody != null) ? new vec3(m_rigidBody.AngularFactor.ToArray()) : vec3.Zero;
            set
            {
                if(m_rigidBody == null)
                    return;

                m_rigidBody.AngularFactor = new BulletSharp.Math.Vector3(value.Values);
                m_rigidBody.Activate(true);
            }
        }

        public float Friction
        {
            get => (m_rigidBody != null) ? m_rigidBody.Friction : 0f;
            set
            {
                if(m_rigidBody == null)
                    return;

                m_rigidBody.Friction = value;
                m_rigidBody.Activate(true);
            }
        }

        public float Restitution
        {
            get => (m_rigidBody != null) ? m_rigidBody.Restitution : 0f;
            set
            {
                if(m_rigidBody == null)
                    return;

                m_rigidBody.Restitution = value;
                m_rigidBody.Activate(true);
            }
        }

        public bool IsActive => ((m_rigidBody != null) && m_rigidBody.IsActive);

        // Collider methods
        public void ApplyForce(vec3 p_force, vec3 p_point)
        {
            if(m_rigidBody == null)
                return;

            m_rigidBody.ApplyForce(new BulletSharp.Math.Vector3(p_force.Values), new BulletSharp.Math.Vector3(p_point.Values));
            m_rigidBody.Activate(true);
        }

        public void ApplyCentralForce(vec3 p_force)
        {
            if(m_rigidBody == null)
                return;

            m_rigidBody.ApplyCentralForce(new BulletSharp.Math.Vector3(p_force.Values));
            m_rigidBody.Activate(true);
        }

        public void ApplyImpulse(vec3 p_impulse, vec3 p_point)
        {
            if(m_rigidBody == null)
                return;

            m_rigidBody.ApplyImpulse(new BulletSharp.Math.Vector3(p_impulse.Values), new BulletSharp.Math.Vector3(p_point.Values));
            m_rigidBody.Activate(true);
        }

        public void ApplyCentralImpulse(vec3 p_impulse)
        {
            if(m_rigidBody == null)
                return;

            m_rigidBody.ApplyCentralImpulse(new BulletSharp.Math.Vector3(p_impulse.Values));
            m_rigidBody.Activate(true);
        }

        public void ApplyTorque(vec3 p_torque)
        {
            if(m_rigidBody == null)
                return;

            m_rigidBody.ApplyTorque(new BulletSharp.Math.Vector3(p_torque.Values));
            m_rigidBody.Activate(true);
        }

        public void ApplyTorqueImpulse(vec3 p_torque)
        {
            if(m_rigidBody == null)
                return;

            m_rigidBody.ApplyTorqueImpulse(new BulletSharp.Math.Vector3(p_torque.Values));
            m_rigidBody.Activate(true);
        }

        public void Activate(bool p_forced) => m_rigidBody?.Activate(p_forced);

        // Internal
        void UpdateMotionType()
        {
            switch(m_motionType)
            {
                case MotionType.Default:
                {
                    m_rigidBody.CollisionFlags = CollisionFlags.None;
                    m_rigidBody.ForceActivationState(ActivationState.ActiveTag);
                }
                break;

                case MotionType.Static:
                {
                    m_rigidBody.CollisionFlags = CollisionFlags.StaticObject;
                    m_rigidBody.ForceActivationState(ActivationState.ActiveTag);
                }
                break;

                case MotionType.Kinematic:
                {
                    m_rigidBody.CollisionFlags = CollisionFlags.KinematicObject;
                    m_rigidBody.ForceActivationState(ActivationState.DisableDeactivation);
                }
                break;
            }
        }
    }
}
