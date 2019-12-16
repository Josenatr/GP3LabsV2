#pragma once
#include <btBulletDynamicsCommon.h>
#include "Transform.h"

class RigidBody;
class BoxPush;

class Physics
{
private:
	static Physics* m_instance;
	btConstraintSolver* m_solver = new
		btSequentialImpulseConstraintSolver();
	btCollisionConfiguration* m_collisionConfig = new
		btDefaultCollisionConfiguration();
	btDispatcher* m_dispatcher = new
		btCollisionDispatcher(m_collisionConfig);
	btBroadphaseInterface* m_broadphase = new btDbvtBroadphase();
	btDynamicsWorld* m_world = new btDiscreteDynamicsWorld(m_dispatcher,
		m_broadphase, m_solver, m_collisionConfig);
	std::vector<RigidBody*> m_rbodies;
	std::vector<BoxPush*> m_bp;
	Physics();
	void Init();
public:
	static Physics* GetInstance();
	inline btDynamicsWorld* GetWorld() {return GetInstance()->m_world;};
	void AddRigidbody(RigidBody* r);
	void AddBoxPush(BoxPush* b);
	void AddForceUp(float deltaTime);
	void AddForceDown(float deltaTime);
	void AddForceLeft(float deltaTime);
	void AddForceRight(float deltaTime);
	void AddTorque(float deltaTime);
	void AddJump(float deltaTime);
	void ChangeGravity(float gravity);
	void PreUpdate();
	void Update(float deltaTime);
	void Quit();
	static btTransform ConvertTransformToBtTransform(Transform t);

	float gravity = -5.0f;
};
