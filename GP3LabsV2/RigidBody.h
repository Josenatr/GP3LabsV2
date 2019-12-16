#include "Component.h"
#include "Physics.h"
#include "CollisionShape.h"
#include "pch.h"
class RigidBody : public Component
{
private:
	CollisionShape* m_shape;
	btRigidBody* m_rigidBody;
	btMotionState* m_mState = nullptr;
	float mass;
public:
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	void Init(CollisionShape* shape, float mass = 1.f, const glm::vec3 localInertia = glm::vec3(1));
	virtual void UpdateParent();
	virtual void UpdateRigidBody();
	void ForceUp();
	void ForceDown();
	void ForceLeft();
	void ForceRight();
	void Torque();
	void Jump();
	btRigidBody* Get() { return m_rigidBody; }
};

class BoxPush : public Component
{
public:
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	void ForceUp(float deltaTime);
	void Init();
};