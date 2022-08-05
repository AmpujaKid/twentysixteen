struct BackupAngle {
	float X, Y;
};

BackupAngle backup[64];

void __fastcall hkFrameStageNotify(void* pThis, void*, ClientFrameStage_t stage) {
	static int m_angEyeAnglesPitch = NetVarManager->getOffset("DT_CSPlayer", "m_angEyeAngles[0]");
	static int m_angEyeAnglesYaw = NetVarManager->getOffset("DT_CSPlayer", "m_angEyeAngles[1]");
	static int m_vecAngles = NetVarManager->getOffset("DT_CSPlayer", "deadflag") + 0x4;
	static int m_viewPunchAngle = NetVarManager->getOffset("DT_CSPlayer", "m_viewPunchAngle"); 
	static int m_aimPunchAngle = NetVarManager->getOffset("DT_CSPlayer", "m_aimPunchAngle");
	static int m_pPlayerAnimState = NetVarManager->getOffset("DT_CSPlayer", "m_bIsScoped") - 8;
	static int m_flPoseParameter = NetVarManager->getOffset("DT_CSPlayer", "m_flPoseParameter");
	CBaseEntity* localPly = Interfaces->EntList->GetClientEntity(Interfaces->Engine->GetLocalPlayer());

	Vector o_m_vecAngles, o_m_aimPunchAngle, o_m_viewPunchAngle;

	if (stage == FRAME_RENDER_START && localPly && localPly->m_lifeState() == 0 && *(byte*)((DWORD)Interfaces->Input + 0xA5)) {
		o_m_vecAngles = *(Vector*)((DWORD)localPly + m_vecAngles);
		*(Vector*)((DWORD)localPly + m_vecAngles) = m_vecAntiAimAngles;
	}

	if (stage == FRAME_RENDER_START && localPly) {
		o_m_viewPunchAngle = *(Vector*)((DWORD)localPly + m_viewPunchAngle);
		o_m_aimPunchAngle = *(Vector*)((DWORD)localPly + m_aimPunchAngle);
		(*(Vector*)((DWORD)localPly + m_viewPunchAngle)).Init();
		(*(Vector*)((DWORD)localPly + m_aimPunchAngle)).Init();
	}

	ClientVmt->getOrig<void(__thiscall*)(void*, ClientFrameStage_t)>(36)(pThis, stage);

	if (stage == 2) {
		for (int i = 1; i < 65; i++) {
			CBaseEntity* pEnt = Interfaces->EntList->GetClientEntity(i);
			if (!pEnt) continue;
			if (pEnt->GetNetworkable()->IsDormant()) continue;
			if (pEnt->m_iHealth() < 1) continue;
			if (pEnt->m_lifeState() != 0) continue;

			*(float*)((DWORD)pEnt + m_angEyeAnglesPitch) = DoPitchAAA(pEnt);
			*(float*)((DWORD)pEnt + m_angEyeAnglesYaw) = DoYawAAA(pEnt);
			//Msg("%f\n", *(float*)((DWORD)pEnt + m_angEyeAnglesYaw));
		}
	}

	if (stage == FRAME_RENDER_START && localPly) {
		*(Vector*)((DWORD)localPly + m_viewPunchAngle) = o_m_viewPunchAngle;
		*(Vector*)((DWORD)localPly + m_aimPunchAngle) = o_m_aimPunchAngle;
	}

	if (stage == FRAME_RENDER_START && localPly && localPly->m_lifeState() == 0 && *(byte*)((DWORD)Interfaces->Input + 0xA5)) {
		*(Vector*)((DWORD)localPly + m_vecAngles) = o_m_vecAngles;
	}
}