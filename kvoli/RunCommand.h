void __fastcall hkRunCommand(void* pThis, void*, CBaseEntity* pEnt, CUserCmd* cmd, void* move_helper) {
	PredictionVmt->hookTable(false);
	PredictionVmt->getOrig<void(__thiscall*)(void*, CBaseEntity*, CUserCmd*, void*)>(19)(pThis, pEnt, cmd, move_helper);
	PredictionVmt->hookTable(true);
	movehelper = move_helper;
}