namespace CmdStuff {
	void AntiAim(CUserCmd*, CBaseEntity*, bool);
	void AtTargets(CUserCmd*, CBaseEntity*);
	bool WallDetection(CUserCmd*, CBaseEntity*);
	bool IsAngel;
}

void CmdStuff::AtTargets(CUserCmd* cmd, CBaseEntity* localPly) {
	float lowest = 99999999.f;
	Vector EyePos = localPly->m_vecViewOffset() + localPly->AbsOrigin();

	for (int i = 1; i < 65; i++) {
		if (i == Interfaces->Engine->GetLocalPlayer()) continue;
		CBaseEntity* pEnt = Interfaces->EntList->GetClientEntity(i);
		if (!pEnt) continue;
		if (pEnt->GetNetworkable()->IsDormant()) continue;
		if (pEnt->m_lifeState() != 0) continue;
		if (pEnt->m_iHealth() < 1) continue;
		if (pEnt->m_iTeamNum() == localPly->m_iTeamNum()) continue;

		Vector CurPos = pEnt->m_vecViewOffset() + pEnt->AbsOrigin();

		if (CurPos.DistToSqr(EyePos) < lowest) {
			lowest = CurPos.DistTo(EyePos);
			CalcAngle(EyePos, CurPos, cmd->viewangles);
		}
	}
}

bool CmdStuff::WallDetection(CUserCmd* pCmd, CBaseEntity* localPly) {
	Vector eyepos = localPly->m_vecViewOffset() + localPly->AbsOrigin();
	Vector tmp = Vector(10.f, 0.f, 0.f);
	float lowestFraction = 1.f;

	for (float y = 0.f; y < 360.f; y += 2.f) {
		tmp.y = y;
		Ray_t ray;
		Vector forward;
		trace_t result;
		AngleVectors(tmp, &forward, 0, 0);
		ray.Init(eyepos, eyepos + (forward * 27.3f));

		CTraceFilterSimple Filter = CTraceFilterSimple(localPly);

		Interfaces->Trace->TraceRay(ray, 0x200400B, &Filter, &result);

		if (result.fraction < lowestFraction) {
			lowestFraction = result.fraction;
			pCmd->viewangles.y = tmp.y;

			Vector left, right;
			Vector vleft = Vector(10.f, y + 25.f, 0.f);
			Vector vright = Vector(10.f, y - 25.f, 0.f);

			AngleVectors(vleft, &left, 0, 0);
			AngleVectors(vright, &right, 0, 0);

			Ray_t trace_left, trace_right;

			trace_left.Init(eyepos, eyepos + (left * 46.f));
			trace_right.Init(eyepos, eyepos + (right * 46.f));

			trace_t result_left, result_right;

			Interfaces->Trace->TraceRay(trace_left, 0x200400B, &Filter, &result_left);
			Interfaces->Trace->TraceRay(trace_right, 0x200400B, &Filter, &result_right);

			if (result_left.fraction == 1.f && result_right.fraction != 1.f)
				pCmd->viewangles.y -= 45.f;
			else if (result_right.fraction == 1.f && result_left.fraction != 1.f)
				pCmd->viewangles.y += 45.f;
		}
	}

	return lowestFraction != 1.f;
}

void CmdStuff::AntiAim(CUserCmd* cmd, CBaseEntity* localPly, bool bSendPacket) {
	IsAngel = false;
	if (!Vars.AntiAim.Normal.Enabled) return;

	int Yaw = Vars.AntiAim.Normal.Yaw;
	int YawMode = Vars.AntiAim.Normal.YawMode;
	int Pitch = Vars.AntiAim.Normal.Pitch;
	int WallMode = Vars.AntiAim.Wall.YawMode;
	int WallPitch = Vars.AntiAim.Wall.Pitch;
	int DuckPitch = Vars.AntiAim.Duck.Pitch;
	int DuckYaw = Vars.AntiAim.Duck.Yaw;
	int DuckYawMode = Vars.AntiAim.Duck.YawMode;
	
	bool WallDtc = Vars.AntiAim.Wall.Enabled ? ((localPly->m_vecVelocity().Length() < 300.f) ? WallDetection(cmd, localPly) : false) : false;
	bool bCrouch = (Vars.AntiAim.Duck.Enabled && (cmd->buttons & IN_DUCK) && localPly->m_vecVelocity().Length() < 300.f) ? true : false;

	if (WallDtc) {
		if (WallPitch == 0) cmd->viewangles.x = 89.f; // Normal
		else if (WallPitch == 1) cmd->viewangles.x = -180.f; // 180 Down
		else if (WallPitch == 2) { cmd->viewangles.x = 36000089.f; IsAngel = true; } // Lisp

		if (WallMode == 1) cmd->viewangles.y += (!bSendPacket) ? 90.f : 0.f; // Fake
		else if (WallMode == 2) cmd->viewangles.y += (bSendPacket) ? 90.f : -90.f;
		else if (WallMode == 3 || WallMode == 4) { // Jitter
			static bool twitch, mode;
			mode = (!bSendPacket) ? !mode : mode;
			twitch = (bSendPacket) ? !twitch : twitch;
			cmd->viewangles.y += 180.f + (((bSendPacket) ? twitch : mode) ? ((WallMode == 3) ? 180.f : 179.990005f) : 0.f);
		}
		else if (WallMode == 5) { cmd->viewangles.y += 36000360.f; IsAngel = true; } // Lisp
	}
	else if (bCrouch) {
		if (Vars.AntiAim.Duck.TargetMode == 0) cmd->viewangles.y = 0.f; // Static
		else if (Vars.AntiAim.Duck.TargetMode == 2) AtTargets(cmd, localPly); // At Targets

		if (DuckPitch == 0) cmd->viewangles.x = (bSendPacket) ? 181.f : 179.f; // Lag Down
		else if (DuckPitch == 1) cmd->viewangles.x = (!bSendPacket) ? 181.f : 179.f; // Lag Up
		else if (DuckPitch == 2) cmd->viewangles.x = -179.990005f; // Fakedown
		else if (DuckPitch == 3) cmd->viewangles.x = 180.f; // Normal
		else if (DuckPitch == 4) cmd->viewangles.x = -180.f; // Normal Inverted
		else if (DuckPitch == 5) cmd->viewangles.x = 181.f; // Up

		if (DuckYaw == 0) cmd->viewangles.y += ((DuckYawMode == 0 || DuckYawMode == 2) ? 90.f : -90.f) + ((DuckYawMode == 0 || DuckYawMode == 1) ? (!bSendPacket ? 180.f : 0.f) : 0.f); // Sideways
		else if (DuckYaw == 1) cmd->viewangles.y += ((DuckYawMode == 0 || DuckYawMode == 2) ? 135.f : 135.f + 180.f) + ((DuckYawMode == 0 || DuckYawMode == 1) ? (!bSendPacket ? 180.f : 0.f) : 0.f); // Half Sideways
		else if (DuckYaw == 2) cmd->viewangles.y += 180.f + ((!bSendPacket && (DuckYawMode == 0 || DuckYawMode == 1)) ? ((DuckYawMode) ? 90.f : -90.f) : 0.f); // Backwards
		else if (DuckYaw == 3) { // Jitter
			if (DuckYawMode == 0 || DuckYawMode == 1) { // Normal, Synced
				static bool twitch, mode;
				mode = (!bSendPacket) ? !mode : mode;
				twitch = (bSendPacket) ? !twitch : twitch;
				cmd->viewangles.y += 180.f + (((bSendPacket) ? twitch : mode) ? ((DuckYawMode == 0) ? 180.f : 179.990005f) : 0.f);
			}
			else if (DuckYawMode == 2) { // M3nly
				static bool m3nlytwitch;
				static bool m3nlytrick, m3nlynigger;
				if (bSendPacket) {
					m3nlytwitch = !m3nlytwitch;
					if (m3nlytwitch) {
						m3nlytrick = !m3nlytrick;
						cmd->viewangles.y += (m3nlytrick ? -179.990005f : 180.f);
					}
					else
						cmd->viewangles.y += (m3nlytrick ? 0.990005f : 0.f);
				}
				else {
					if (m3nlynigger)
						m3nlynigger = false;
					else {
						m3nlynigger = true;
						m3nlytwitch = !m3nlytwitch;
						if (m3nlytwitch) {
							m3nlytrick = !m3nlytrick;
							cmd->viewangles.y += (m3nlytrick ? -179.990005f : 180.f);
						}
						else {
							cmd->viewangles.y += (m3nlytrick ? 0.990005f : 0.f);
						}
					}
				}
			}
			else { // Sp3cial, RSp3cial
				static bool mode;
				mode = (bSendPacket) ? !mode : mode;
				cmd->viewangles.y -= ((mode ? 60.f : -60.f) * ((bSendPacket) ? 1.f : -1.f)) - (DuckYawMode == 4 ? 180.f : 0.f);
			}
		}
	}
	else {
		if (Vars.AntiAim.Normal.TargetMode == 0) cmd->viewangles.y = 0.f; // Static
		else if (Vars.AntiAim.Normal.TargetMode == 2) AtTargets(cmd, localPly); // At Targets

		if (Pitch == 0) cmd->viewangles.x = (bSendPacket) ? 181.f : 179.f; // Lag Down
		else if (Pitch == 1) cmd->viewangles.x = (!bSendPacket) ? 181.f : 179.f; // Lag Up
		else if (Pitch == 2) cmd->viewangles.x = -179.990005f; // Fakedown
		else if (Pitch == 3) cmd->viewangles.x = 180.f; // Normal
		else if (Pitch == 4) cmd->viewangles.x = -180.f; // Normal Inverted
		else if (Pitch == 5) { cmd->viewangles.x = 36000088.f; IsAngel = true; }// Angel Down
		else if (Pitch == 6) { cmd->viewangles.x = 35999912.f; IsAngel = true; } // Angel Up

		if (Yaw == 0) cmd->viewangles.y += ((YawMode == 0 || YawMode == 2) ? 90.f : -90.f) + ((YawMode == 0 || YawMode == 1) ? (!bSendPacket ? 180.f : 0.f) : 0.f); // Sideways
		else if (Yaw == 1) cmd->viewangles.y += ((YawMode == 0 || YawMode == 2) ? 135.f : 135.f + 180.f) + ((YawMode == 0 || YawMode == 1) ? (!bSendPacket ? 180.f : 0.f) : 0.f); // Half Sideeways
		else if (Yaw == 2) cmd->viewangles.y += 180.f + ((!bSendPacket && (YawMode == 0 || YawMode == 1)) ? ((YawMode) ? 90.f : -90.f) : 0.f); // Backwards
		else if (Yaw == 3) { // Jitter
			if (YawMode == 0 || YawMode == 1) { // Normal, Synced
				static bool twitch, mode;
				mode = (!bSendPacket) ? !mode : mode;
				twitch = (bSendPacket) ? !twitch : twitch;
				cmd->viewangles.y += 180.f + (((bSendPacket) ? twitch : mode) ? ((YawMode == 0) ? 180.f : 179.990005f) : 0.f);
			}
			else if (YawMode == 2) { // M3nly
				static bool m3nlytwitch;
				static bool m3nlytrick, m3nlynigger;
				if (bSendPacket) {
					m3nlytwitch = !m3nlytwitch;
					if (m3nlytwitch) {
						m3nlytrick = !m3nlytrick;
						cmd->viewangles.y += (m3nlytrick ? -179.990005f : 180.f);
					}
					else
						cmd->viewangles.y += (m3nlytrick ? 0.990005f : 0.f);
				}
				else {
					if (m3nlynigger)
						m3nlynigger = false;
					else {
						m3nlynigger = true;
						m3nlytwitch = !m3nlytwitch;
						if (m3nlytwitch) {
							m3nlytrick = !m3nlytrick;
							cmd->viewangles.y += (m3nlytrick ? -179.990005f : 180.f);
						}
						else {
							cmd->viewangles.y += (m3nlytrick ? 0.990005f : 0.f);
						}
					}
				}
			}
			else { // Sp3cial, RSp3cial
				static bool mode;
				mode = (bSendPacket) ? !mode : mode;
				cmd->viewangles.y -= ((mode ? 60.f : -60.f) * ((bSendPacket) ? 1.f : -1.f)) - (YawMode == 4 ? 180.f : 0.f);
			}
		}
		else if (Yaw == 4) cmd->viewangles.y += fmodf(Interfaces->Globals->curtime, 3.6f) * ((YawMode == 2 || YawMode == 3) ? 500.f : 1500.f) - ((!bSendPacket && (YawMode == 1 || YawMode == 3)) ? 180.f : 0.f); // Spin
		else if (Yaw == 5) { // Angel
			IsAngel = true;
			if (YawMode == 0 || YawMode == 1) cmd->viewangles.y += 36000180.f + ((YawMode == 1) ? 180.f : 0.f); // Normal, Inverted
			else if (YawMode == 6) {
				static bool mode;
				mode = (bSendPacket) ? !mode : mode;
				cmd->viewangles.y += 36000180.f - (((mode ? 60.f : -60.f) * ((bSendPacket) ? 1.f : -1.f)));
			}
			else {
				cmd->viewangles.y += 36000180.f;
				if (YawMode == 2 || YawMode == 3) {
					if(YawMode == 2 || (YawMode == 3 && !bSendPacket))
						cmd->viewangles.y += fmodf(Interfaces->Globals->curtime, 3.6f) * 1500.f;
				}
				if (YawMode == 5 || YawMode == 4) {
					if (YawMode == 4 || (YawMode == 5 && !bSendPacket))
						cmd->viewangles.y += fmodf(Interfaces->Globals->curtime, 3.6f) * 500.f;
				}
			}
		}
	}
}