#include "kvoli.h"
#include <thread>

void CheatInit() {
	while (!GetModuleHandleA("tier0.dll"))
		Sleep(100);
	Msg = IMPORT("tier0.dll", "Msg", MsgFn);

	Interfaces = new CInterfaces();
	NetVarManager = new CNetVarManager();

	SurfaceVmt = new CHooker(Interfaces->Surface);
	SurfaceVmt->hookFunc(hkPlaySound, 82);
	SurfaceVmt->hookTable(true);

	ClientModeVmt = new CHooker(Interfaces->ClientMode);
	ClientModeVmt->hookFunc(hkCreateMove, 24);
	ClientModeVmt->hookFunc(hkOverrideView, 18);
	ClientModeVmt->hookTable(true);

	PanelVmt = new CHooker(Interfaces->Panel);
	PanelVmt->hookFunc(hkPaintTraverse, 41);
	PanelVmt->hookTable(true);

	PredictionVmt = new CHooker(Interfaces->Prediction);
	PredictionVmt->hookFunc(hkRunCommand, 19);
	PredictionVmt->hookTable(true);

	ClientVmt = new CHooker(Interfaces->Client);
	ClientVmt->hookFunc(hkFrameStageNotify, 36);
	ClientVmt->hookTable(true);

	MaterialSystemVmt = new CHooker(Interfaces->MaterialSystem);
	MaterialSystemVmt->hookFunc(hkFindMaterial, 84);
	MaterialSystemVmt->hookTable(true);

	EngineSoundVmt = new CHooker(Interfaces->EngineSound);
	EngineSoundVmt->hookFunc(hkEmitAmbientSound, 12);
	EngineSoundVmt->hookTable(true);

	ModelRenderVmt = new CHooker(Interfaces->ModelRender);
	ModelRenderVmt->hookFunc(hkDrawModelExecute, 21);
	ModelRenderVmt->hookTable(true);

	OldProxy_X = (RecvVarProxyFn)NetVarManager->hookProp("DT_CSPlayer", "m_angEyeAngles[0]", AAA_Pitch);
	OldProxy_Y = (RecvVarProxyFn)NetVarManager->hookProp("DT_CSPlayer", "m_angEyeAngles[1]", AAA_Yaw);

	m_pCurrentCommand = NetVarManager->getOffset("DT_CSPlayer", "m_hConstraintEntity") - 0xC;

	srand(time(0));

	if (pUser) {
		Msg("kvoli\n");
		Msg("Welcome, Mochi!\n", pUser);
	} else 
		Msg("bad\n");

//	Stepmania::Init();
}

int __stdcall DllMain(void*, DWORD reason, void*) {
	if (reason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CheatInit, 0, 0, 0);
	}
	else if(reason == DLL_PROCESS_DETACH) {
		Menu::DeleteObjects();
		delete Interfaces;
		delete SurfaceVmt;
		delete NetVarManager;
		delete ClientModeVmt;
		delete ClientVmt;
		delete PanelVmt;
		delete PredictionVmt;
		delete MaterialSystemVmt;
		delete GameEventManagerVmt;
		delete ModelRenderVmt;
	}
	return 1;
}