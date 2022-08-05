#define getebp() stack *_bp; __asm mov _bp, ebp;
struct stack{
	stack *next;
};

Vector m_vecCurrentAngles;

void __fastcall hkPlaySound(void*, void*, const char*);
int __fastcall hkEmitAmbientSound(void* pThis, void*, const char* pSample, float flVolume, int iPitch = 100, int flags = 0, float soundtime = 0.0f);

Vector m_vecStrafedAngles;// fix for silent circle strafer
int FakelagTick;

#include "Bunnyhop.h"
#include "AntiAim.h"
#include "Prediction.h"
#include "Aimbot.h"
#include "GetAlivePlayers.h"

Vector m_vecAntiAimAngles; 

bool __fastcall hkCreateMove(void* pThis, void*, float sampletime, CUserCmd* cmd) {
	if (!cmd->commandnumber || !pUser) return true;

	getebp();
	byte* bSendPacket = ((byte*)_bp->next - 0x1C);
	CBaseEntity* localPly = Interfaces->EntList->GetClientEntity(Interfaces->Engine->GetLocalPlayer());
	if (!localPly || localPly->m_lifeState() != 0 || localPly->m_iHealth() < 1) { 
		//KillSounds::Update(false, true);
		return true; 
	}

	m_vecCurrentAngles = cmd->viewangles;
	m_vecStrafedAngles = cmd->viewangles;

	CmdStuff::Bunnyhop(cmd, localPly);

	StartPrediction(cmd, localPly);

	FakelagTick++;
	if (Vars.Fakelag.Mode == 0) {
		*bSendPacket = Vars.Fakelag.Enabled ? ~(FakelagTick % (Vars.Fakelag.Factor + 1)) : 1; // Normal
	}
	else if (Vars.Fakelag.Mode == 1) { // Switch
		static int FakelagFactor = 8;
		*bSendPacket = Vars.Fakelag.Enabled ? ~(FakelagTick % (FakelagFactor + 1)) : 1;
		if (*bSendPacket) {
			FakelagFactor = (rand() % 7) + 6;
		}
	} 
	else if (Vars.Fakelag.Mode == 2) { // Adaptive
		static int FakelagFactor = 2;
		*bSendPacket = Vars.Fakelag.Enabled ? ~(FakelagTick % (FakelagFactor + 1)) : 1;
		if (*bSendPacket)
			FakelagFactor = max(1, min((int)(fabs(localPly->m_vecVelocity().Length() / 80.f)), 12));
	}
	else if (Vars.Fakelag.Mode == 3) { // Pulsive
		static int FakelagFactor = 10;
		static int m_iOldHealth = 0;
		static int iChoked;
		if (m_iOldHealth != localPly->m_iHealth()) {
			m_iOldHealth = localPly->m_iHealth();
			FakelagFactor = rand() % 7 + 6 + 1;
		}
		*bSendPacket = Vars.Fakelag.Enabled ? ~(FakelagTick % (FakelagFactor + 1)) : 1;
		if (!*bSendPacket)
			iChoked++;
		else iChoked = 0;

		if (iChoked > 14) {
			*bSendPacket = 1;
			FakelagTick = -1;
		}
	}

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces->EntList->GetClientEntityFromHandle(localPly->m_hActiveWeapon());

	if (!pWeapon) return;

	static bool bOnce;
	if (!bOnce)
		hkEmitAmbientSound(Interfaces->EngineSound, 0, "music/hankypanky.mp3", 100.f, 100, 0, 0.f);
	bOnce = true;

	if(!pWeapon->IsKnife()){
		if (UTIL::GetPlayersAlive(localPly) != 0) {
			CmdStuff::AntiAim(cmd, localPly, *bSendPacket);

			if (!CmdStuff::IsAngel)
				AngleNormalize(cmd->viewangles);
		}
		else {
			*bSendPacket = 1;
			FakelagTick = -1;
		}

		if (!(*bSendPacket) || Vars.Fakelag.Factor == 0 || !Vars.Fakelag.Enabled || Vars.Aimbot.Silent == 1)
		{
			CmdStuff::LegitAim(cmd, localPly, pWeapon, bSendPacket);
			CmdStuff::Aimbot(cmd, localPly, pWeapon, bSendPacket);
		}
	}
	else {
		*bSendPacket = 1;
		FakelagTick = -1;
	}

	AngleNormalize(m_vecStrafedAngles.y);

	FixMovement(m_vecStrafedAngles, cmd);

	for (int i = 1; i < 65; i++) {
		CBaseEntity* pEnt = Interfaces->EntList->GetClientEntity(i);
		if (pEnt && !pEnt->GetNetworkable()->IsDormant() && pEnt->m_lifeState() == 0) OldSimulationTimes[i] = pEnt->m_flSimulationTime();
		if (!pEnt) {
			Correction_Struct& sPlayer = CorrectionsPlayers[i];
			sPlayer.X = 0.f;
			sPlayer.Y = 0.f;
			sPlayer.MethodX = 0;
			sPlayer.MethodY = 0;
			sPlayer.Resolver = 0;
			sPlayer.Friend = 0;
			sPlayer.Bodyaim = 0;
			sPlayer.AngelAutoCorrection = 1;
			sPlayer.AngelX = 0;
			sPlayer.AngelY = 1;
			sPlayer.ResolverMaxShots = 5;
			Resolver::ResolverPlayer_s& rPlayer = Resolver::ResolverPlayers[i];
			rPlayer.Init();
		}
	}

	if (*bSendPacket)
		m_vecAntiAimAngles = cmd->viewangles;

	return false;
}