//
// Created by Kevin Yu on 4/14/16.
//
#pragma once

#include <falton/setting.h>
#include <falton/dynamic/ftConstraintSolver.h>
#include <falton/shape/ftShapeBuffer.h>
#include <falton/dynamic/ftIslandSystem.h>

struct ftBody;
struct ftCollider;
struct ftShape;
struct ftHingeJoint;
struct ftDistanceJoint;

typedef void (*ftBodyIterFunc)(ftBody *body, void *data);

/* Role : System */
class ftPhysicsSystem
{

  public:
    struct ftConfig
    {

        real sleepTimeLimit = 0.05f;
        real sleepLinearLimit = 0.08f;
        real sleepAngularLimit = (2.0f / 180.0f * PI);
        real sleepRatio = 0.2f;
        ftVector2 gravity = {0, -10};

        ftCollisionSystem::ftConfig  collisionConfig;
        ftConstraintSolver::ftConfig solverConfig;
    };

    ftVector2 m_gravity = {0, -10};

    void setConfiguration(const ftConfig &config);

    void init();
    void shutdown();

    ftBody *createStaticBody(const ftVector2 &position, real orientation);
    ftBody *createKinematicBody(const ftVector2 &position, real orientation);
    ftBody *createDynamicBody(const ftVector2 &position, real orientation, real mass, real moment);
    void destroyBody(ftBody *body);
    void updateBody(ftBody *body); //call this every time user change body member variable.

    ftCollider *createCollider(ftBody *body, ftShape *shape, const ftVector2 &position, real orientation);
    void destroyCollider(ftCollider *collider);

    ftHingeJoint *createHingeJoint(ftBody *bodyA, ftBody *bodyB, ftVector2 anchorPoint);
    ftDistanceJoint *createDistanceJoint(ftBody *bodyA, ftBody *bodyB, ftVector2 localAnchorA, ftVector2 localAnchorB);
    ftSpringJoint *createSpringJoint(ftBody *bodyA, ftBody *bodyB, ftVector2 localAnchorA, ftVector2 localAnchorB);
    ftDynamoJoint *createDynamoJoint(ftBody *bodyA, ftBody *bodyB, real targetRate, real maxTorque);
    ftPistonJoint* createPistonJoint(ftBody* bodyA, ftBody* bodyB, ftVector2 localAnchorA, ftVector2 localAnchorB, ftVector2 axis);
    void destroyJoint(ftJoint *joint);
    
    void iterateBody(ftBodyIterFunc iterFunc, void *data);
    void iterateStaticBody(ftBodyIterFunc iterFunc, void *data);
    void iterateKinematicBody(ftBodyIterFunc iterFunc, void *data);
    void iterateDynamicBody(ftBodyIterFunc iterFunc, void *data);

	void regionQuery(ftAABB region, ftVectorArray<ftCollider*>* collidersInRegion);

    template <typename T>
    void forEachBody(T func);
    template <typename T>
    void forEachStaticBody(T func);
    template <typename T>
    void forEachKinematicBody(T func);
    template <typename T>
    void forEachDynamicBody(T func);

    template <typename T>
    void forEachContact(T func);

	template <typename T>
	void forEachJoint(T func);

    void step(real dt);

  private:
    ftBodyBuffer m_staticBodies;
    ftBodyBuffer m_kinematicBodies;
    ftBodyBuffer m_dynamicBodies;
    ftBodyBuffer m_sleepingBodies;

    ftShapeBuffer m_shapeBuffer;

    ftConstraintSolver m_contactSolver;
    ftCollisionSystem m_collisionSystem;
    ftIslandSystem m_islandSystem;

    ftChunkArray<ftJoint *> m_joints;

    //configuration
    real m_sleepTimeLimit = 0.05f;
    real m_sleepLinearLimit = 0.08f;
    real m_sleepAngualrLimit = (2.0f / 180.0f * PI);

    ftShape *createShape(ftShape *shape);
    void destroyShape(ftShape *shape);

    void integrateVelocity(real dt);
    void integratePosition(real dt);

    void updateBodiesActivation(real dt);

    static void beginContactListener(ftContact *contact, void *data);
    static void updateContactListener(ftContact *contact, void *data);
    static void endContactListener(ftContact *contact, void *data);
};

template <typename T>
inline void ftPhysicsSystem::forEachBody(T func)
{
    m_staticBodies.forEach(func);
    m_kinematicBodies.forEach(func);
    m_dynamicBodies.forEach(func);
    m_sleepingBodies.forEach(func);
}

template <typename T>
inline void ftPhysicsSystem::forEachStaticBody(T func)
{
    m_staticBodies.forEach(func);
}

template <typename T>
inline void ftPhysicsSystem::forEachKinematicBody(T func)
{
    m_kinematicBodies.forEach(func);
}

template <typename T>
inline void ftPhysicsSystem::forEachDynamicBody(T func)
{
    m_dynamicBodies.forEach(func);
    m_sleepingBodies.forEach(func);
}

template <typename T>
inline void ftPhysicsSystem::forEachContact(T func)
{
    const auto iterate = [func](ftColHandle handleA, ftColHandle handleB, ftContact *contact) {
        func(contact);
    };
    m_collisionSystem.forEachContact(std::cref(iterate));
}

template <typename T>
inline void ftPhysicsSystem::forEachJoint(T func) 
{
	for (int i = 0; i < m_joints.getSize(); ++i) {
		func(m_joints[i]);
	}
}
