#include "PhysBody3D.h"
#include "glmath.h"
#include <cmath>
#include "Bullet/include/btBulletDynamicsCommon.h"

// =================================================
PhysBody3D::PhysBody3D(btRigidBody* body) : body(body)
{}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	delete body;
}

// ---------------------------------------------------------
void PhysBody3D::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
}

void PhysBody3D::SetAsSensor(bool is_sensor)
{
	if (this->is_sensor != is_sensor)
	{
		this->is_sensor = is_sensor;
		if (is_sensor == true)
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		else
			body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}

void PhysBody3D::SetId(int id)
{
	this->id = id;
}

void PhysBody3D::SetRotation(btQuaternion q)
{
	btTransform t = body->getWorldTransform();
	t.setRotation(q);
	body->setWorldTransform(t);
}

btQuaternion PhysBody3D::GetRotation() {
	btQuaternion q;
	btTransform t = body->getWorldTransform();
	q = t.getRotation();
	return q;
}

Euler PhysBody3D::GetEulerAngles(btQuaternion q)
{
	Euler eule;

	float sinr_cosp = 2 * (q.getW() * q.getX() + q.getY() * q.getZ());
	float cosr_cosp = 1 - 2 * (q.getX() * q.getX() + q.getY() * q.getY());
	eule.roll = std::atan2(sinr_cosp, cosr_cosp);

	float sinp = 2 * (q.getW() * q.getY() - q.getX() * q.getZ());
	if (std::abs(sinp) >= 1)
		eule.pitch = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
	else
		eule.pitch = std::asin(sinp);

	float siny_cosp = 2 * (q.getW() * q.getZ() + q.getY() * q.getX());
	float cosy_cosp = 1 - 2 * (q.getY() * q.getY() + q.getZ() * q.getZ());
	eule.yaw = std::atan2(siny_cosp, cosy_cosp);

	return eule;
}

btVector3 PhysBody3D::GetPos() {

	btVector3 a = body->getWorldTransform().getOrigin();

	return a;
}

void PhysBody3D::SetLinearVelocity(float x, float y, float z)
{
	btVector3 v(x, y, z);
	body->setLinearVelocity(v);
}

void PhysBody3D::SetAngularVelocity(float x, float y, float z)
{
	btVector3 v(x, y, z);
	body->setAngularVelocity(v);
}