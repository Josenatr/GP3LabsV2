#include "pch.h"
#include "Physics.h"
#include "RigidBody.h"
#include "Application.h"

Physics* Physics::m_instance = nullptr;
Physics::Physics()
{
	Init();
}
void Physics::Init()
{
	m_world->setGravity(btVector3(0, gravity, 0));
}
void Physics::AddRigidbody(RigidBody* r)
{
	m_rbodies.push_back(r);
	m_world->addRigidBody(r->Get());
}

void Physics::AddBoxPush(BoxPush* b)
{
	m_bp.push_back(b);
}
void Physics::PreUpdate()
{
	for (auto r : m_rbodies)
	{
		r->UpdateRigidBody();
	}
}

void Physics::AddForceUp(float deltaTime) {
	PreUpdate();
	m_world->stepSimulation(deltaTime, 2);

	
		for (auto r : m_rbodies)
		{
			r->ForceUp();
		}
	
}
void Physics::AddForceDown(float deltaTime) {
	PreUpdate();
	m_world->stepSimulation(deltaTime, 2);

	for (auto r : m_rbodies)
		{
			r->ForceDown();
		}
	
}
void Physics::AddForceLeft(float deltaTime) {
	PreUpdate();
	m_world->stepSimulation(deltaTime, 2);

	
		for (auto r : m_rbodies)
		{
			r->ForceLeft();
		}
	
}
void Physics::AddForceRight(float deltaTime) {
	PreUpdate();
	m_world->stepSimulation(deltaTime, 2);

	
		for (auto r : m_rbodies)
		{
			r->ForceRight();
		}
	
}

void Physics::AddTorque(float deltaTime) {
	PreUpdate();
	m_world->stepSimulation(deltaTime, 2);

	for (auto bp : m_bp){
		for (auto r : m_rbodies)
		{
			r->Torque();
		}
	}
	
}
void Physics::AddJump(float deltaTime) {
	PreUpdate();
	m_world->stepSimulation(deltaTime, 2);

	
		for (auto r : m_rbodies)
		{
			r->Jump();
		}
	
}

void Physics::Update(float deltaTime)
{
	PreUpdate();
	m_world->stepSimulation(deltaTime, 2);
	for (auto r : m_rbodies)
	{
		r->UpdateParent();
	}
}
void Physics::Quit()
{
	delete m_world;
	delete m_solver;
	delete m_broadphase;
	delete m_dispatcher;
	delete m_collisionConfig;
}
btTransform Physics::ConvertTransformToBtTransform(Transform t)
{
	glm::quat rot = t.GetRotation();
	glm::vec3 pos = t.GetPosition();
	return btTransform(btQuaternion(rot.x, rot.y, rot.z, rot.w),
		btVector3(pos.x, pos.y, pos.z));
}
Physics* Physics::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new Physics();
	}
	return m_instance;
}
