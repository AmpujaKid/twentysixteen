float OldFrametime;
float OldCurtime;
int m_pCurrentCommand;

void* movehelper;

struct movedata {
	byte _[0x200];
};

void StartPrediction(CUserCmd* cmd, CBaseEntity* localPly) {
	if (!movehelper) return;

	OldFrametime = Interfaces->Globals->frametime;
	OldCurtime = Interfaces->Globals->curtime;

	Interfaces->Globals->frametime = Interfaces->Globals->interval_per_tick;
	Interfaces->Globals->curtime = (float)localPly->m_nTickBase() * Interfaces->Globals->interval_per_tick;

	*(CUserCmd**)((DWORD)localPly + m_pCurrentCommand) = cmd;

	call_vfunc<void(__thiscall*)(void*, void*)>(movehelper, 1)(movehelper, localPly);

	movedata data;
	memset(&data, 0, sizeof(movedata));

	Interfaces->Prediction->SetupMove(localPly, cmd, movehelper, &data);
	Interfaces->GameMovement->ProcessMovement(localPly, &data);
	Interfaces->Prediction->FinishMove(localPly, cmd, &data);

	*(CUserCmd**)((DWORD)localPly + m_pCurrentCommand) = 0;
}