#include "stdafx.h"
#include <cmath> 
#include "FunctionHook.h"
#include "InterpolationFixes.h"

// Euler/Quat conversions: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
// Quat lerping: https://stackoverflow.com/a/46187052

FunctionHook<void, const NJS_MKEY_A*, Uint32, Float, Angle3*> LinearMotionA_t(0x42DF60);

namespace interpolation
{
	bool enabled = false;
}

void NinjaAngleToQuaternion(NJS_QUATERNION* q, Rotation* ang)
{
	auto yaw = (float)NJM_ANG_RAD(ang->z);
	auto pitch = (float)NJM_ANG_RAD(ang->y);
	auto roll = (float)NJM_ANG_RAD(ang->x);

	auto cy = cosf(yaw * 0.5f);
	auto sy = sinf(yaw * 0.5f);
	auto cp = cosf(pitch * 0.5f);
	auto sp = sinf(pitch * 0.5f);
	auto cr = cosf(roll * 0.5f);
	auto sr = sinf(roll * 0.5f);

	q->im[0] = sr * cp * cy - cr * sp * sy;
	q->im[1] = cr * sp * cy + sr * cp * sy;
	q->im[2] = cr * cp * sy - sr * sp * cy;
	q->re = cr * cp * cy + sr * sp * sy;
}

void QuaternionToNinjaAngle(Rotation* angle, NJS_QUATERNION* q)
{
	Float sinr_cosp = 2.0f * (q->re * q->im[0] + q->im[1] * q->im[2]);
	Float cosr_cosp = 1.0f - 2.0f * (q->im[0] * q->im[0] + q->im[1] * q->im[1]);
	angle->x = NJM_RAD_ANG(std::atan2(sinr_cosp, cosr_cosp));

	Float sinp = 2.0f * (q->re * q->im[1] - q->im[2] * q->im[0]);
	if (std::abs(sinp) >= 1.0f)
		angle->y = NJM_RAD_ANG(std::copysign(NJD_PI / 2, sinp)); // use 90 degrees if out of range
	else
		angle->y = NJM_RAD_ANG(std::asin(sinp));

	Float siny_cosp = 2.0f * (q->re * q->im[2] + q->im[0] * q->im[1]);
	Float cosy_cosp = 1.0f - 2.0f * (q->im[1] * q->im[1] + q->im[2] * q->im[2]);
	angle->z = NJM_RAD_ANG(std::atan2(siny_cosp, cosy_cosp));
}

Float scalorQuaternion(NJS_QUATERNION* q1, NJS_QUATERNION* q2)
{
	return q1->im[0] * q2->im[0] + q1->im[1] * q2->im[1] + q1->im[2] * q2->im[2] + q1->re * q2->re;
}

void negateQuaternion(NJS_QUATERNION* q)
{
	q->im[0] = -q->im[0];
	q->im[1] = -q->im[1];
	q->im[2] = -q->im[2];
	q->re = -q->re;
}

void unitQuaternion(NJS_QUATERNION* q)
{
	float l = 1.0f / std::sqrt(scalorQuaternion(q, q));
	q->im[0] = l * q->im[0];
	q->im[1] = l * q->im[1];
	q->im[2] = l * q->im[2];
	q->re = l * q->re;
}

void lerpQuaternion(NJS_QUATERNION* q, NJS_QUATERNION* a, NJS_QUATERNION* b, Float t)
{
	if (scalorQuaternion(a, b) < 0.0f)
	{
		negateQuaternion(b);
	}

	q->im[0] = a->im[0] - t * (a->im[0] - b->im[0]);
	q->im[1] = a->im[1] - t * (a->im[1] - b->im[1]);
	q->im[2] = a->im[2] - t * (a->im[2] - b->im[2]);
	q->re = a->re - t * (a->re - b->re);
}

void nlerpQuaternion(NJS_QUATERNION* q, NJS_QUATERNION* a, NJS_QUATERNION* b, Float t)
{
	lerpQuaternion(q, a, b, t);
	unitQuaternion(q);
}

const intptr_t sub_42C2C0Ptr = 0x42C2C0;
void _nuGetMotionLinearKeys(void* a2, int a1, int a3, float a4, void** a5, void** a6, float* a7) {
	__asm {
		mov esi, a2
		mov edi, a1
		push a7
		push a6
		push a5
		push a4
		push a3
		call sub_42C2C0Ptr
		add esp, 5 * 4
	}
}

void __cdecl LinearMotionA_r(const NJS_MKEY_A* key, Uint32 nbkeys, Float frame, Angle3* dst)
{
	if (!interpolation::enabled)
	{
		return LinearMotionA_t.Original(key, nbkeys, frame, dst);
	}

	Float rate1;
	NJS_MKEY_A* key1;
	NJS_MKEY_A* key2;

	_nuGetMotionLinearKeys((void*)key, sizeof(NJS_MKEY_A), nbkeys, frame,
		(void**)&key1, (void**)&key2, &rate1);

	NJS_MKEY_A* key_o = key1;
	NJS_MKEY_A* key_n = key2;

	Rotation ang_orig = { key_o->key[0], key_o->key[1], key_o->key[2] };
	Rotation ang_next = { key_n->key[0], key_n->key[1], key_n->key[2] };

	NJS_QUATERNION q1, q2;
	NinjaAngleToQuaternion(&q1, &ang_orig);
	NinjaAngleToQuaternion(&q2, &ang_next);

	NJS_QUATERNION r;
	nlerpQuaternion(&r, &q1, &q2, rate1);

	QuaternionToNinjaAngle(dst, &r);
}

void interpolation::push()
{
	interpolation::enabled = true;
}

void interpolation::pop()
{
	interpolation::enabled = false;
}

void interpolation::init()
{
	LinearMotionA_t.Hook(LinearMotionA_r);
}