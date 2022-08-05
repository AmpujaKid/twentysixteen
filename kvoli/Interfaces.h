#include "CEngineClient.h"
#include "IBaseClientDLL.h"
#include "CClientEntityList.h"
#include "CIVDebugOverlay.h"
#include "IPanel.h"
#include "ISurface.h"
#include "CPrediction.h"
#include "CGameMovement.h"
#include "CGlobalVarsBase.h"
#include "CClientModeShared.h"
#include "CEngineTrace.h"
#include "CNetChannelInfo.h"
#include "IMaterialSystem.h"
#include "ICvar.h"
#include "IVModelRender.h"
#include "IVModelInfo.h"
#include "IInput.h"
#include "KeyValuesSystem.h"
#include "IGameEvent.h"
#include "IGameEventListener.h"
#include "IGameEventManager.h"
#include "IVRenderView.h"
#include "ISteamUser.h"
#include "ISteamClient.h"
#include "IEngineSound.h"

class CInterfaces {
public:
	CInterfaces();
	IVEngineClient* Engine;
	IBaseClientDLL* Client;
	CClientEntityList* EntList;
	IVDebugOverlay* DebugOverlay;
	IPanel* Panel;
	ISurface* Surface;
	CEngineTrace* Trace;
	IPrediction* Prediction;
	IGameMovement* GameMovement;
	IMaterialSystem* MaterialSystem;
	ICvar* CVar;
	IVModelRender* ModelRender;
	IVModelInfo* ModelInfo;
	IVRenderView* RenderView;
	IEngineSound* EngineSound;
	IGameEventManager2* GameEventManager;
	CGlobalVarsBase* Globals;
	CClientModeShared** ClientMode;
	IInput* Input;
}; CInterfaces* Interfaces;

const char* InterfaceNames[] = {
	"VEngineClient",
	"VClient",
	"VClientEntityList",
	"VDebugOverlay",
	"VGUI_Panel",
	"VGUI_Surface",
	"EngineTraceClient",
	"VClientPrediction",
	"GameMovement",
	"VMaterialSystem",
	"VEngineCvar",
	"VEngineModel",
	"VModelInfoClient",
	"VEngineRenderView",
	"IEngineSoundClient",
};

const char* ModuleNames[] = {
	"engine.dll",
	"client.dll",
	"client.dll",
	"engine.dll",
	"vgui2.dll",
	"vguimatsurface.dll",
	"engine.dll",
	"client.dll",
	"client.dll",
	"materialsystem.dll",
	"vstdlib.dll",
	"engine.dll",
	"engine.dll",
	"engine.dll",
	"engine.dll",
};

CInterfaces::CInterfaces() {
	typedef void*(*CreateInterfaceFn)(const char*, int);
	char buf[64];

	/* ghetto good for you */

	for (int i = 0; i < (sizeof(InterfaceNames) / 4); i++) {
		while (!GetModuleHandleA(ModuleNames[i])) { Sleep(100); }
		CreateInterfaceFn TempFactory = IMPORT(ModuleNames[i], "CreateInterface", CreateInterfaceFn);

		for (int version = 0; version < 100; version++) {
			sprintf(buf, "%s%03d", InterfaceNames[i], version);
			void* TempInterface = TempFactory(buf, 0);
			if (TempInterface) {
				*(void**)((DWORD)this + i * 4) = TempInterface;
				Msg("Grabbed %s (%s)\n", buf, ModuleNames[i]);
				break;
			}
		}
	}

	ClientMode = *(CClientModeShared***)*(void***)((DWORD)(*(void***)Client)[10] + 5);
	while (!ClientMode) {
		ClientMode = *(CClientModeShared***)*(void***)((DWORD)(*(void***)Client)[10] + 5);
		Msg("clientmode not found, regrabbing\n");
		Sleep(100);
	}

	Globals = **(CGlobalVarsBase***)((*(DWORD**)Client)[0] + 0x53);
	while (!Globals) {
		Globals = **(CGlobalVarsBase***)((*(DWORD**)Client)[0] + 0x53);
		Msg("globals not found, regrabbing\n");
		Sleep(100);
	}

	Input = *(IInput**)((*(DWORD**)Client)[15] + 0x1);
	while (!Input) {
		Input = *(IInput**)((*(DWORD**)Client)[15] + 0x1);
		Msg("input not found, regrabbing\n");
		Sleep(100);
	}

	pUser = getenv("STEAMID");
	if (strcmp(pUser, "76561197960407380") && strcmp(pUser, "76561197968259598") && strcmp(pUser, "76561198067588385"))
		pUser = 0;

	if (pUser) {
		if (!strcmp(pUser, "76561197960407380"))
			pUser = "razor";
		else if (!strcmp(pUser, "76561197968259598"))
			pUser = "Edd";
		else if (!strcmp(pUser, "76561198067588385"))
			pUser = "Capn";
	}

	pUser = " ";

	//GameEventManager = (IGameEventManager2*)(IMPORT("engine.dll", "CreateInterface", CreateInterfaceFn))("GAMEEVENTSMANAGER002", 0);
	//Engine->ExecuteClientCmd("clear");

	/*DWORD m_GlowManagerReturner = UTIL::dwFindPattern((DWORD)GetModuleHandleA("client.dll"), 0xFFFFFFFF, (BYTE*)"\xA1\x00\x00\x00\x00\xA8\x01\x75\x4E\x0F\x57\xC0", "x????xxxxxxx");
	void* m_GlowManager = ((void*(*)())m_GlowManagerReturner)();*/
}