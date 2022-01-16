#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Black;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	// All this stuff could be done with a list but i'm so lazy

	Cube chassis1(info.chassis1_size.x, info.chassis1_size.y, info.chassis1_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis1.transform);
	btQuaternion q1 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset1(info.chassis1_offset.x, info.chassis1_offset.y, info.chassis1_offset.z);
	offset1 = offset1.rotate(q1.getAxis(), q1.getAngle());
	chassis1.color = Black;
	chassis1.transform.M[12] += offset1.getX();
	chassis1.transform.M[13] += offset1.getY();
	chassis1.transform.M[14] += offset1.getZ();

	Cube chassis2(info.chassis2_size.x, info.chassis2_size.y, info.chassis2_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis2.transform);
	btQuaternion q2 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset2(info.chassis2_offset.x, info.chassis2_offset.y, info.chassis2_offset.z);
	offset2 = offset2.rotate(q2.getAxis(), q2.getAngle());
	chassis2.color = Black;
	chassis2.transform.M[12] += offset2.getX();
	chassis2.transform.M[13] += offset2.getY();
	chassis2.transform.M[14] += offset2.getZ();

	Cube chassis3(info.chassis3_size.x, info.chassis3_size.y, info.chassis3_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis3.transform);
	btQuaternion q3 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset3(info.chassis3_offset.x, info.chassis3_offset.y, info.chassis3_offset.z);
	offset3 = offset3.rotate(q3.getAxis(), q3.getAngle());
	chassis3.color = Black;
	chassis3.transform.M[12] += offset3.getX();
	chassis3.transform.M[13] += offset3.getY();
	chassis3.transform.M[14] += offset3.getZ();

	Cube chassis4(info.chassis4_size.x, info.chassis4_size.y, info.chassis4_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis4.transform);
	btQuaternion q4 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset4(info.chassis4_offset.x, info.chassis4_offset.y, info.chassis4_offset.z);
	offset4 = offset4.rotate(q4.getAxis(), q4.getAngle());
	chassis4.color = Red;
	chassis4.transform.M[12] += offset4.getX();
	chassis4.transform.M[13] += offset4.getY();
	chassis4.transform.M[14] += offset4.getZ();

	Cube chassis5(info.chassis5_size.x, info.chassis5_size.y, info.chassis5_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis5.transform);
	btQuaternion q5 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset5(info.chassis5_offset.x, info.chassis5_offset.y, info.chassis5_offset.z);
	offset5 = offset5.rotate(q5.getAxis(), q5.getAngle());
	chassis5.color = Red;
	chassis5.transform.M[12] += offset5.getX();
	chassis5.transform.M[13] += offset5.getY();
	chassis5.transform.M[14] += offset5.getZ();

	Cube chassis6(info.chassis6_size.x, info.chassis6_size.y, info.chassis6_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis6.transform);
	btQuaternion q6 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset6(info.chassis6_offset.x, info.chassis6_offset.y, info.chassis6_offset.z);
	offset6 = offset6.rotate(q6.getAxis(), q6.getAngle());
	chassis6.color = White;
	chassis6.transform.M[12] += offset6.getX();
	chassis6.transform.M[13] += offset6.getY();
	chassis6.transform.M[14] += offset6.getZ();

	Cube chassis7(info.chassis7_size.x, info.chassis7_size.y, info.chassis7_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis7.transform);
	btQuaternion q7 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset7(info.chassis7_offset.x, info.chassis7_offset.y, info.chassis7_offset.z);
	offset7 = offset7.rotate(q7.getAxis(), q7.getAngle());
	chassis7.color = White;
	chassis7.transform.M[12] += offset7.getX();
	chassis7.transform.M[13] += offset7.getY();
	chassis7.transform.M[14] += offset7.getZ();

	Cube chassis8(info.chassis8_size.x, info.chassis8_size.y, info.chassis8_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis8.transform);
	btQuaternion q8 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset8(info.chassis8_offset.x, info.chassis8_offset.y, info.chassis8_offset.z);
	offset8 = offset8.rotate(q8.getAxis(), q8.getAngle());
	chassis8.color = White;
	chassis8.transform.M[12] += offset8.getX();
	chassis8.transform.M[13] += offset8.getY();
	chassis8.transform.M[14] += offset8.getZ();

	Cube chassis9(info.chassis9_size.x, info.chassis9_size.y, info.chassis9_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis9.transform);
	btQuaternion q9 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset9(info.chassis9_offset.x, info.chassis9_offset.y, info.chassis9_offset.z);
	offset9 = offset9.rotate(q9.getAxis(), q9.getAngle());
	chassis9.color = Red;
	chassis9.transform.M[12] += offset9.getX();
	chassis9.transform.M[13] += offset9.getY();
	chassis9.transform.M[14] += offset9.getZ();

	Cube chassis10(info.chassis10_size.x, info.chassis10_size.y, info.chassis10_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis10.transform);
	btQuaternion q10 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset10(info.chassis10_offset.x, info.chassis10_offset.y, info.chassis10_offset.z);
	offset10 = offset10.rotate(q10.getAxis(), q10.getAngle());
	chassis10.color = Red;
	chassis10.transform.M[12] += offset10.getX();
	chassis10.transform.M[13] += offset10.getY();
	chassis10.transform.M[14] += offset10.getZ();

	Cube chassis11(info.chassis11_size.x, info.chassis11_size.y, info.chassis11_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis11.transform);
	btQuaternion q11 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset11(info.chassis11_offset.x, info.chassis11_offset.y, info.chassis11_offset.z);
	offset11 = offset11.rotate(q11.getAxis(), q11.getAngle());
	chassis11.color = Red;
	chassis11.transform.M[12] += offset11.getX();
	chassis11.transform.M[13] += offset11.getY();
	chassis11.transform.M[14] += offset11.getZ();

	Cube chassis12(info.chassis12_size.x, info.chassis12_size.y, info.chassis12_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis12.transform);
	btQuaternion q12 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset12(info.chassis12_offset.x, info.chassis12_offset.y, info.chassis12_offset.z);
	offset12 = offset12.rotate(q12.getAxis(), q12.getAngle());
	chassis12.color = Red;
	chassis12.transform.M[12] += offset12.getX();
	chassis12.transform.M[13] += offset12.getY();
	chassis12.transform.M[14] += offset12.getZ();

	Cube chassis13(info.chassis13_size.x, info.chassis13_size.y, info.chassis13_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis13.transform);
	btQuaternion q13 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset13(info.chassis13_offset.x, info.chassis13_offset.y, info.chassis13_offset.z);
	offset13 = offset13.rotate(q13.getAxis(), q13.getAngle());
	chassis13.color = Grey;
	chassis13.transform.M[12] += offset13.getX();
	chassis13.transform.M[13] += offset13.getY();
	chassis13.transform.M[14] += offset13.getZ();

	Cube chassis14(info.chassis14_size.x, info.chassis14_size.y, info.chassis14_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis14.transform);
	btQuaternion q14 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset14(info.chassis14_offset.x, info.chassis14_offset.y, info.chassis14_offset.z);
	offset14 = offset14.rotate(q14.getAxis(), q14.getAngle());
	chassis14.color = Grey;
	chassis14.transform.M[12] += offset14.getX();
	chassis14.transform.M[13] += offset14.getY();
	chassis14.transform.M[14] += offset14.getZ();

	Cube chassis15(info.chassis15_size.x, info.chassis15_size.y, info.chassis15_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis15.transform);
	btQuaternion q15 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset15(info.chassis15_offset.x, info.chassis15_offset.y, info.chassis15_offset.z);
	offset15 = offset15.rotate(q15.getAxis(), q15.getAngle());
	chassis15.color = Cyan;
	chassis15.transform.M[12] += offset15.getX();
	chassis15.transform.M[13] += offset15.getY();
	chassis15.transform.M[14] += offset15.getZ();

	Cube chassis16(info.chassis16_size.x, info.chassis16_size.y, info.chassis16_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis16.transform);
	btQuaternion q16 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset16(info.chassis16_offset.x, info.chassis16_offset.y, info.chassis16_offset.z);
	offset16 = offset16.rotate(q16.getAxis(), q16.getAngle());
	chassis16.color = White;
	chassis16.transform.M[12] += offset16.getX();
	chassis16.transform.M[13] += offset16.getY();
	chassis16.transform.M[14] += offset16.getZ();

	Cube chassis17(info.chassis17_size.x, info.chassis17_size.y, info.chassis17_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis17.transform);
	btQuaternion q17 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset17(info.chassis17_offset.x, info.chassis17_offset.y, info.chassis17_offset.z);
	offset17 = offset17.rotate(q17.getAxis(), q17.getAngle());
	chassis17.color = Red;
	chassis17.transform.M[12] += offset17.getX();
	chassis17.transform.M[13] += offset17.getY();
	chassis17.transform.M[14] += offset17.getZ();

	Cube chassis18(info.chassis18_size.x, info.chassis18_size.y, info.chassis18_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis18.transform);
	btQuaternion q18 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset18(info.chassis18_offset.x, info.chassis18_offset.y, info.chassis18_offset.z);
	offset18 = offset18.rotate(q18.getAxis(), q18.getAngle());
	chassis18.color = Grey;
	chassis18.transform.M[12] += offset18.getX();
	chassis18.transform.M[13] += offset18.getY();
	chassis18.transform.M[14] += offset18.getZ();

	Cube chassis19(info.chassis19_size.x, info.chassis19_size.y, info.chassis19_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis19.transform);
	btQuaternion q19 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset19(info.chassis19_offset.x, info.chassis19_offset.y, info.chassis19_offset.z);
	offset19 = offset19.rotate(q19.getAxis(), q19.getAngle());
	chassis19.color = Grey;
	chassis19.transform.M[12] += offset19.getX();
	chassis19.transform.M[13] += offset19.getY();
	chassis19.transform.M[14] += offset19.getZ();

	// Not rendering chassis1 bc is the hitbox
	//chassis1.Render();
	chassis2.Render();
	chassis3.Render();
	chassis4.Render();
	chassis5.Render();
	chassis6.Render();
	chassis7.Render();
	chassis8.Render();
	chassis9.Render();
	chassis10.Render();
	chassis11.Render();
	chassis12.Render();
	chassis13.Render();
	chassis14.Render();
	chassis15.Render();
	chassis16.Render();
	chassis17.Render();
	chassis18.Render();
	chassis19.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}

vec3 PhysVehicle3D::GetForwardVector() const
{
	btVector3 h = vehicle->getForwardVector();
	vec3 ret;
	ret.Set(h.getX(), h.getY(), h.getZ());
	return ret;
}

void PhysVehicle3D::Orient(float angle)
{
	float matrix[16];
	memset(matrix, 0.0f, sizeof(matrix));

	// Keep position
	vec3 p = { GetPos().getX(), GetPos().getY(), GetPos().getZ() };
	matrix[12] = p.x;
	matrix[13] = p.y;
	matrix[14] = p.z;
	matrix[15] = 1;

	// Rotate the body by angle
	matrix[0] = cos(angle);
	matrix[2] = -sin(angle);
	matrix[5] = 1;
	matrix[8] = sin(angle);
	matrix[10] = cos(angle);

	SetTransform(matrix);
}