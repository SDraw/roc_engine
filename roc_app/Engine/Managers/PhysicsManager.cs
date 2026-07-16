using BulletSharp;
using BulletSharp.Math;
using GlmSharp;

namespace ROC.Engine.Managers
{
    public sealed class PhysicsManager : Manager
    {
        static Vector3 ms_defaultGravity = new Vector3(0f, -9.8f, 0f);

        DbvtBroadphase m_broadPhase = null;
        DefaultCollisionConfiguration m_collisionConfiguration = null;
        CollisionDispatcher m_collisionDispatcher = null;
        SequentialImpulseConstraintSolver m_constraintSolver = null;

        static DiscreteDynamicsWorld ms_dynamicWorld = null;

        float m_timeStep = 0f;

        RigidBody m_floor = null;
        vec3 m_gravity;

        internal PhysicsManager()
        {
            PhysicsEnabled = true;
            m_gravity = new vec3(ms_defaultGravity.ToArray());
        }

        internal override void Start()
        {
            if(m_active)
                return;

            m_broadPhase = new DbvtBroadphase();
            m_collisionConfiguration = new DefaultCollisionConfiguration();
            m_collisionDispatcher = new CollisionDispatcher(m_collisionConfiguration);
            m_constraintSolver = new SequentialImpulseConstraintSolver();
            ms_dynamicWorld = new DiscreteDynamicsWorld(m_collisionDispatcher, m_broadPhase, m_constraintSolver, m_collisionConfiguration);

            ms_dynamicWorld.Gravity = ms_defaultGravity;

            uint l_fpsLimit = Core.Core.Instance.ConfigManager.FpsLimit;
            m_timeStep = (l_fpsLimit == 0U) ? (1f / 120f) : (1.5f / l_fpsLimit);

            m_active = true;
            FloorState = true;
        }

        internal override void Stop()
        {
            if(!m_active)
                return;

            ms_dynamicWorld.Dispose();
            ms_dynamicWorld = null;

            m_constraintSolver.Dispose();
            m_constraintSolver = null;

            m_collisionDispatcher.Dispose();
            m_collisionDispatcher = null;

            m_collisionConfiguration.Dispose();
            m_collisionConfiguration = null;

            m_broadPhase.Dispose();
            m_broadPhase = null;

            m_active = false;
        }

        internal void DoPulse()
        {
            if(!m_active || !PhysicsEnabled)
                return;

            ms_dynamicWorld.StepSimulation(m_timeStep, 10, 1f / 120f);
        }

        // Open methods
        public bool PhysicsEnabled
        {
            get;
            set;
        }

        public bool FloorState
        {
            get => (m_floor != null);
            set
            {
                if(ms_dynamicWorld == null)
                    return;

                if(value && (m_floor == null))
                {
                    m_floor = new RigidBody(
                        new RigidBodyConstructionInfo(
                            0f,
                            new DefaultMotionState(),
                            new StaticPlaneShape(
                                new Vector3(0f, 1f, 0f),
                                0f
                            )
                        )
                    );

                    ms_dynamicWorld.AddRigidBody(m_floor);
                    return;
                }

                if(!value && (m_floor != null))
                {
                    ms_dynamicWorld.RemoveRigidBody(m_floor);

                    m_floor.MotionState.Dispose();
                    m_floor.Dispose();
                    m_floor = null;
                }
            }
        }

        public vec3 Gravity
        {
            get => m_gravity;
            set
            {
                if(ms_dynamicWorld == null)
                    return;

                m_gravity = value;
                ms_dynamicWorld.Gravity = new Vector3(m_gravity.Values);
            }
        }

        public bool RayCast(vec3 p_start, vec3 p_end, out vec3 p_hitPoint, out vec3 p_normal, out object p_object)
        {
            p_hitPoint = vec3.Zero;
            p_normal = vec3.Zero;
            p_object = null;

            if(ms_dynamicWorld == null)
                return false;

            var l_from = new Vector3(p_start.Values);
            var l_to = new Vector3(p_end.Values);

            ClosestRayResultCallback l_callback = new ClosestRayResultCallback(ref l_from, ref l_to);
            ms_dynamicWorld.RayTest(l_from, l_to, l_callback);

            if(!l_callback.HasHit)
                return false;

            p_hitPoint = new vec3(l_callback.HitPointWorld.ToArray());
            p_normal = new vec3(l_callback.HitNormalWorld.ToArray());
            p_object = l_callback.CollisionObject?.UserObject;
            return true;
        }

        // Internal only
        internal static void AddRigidBody(RigidBody p_body) => ms_dynamicWorld?.AddRigidBody(p_body);
        internal static void RemoveRigidBody(RigidBody p_body) => ms_dynamicWorld?.RemoveRigidBody(p_body);
    }
}
