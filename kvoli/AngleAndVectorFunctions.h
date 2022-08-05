void NormalizeVector(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0)
	{
		vOut = Vector(0.f, 0.f, 1.f);
		return;
	}

	flLen = 1 / flLen;
	vOut = Vector(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}

float VectorNormalize(Vector& vec)
{
	float sqrlen = vec.LengthSqr() + 1.0e-10f, invlen;
	invlen = 1.f / sqrt(sqrlen);
	vec.x *= invlen;
	vec.y *= invlen;
	vec.z *= invlen;
	return sqrlen * invlen;
}

float VectorDotProductFast(const Vector& a, const Vector& b)
{
	float dotret;
	__asm
	{
		mov ecx, a
		mov eax, b
			fld dword ptr[eax + 0]
			fmul dword ptr[ecx + 0]
			fld dword ptr[eax + 4]
			fmul dword ptr[ecx + 4]
			fld dword ptr[eax + 8]
			fmul dword ptr[ecx + 8]
			fxch st(1)
			faddp st(2), st(0)
			faddp st(1), st(0)
			fstp dword ptr[dotret]
	}
	return dotret;
}

void AngleNormalize(Vector& angles) {
	for (int i = 0; i < 3; i++) {
		while (angles[i] > 180.f)
			angles[i] -= 360.f;
		while (angles[i] < -180.f)
			angles[i] += 360.f;
	}

	if (angles[1] == -180.f) angles[1] = 180.f;
	angles.z = 0.f;
}

void ClampAngle(Vector& angles) {
	if (angles.x > 89.f) angles.x = 89.f;
	else if (angles.x < -89.f) angles.x = -89.f;
}

void AngleNormalize(float& angle) {
	while (angle > 180.f)
		angle -= 360.f;
	while (angle < -180.f)
		angle += 360.f;
}

float GetNormalized(float angle) {
	float temp = angle;
	while (angle > 180.f)
		angle -= 360.f;
	while (angle < -180.f)
		angle += 360.f;

	return temp;
}

float __fastcall fastSqrt(float ehi)
{
	float tmp = ehi;
	float half = 0.5f * ehi;
	int i = *(int*)(&ehi);

	i = 0x5f3759df - (i >> 1);
	ehi = *(float*)(&i);

	ehi = ehi * (1.5f - half * ehi * ehi);
	ehi = ehi * (1.5f - half * ehi * ehi);

	return tmp * ehi;
}

void CalcAngle(Vector &src, Vector &dst, Vector &angles) {
	Vector delta = Vector((src[0] - dst[0]), (src[1] - dst[1]), (src[2] - dst[2]));
	double hyp = sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
	angles[0] = atan(delta[2] / hyp)        *(180.0 / (float)M_PI);
	angles[1] = atan(delta[1] / delta[0])    *(180.0 / (float)M_PI);
	angles[2] = 0.0f;
	if (delta[0] >= 0.0) { angles[1] += 180.0f; }
}

void AngleVectors(const Vector &angles, Vector* forward, Vector* right, Vector* up)
{
	double angle, angle2, angle3;
	double sr, sp, sy, cr, cp, cy, cpi = (M_PI / 180);

	angle = angles.y * cpi;
	sy = sin(angle);
	cy = cos(angle);

	angle2 = angles.x * cpi;
	sp = sin(angle2);
	cp = cos(angle2);

	angle3 = angles.z * cpi;
	sr = sin(angle3);
	cr = cos(angle3);

	if (forward)
	{
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr*-sy);
		up->y = (cr*sp*sy + -sr*cy);
		up->z = cr*cp;
	}
}

void VectorAngles(const Vector& forward, Vector &angles) {
	float	tmp, yaw, pitch;
	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180.f / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrtf(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180.f / M_PI);
		if (pitch < 0)
			pitch += 360;
	}
	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

void VectorAngles2(const Vector& forward, Vector &angles)
{
	float tmp, yaw, pitch;
	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		pitch = 0;
	}
	else
	{
		yaw = atan2(forward[1], forward[0]) * 180.f / M_PI;
		if (yaw < 0) yaw += 360.f;
		tmp = fastSqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = atan2(-forward[2], tmp) * 180.f / M_PI;
	}
	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

void FixMovement(Vector& oang, CUserCmd* pCmd) {
	Vector vMove = Vector(pCmd->forwardmove, pCmd->sidemove, 0.0f);
	float flSpeed = vMove.Length();
	Vector qMove;
	VectorAngles(vMove, qMove);
	float normalized = fmod(pCmd->viewangles.y + 180.f, 360.f) - 180.f;
	float normalizedx = fmod(pCmd->viewangles.x + 180.f, 360.f) - 180.f;
	float flYaw = DEG2RAD((normalized - oang.y) + qMove.y);
	if (normalizedx >= 90.0f || normalizedx <= -90.0f) pCmd->forwardmove = -cos(flYaw) * flSpeed;
	else pCmd->forwardmove = cos(flYaw) * flSpeed;
	pCmd->sidemove = sin(flYaw) * flSpeed;
}

FORCEINLINE float DotProduct(const float *v1, const float *v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

FORCEINLINE void VectorTransform(const float *in1, matrix3x4 & in2, float *out)
{
	out[0] = DotProduct(in1, in2[0]) + in2[0][3];
	out[1] = DotProduct(in1, in2[1]) + in2[1][3];
	out[2] = DotProduct(in1, in2[2]) + in2[2][3];
}

FORCEINLINE void VectorTransform(const Vector& in1, matrix3x4 &in2, Vector &out)
{
	VectorTransform(&in1.x, in2, &out.x);
}