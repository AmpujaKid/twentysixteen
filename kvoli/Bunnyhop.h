namespace CmdStuff {
	void Bunnyhop(CUserCmd*, CBaseEntity*);
	void CircleStrafer(CUserCmd*, CBaseEntity*);
	bool JustStarted = true;
	bool CircleDone = true;
}

void CmdStuff::CircleStrafer(CUserCmd* cmd, CBaseEntity* localPly) {
	static float angle = 0.f;
	static int inv = -1;

	CircleDone = false;

	if (JustStarted) {
		JustStarted = false;
		angle = cmd->viewangles.y;
	}
	
	Ray_t left_trace;
	Ray_t right_trace;
	Ray_t normal_trace;
	CTraceFilterSimple Filter = CTraceFilterSimple(localPly);
	Vector EyePos = localPly->m_vecViewOffset() + localPly->AbsOrigin();

	Vector vleft = Vector(10.f, angle - 50.f, 0.f), vright = Vector(10.f, angle + 50.f, 0.f), vnormal = Vector(10.f, angle, 0.f), left, right, normal;
	AngleVectors(vright, &right, 0, 0);
	AngleVectors(vleft, &left, 0, 0);
	AngleVectors(vnormal, &normal, 0, 0);

	left_trace.Init(EyePos, EyePos + left * 50.f);
	right_trace.Init(EyePos, EyePos + right * 50.f);
	normal_trace.Init(EyePos, EyePos + normal * 50.f);

	trace_t result_left, result_right, result;

	Interfaces->Trace->TraceRay(left_trace, MASK_SOLID, &Filter, &result_left);
	Interfaces->Trace->TraceRay(right_trace, MASK_SOLID, &Filter, &result_right);
	Interfaces->Trace->TraceRay(normal_trace, MASK_SOLID, &Filter, &result);

	if ((result.fraction != 1.f && !result.m_pEnt) || (result_right.fraction != 1.f && !result_right.m_pEnt)) {
		inv = -1;
		angle -= 30.f;
		cmd->sidemove = -250.f;
	}
	else if (result_left.fraction != 1.f && !result_left.m_pEnt) {
		inv = 1;
		angle += 30.f;
		cmd->sidemove = 250.f;
	}
	else {
		float flAdd = (200.f / localPly->m_vecVelocity().Length()) * 16.f;
		if (flAdd > 5.f) flAdd = 5.f;
		angle += flAdd * (float)inv;
		cmd->sidemove = 250.f * (float)inv;
	}

	AngleNormalize(m_vecStrafedAngles.y);

	Vector noob(0.f, angle - cmd->viewangles.y, 0.f);
	AngleNormalize(noob);

	if (noob.y > 170.f && noob.y < 180.f)
		CircleDone = true;

	m_vecStrafedAngles.y = angle;
}

void CmdStuff::Bunnyhop(CUserCmd* cmd, CBaseEntity* localPly) {
	if (!Vars.Misc.Bunnyhop) return;
	static bool OldBool;
	bool CircleStrafe = GetAsyncKeyState('C') & 0x8000 || !CircleDone;

	if (OldBool != CircleStrafe)
		JustStarted = true;

	if (!(cmd->buttons & IN_JUMP)) {
		CircleStrafe = false;
		CircleDone = true;
		JustStarted = true;
		return;
	}

	OldBool = CircleStrafe;

	if (!(localPly->m_fFlags() & FL_ONGROUND) && cmd->buttons & IN_JUMP) {
		cmd->buttons &= ~IN_JUMP;

		if (CircleStrafe) {
			CircleStrafer(cmd, localPly);
			return;
		}

		if (cmd->mousedx > 1 || cmd->mousedx < -1) {
			cmd->sidemove = cmd->mousedx < 0.f ? -localPly->m_flMaxspeed() : localPly->m_flMaxspeed();
		}
		else {
			cmd->forwardmove = (850.f * 3.f) / localPly->m_vecVelocity().Length();
			cmd->sidemove = (cmd->commandnumber % 2) == 0 ? -localPly->m_flMaxspeed() : localPly->m_flMaxspeed();
			if (cmd->forwardmove > localPly->m_flMaxspeed())
				cmd->forwardmove = localPly->m_flMaxspeed();
		}
	}
	else {
		if (cmd->buttons & IN_JUMP && !CircleStrafer) {
			cmd->forwardmove = localPly->m_flMaxspeed();
		}
	}
}