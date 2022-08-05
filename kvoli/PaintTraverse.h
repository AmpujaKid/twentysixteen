#include "W2S.h"
#include "ESP.h"
#include "StepMania.h"
#include "Menu.h"
#include "PlayerList.h"

void ASUS();

void __fastcall hkPaintTraverse(void* pThis, void*, unsigned int Panel, bool allowForce, bool forceRepaint) {
	PanelVmt->getOrig<void(__thiscall*)(void*, unsigned int, bool, bool)>(41)(pThis, Panel, allowForce, forceRepaint);

	static unsigned int overlayPanel;

	if (!overlayPanel)
		if (strstr(Interfaces->Panel->GetName(Panel), "MatSystemTopPanel"))
			overlayPanel = Panel;
	
	if (!overlayPanel || overlayPanel != Panel) return;

//	if (!Stepmania::bHasPrecached) Stepmania::Precache();

	if (!pUser) {
		int ScrW = 0, ScrH = 0;
		Interfaces->Surface->GetScreenSize(ScrW, ScrH);
		for (int i = 0; i <= ScrH; i++) {
			Interfaces->Surface->DrawSetColor(rand() % 255, rand() % 255, rand() % 255, 255);
			Interfaces->Surface->DrawLine(0, i, ScrW, i);
		}
		static unsigned long Font;
		if (!Font) {
			Font = Interfaces->Surface->CreateFont();
			Interfaces->Surface->SetFontGlyphSet(Font, "Fixedsys", 60, 500, 0, 0, FONTFLAG_DROPSHADOW, 0, 0);
		}
		int tw = 0, th = 0;
		Drawing::GetTextSize(Font, tw, th, "Excuse me?");
		Drawing::DrawString(Font, ScrW / 2 - tw / 2, ScrH / 2 - th / 2, 220, 220, 220, 255, "Excuse me?");

		static float wait;
		wait -= Interfaces->Globals->frametime;
		return;
	}

	/*CBaseEntity* pLocal = Interfaces->EntList->GetClientEntity(Interfaces->Engine->GetLocalPlayer());
	if (pLocal && Menu::ScrW) {
		CViewSetup pViewSetup;
		if (Interfaces->Client->GetViewSetup(pViewSetup)) {
			pViewSetup.x = Menu::ScrW;
			pViewSetup.y = Menu::ScrH;
			pViewSetup.origin = pLocal->AbsOrigin() + pLocal->m_vecViewOffset();
			pViewSetup.angles = Vector(0, 0, 0);
			pViewSetup.angles.y += 180.0f; //rotate yaw
			pViewSetup.fov = 90.0f; //default fov
			Interfaces->Client->RenderView(pViewSetup, 0, 2); //render our view on the screen
		}
	}*/

	ESP::Draw();
	Menu::Draw();

	ASUS();

	Resolver::UpdatePlayers();

	static float TimeForNext[65];
	
	for (int i = 1; i < 65; i++) {
		TimeForNext[i] -= Interfaces->Globals->frametime;
		
		if (TimeForNext[i] <= 0.f) {
			TimeForNext[i] = 0.1f;
			CurrentStage[i]++;
		}

		if (CurrentStage[i] >= 8)
			CurrentStage[i] = 0;
	}

	//if(Stepmania::bDrawOverlay) Stepmania::DrawOverlay();

	/*if (!Stepmania::bDrawOverlay) {
		if (GetAsyncKeyState(VK_F6) & 0x1) {
			Stepmania::ConfirmSelection();
		}
	}*/
}