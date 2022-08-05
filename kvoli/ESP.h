#include "GetWeaponName.h"

Color pColorCTESP = Color(54, 61, 255);
Color pColorTESP = Color(255, 81, 81);

namespace ESP {
	void Draw();
	bool DrawBoundingBox(CBaseEntity*, int&, int&, int&, int&);
	void DrawName(CBaseEntity*, int&, int&, int&);
	void DrawWeaponName(const char*, int&, int&);
	void DrawHealth(CBaseEntity*, int&, int&, int&, int&);
}

void ESP::Draw() {
	CBaseEntity* localPly = Interfaces->EntList->GetClientEntity(Interfaces->Engine->GetLocalPlayer());
	for (int i = 0; i < 65; i++) {
		if (i == Interfaces->Engine->GetLocalPlayer()) continue;
		CBaseEntity* pEnt = Interfaces->EntList->GetClientEntity(i);
		if (!pEnt) continue;
		if (pEnt->GetNetworkable()->IsDormant()) continue;
		if (pEnt->m_lifeState() != 0) continue;
		if (pEnt->m_iHealth() < 1) continue;
		if (Vars.Visuals.Player.EnemyOnly && pEnt->m_iTeamNum() == localPly->m_iTeamNum()) continue;

		int x = 0, y = 0, w = 0, h = 0;

		if (!DrawBoundingBox(pEnt, x, y, w, h))
			continue;

		if(Vars.Visuals.Player.Name) DrawName(pEnt, x, y, h);
		if(Vars.Visuals.Player.Health) DrawHealth(pEnt, x, y, w, h);

		if (!Vars.Visuals.Player.Weapon) continue;

		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces->EntList->GetClientEntityFromHandle(pEnt->m_hActiveWeapon());
		if (!pWeapon) continue;

		DrawWeaponName(UTIL::GetWeaponName(pWeapon->m_iItemDefinitionIndex()), x, y);
	}
}

void ESP::DrawName(CBaseEntity* pEnt, int& x, int& y, int& h) {
	player_info_s pInfo;
	if (!Interfaces->Engine->GetPlayerInfo(pEnt->GetNetworkable()->EntIndex(), &pInfo)) return;

	static unsigned long Font;

	if (!Font) {
		Font = Interfaces->Surface->CreateFont();
		Interfaces->Surface->SetFontGlyphSet(Font, "", 14, 500, 0, 0, 0x80, 0, 0);
	}

	int tw, th;

	Drawing::GetTextSize(Font, tw, th, pInfo.name);

	Drawing::DrawString(Font, x - tw / 2, y - h - th - 1, 220, 220, 220, 255, pInfo.name);
}

void ESP::DrawWeaponName(const char* pWeaponName, int& x, int& y) {
	static unsigned long Font;

	if (!Font) {
		Font = Interfaces->Surface->CreateFont();
		Interfaces->Surface->SetFontGlyphSet(Font, "", 14, 500, 0, 0, 0x80, 0, 0);
	}

	int tw, th;

	Drawing::GetTextSize(Font, tw, th, pWeaponName);
	Drawing::DrawString(Font, x - tw / 2, y + 1, 220, 220, 220, 255, pWeaponName);
}

void ESP::DrawHealth(CBaseEntity* pEnt, int& x, int& y, int& w, int& h) {
	static unsigned long Font;

	if (!Font) {
		Font = Interfaces->Surface->CreateFont();
		Interfaces->Surface->SetFontGlyphSet(Font, "", 14, 500, 0, 0, 0x80, 0, 0);
	}

	int plyHp = pEnt->m_iHealth();
	plyHp = plyHp > 100 ? 100 : plyHp;
	int sPos = plyHp * h / 100;
	int hDelta = h - sPos;

	if (Vars.Visuals.Player.Health == 1 || Vars.Visuals.Player.Health == 3) {
		Drawing::DrawRect(x - w / 2 - 5, y - h - 1, 3, h + 2, 0, 0, 0);
		Drawing::DrawRect(x - w / 2 - 4, y - h + hDelta, 1, sPos, (100 - plyHp) * 2.55, plyHp * 2.55, 0);
	}

	if (Vars.Visuals.Player.Health == 2 || Vars.Visuals.Player.Health == 3) {
		int tw = 0, th = 0;
		Drawing::GetTextSize(Font, tw, th, "%d", pEnt->m_iHealth());
		Drawing::DrawString(Font, x + w / 2 + 2, y - h - 1, 220, 220, 220, 255, "%d", pEnt->m_iHealth());
	}
}

bool ESP::DrawBoundingBox(CBaseEntity* pEnt, int& x, int& y, int& _w, int& _h) {
	Vector vecBottom = pEnt->AbsOrigin();
	vecBottom.z -= 5.f;
	Vector vecTop = vecBottom + Vector(0.f, 0.f, pEnt->m_vecMaxs().z);
	vecTop.z += 10.f;

	Vector vBottom, vTop;

	if (!UTIL::WorldToScreen(vecBottom, vBottom) || !UTIL::WorldToScreen(vecTop, vTop)) return false;

	int w, h;
	h = vBottom.y - vTop.y;
	w = h / 1.75;

	if (Vars.Visuals.Player.Box) {
		if (pEnt->m_iTeamNum() == 2)
			Drawing::DrawOutlinedRect(vBottom.x - w / 2, vBottom.y - h, w, h, pColorTESP.r, pColorTESP.g, pColorTESP.b, 255);
		else
			Drawing::DrawOutlinedRect(vBottom.x - w / 2, vBottom.y - h, w, h, pColorCTESP.r, pColorCTESP.g, pColorCTESP.b, 255);

		if (Vars.Visuals.Player.Outline) {
			if (Vars.Visuals.Player.Outline == 2 || Vars.Visuals.Player.Outline == 3)
				Drawing::DrawOutlinedRect(vBottom.x - w / 2 - 1, vBottom.y - h - 1, w + 2, h + 2, 0, 0, 0, 255);
			if (Vars.Visuals.Player.Outline == 1 || Vars.Visuals.Player.Outline == 3)
				Drawing::DrawOutlinedRect(vBottom.x - w / 2 + 1, vBottom.y - h + 1, w - 2, h - 2, 0, 0, 0, 255);
		}
	}

	x = vBottom.x;
	y = vBottom.y;
	_w = w;
	_h = h;

	return true;
}