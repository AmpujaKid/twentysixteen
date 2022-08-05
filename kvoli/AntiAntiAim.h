struct Correction_Struct {
	float X, Y;
	int MethodX, MethodY;
	int Resolver, Friend, Bodyaim, AngelAutoCorrection = 1;
	int ResolverMaxShots = 2;
	int AngelX, AngelY = 3;
};

Correction_Struct CorrectionsPlayers[65];
static int CurrentStage[65];
RecvVarProxyFn OldProxy_X, OldProxy_Y;

#include "Resolver.h"

void FixAngel2(CBaseEntity* pEnt, float* pfPitch, float* pfYaw) {
	int index = pEnt->GetNetworkable()->EntIndex();
	if (pfYaw) {
		static float SpinYaw[65], TwitchTime[65];
		static bool Twitch[65];
		SpinYaw[index] += Interfaces->Globals->frametime * 135.f;
		if (SpinYaw[index] > 360.f) SpinYaw[index] = 0.f;

		TwitchTime[index] -= Interfaces->Globals->frametime;
		if (TwitchTime[index] <= 0.f) {
			Twitch[index] = !Twitch[index];
			TwitchTime[index] = 0.02f;
		}

		*pfYaw = SpinYaw[index] - (Twitch[index] ? 180.f : 0.f);
	}
}

void Twitch(CBaseEntity* pEnt, float* pfPitch, float* pfYaw) {
	int index = pEnt->GetNetworkable()->EntIndex();

	if (pfPitch) {
		if (CurrentStage[index] < 4)
			*pfPitch = 89.f;
		else if (CurrentStage[index] < 8)
			*pfPitch = -89.f;
	}

	if (pfYaw) {
		if (CurrentStage[index] == 0 || CurrentStage[index] == 4)
			*pfYaw -= 90.f;
		else if (CurrentStage[index] == 1 || CurrentStage[index] == 5)
			*pfYaw -= 180.f;
		else if (CurrentStage[index] == 2 || CurrentStage[index] == 6)
			*pfYaw += 90.f;
		else if (CurrentStage[index] == 3 || CurrentStage[index] == 7)
			*pfYaw -= 0.f;
	}
}

void FixAngel(CBaseEntity* pEnt, float* pfPitch, float* pfYaw) {

	int index = pEnt->GetNetworkable()->EntIndex();

	if (pfPitch){
		if (CorrectionsPlayers[index].AngelX == 0) { // auto
			*pfPitch = 89.f;
		}
		else if (CorrectionsPlayers[index].AngelX == 1) { // twitch
			Twitch(pEnt, pfPitch, 0);
		}
		else if (CorrectionsPlayers[index].AngelX == 2) { // Down
			*pfPitch = 89.f;
		}
		else if (CorrectionsPlayers[index].AngelX == 3) { // Up
			*pfPitch = -89.f;
		}
	}

	if (pfYaw) {
		if (CorrectionsPlayers[index].AngelY == 0) { // Auto
			*pfYaw -= 180.f;
		}
		else if (CorrectionsPlayers[index].AngelY == 3) { // Twitch
			Twitch(pEnt, 0, pfYaw);
		}
		else if (CorrectionsPlayers[index].AngelY == 1) { // Half Left
			*pfYaw -= 90.f;
		}
		else if (CorrectionsPlayers[index].AngelY == 2) { // Half Right
			*pfYaw -= 270.f;
		}
		else if (CorrectionsPlayers[index].AngelY == 4) // Twitch2
			FixAngel2(pEnt, 0, pfYaw);
	}
}

float DoPitchAAA(CBaseEntity* pEnt) {
	int index = pEnt->GetNetworkable()->EntIndex();
	float pitch = CorrectionsPlayers[index].X;
	static int angletwitch[65];

	/*if (CorrectionsPlayers[index].Resolver)
		return Resolver::GetPitch(pEnt);*/

	if (CorrectionsPlayers[index].MethodX == 0 || CorrectionsPlayers[index].MethodX == 3) {//auto
		if (pitch == 180.f) pitch = -89.f;
		else if (pitch > 180.f && pitch < 290.f) pitch = -89.f;
		else if (pitch >= 89.f && pitch <= 180.f) pitch = 89.f;
		else if (pitch == 0.f && CorrectionsPlayers[index].AngelAutoCorrection)
			FixAngel(pEnt, &pitch, 0);

		if (CorrectionsPlayers[index].MethodX == 3 && !(CorrectionsPlayers[index].AngelAutoCorrection && pitch == 0.f)) { // inverse
			pitch = -pitch;
		}
	}
	else if (CorrectionsPlayers[index].MethodX == 1) { //down
		pitch = 89.f;
	}
	else if (CorrectionsPlayers[index].MethodX == 2) { // up
		pitch = -89.f;
	}
	else if (CorrectionsPlayers[index].MethodX == 4) {//twitch
		Twitch(pEnt, &pitch, 0);
	}

	return pitch;
}


void AAA_Pitch(const CRecvProxyData* pData, void* pStruct, void* pOut) {
	float pitch = pData->m_Value.m_Float;
	int index = ((CBaseEntity*)pStruct)->GetNetworkable()->EntIndex();
	CorrectionsPlayers[index].X = pitch;

	OldProxy_X(pData, pStruct, pOut);
}

float DoYawAAA(CBaseEntity* pEnt) {
	int index = pEnt->GetNetworkable()->EntIndex();
	if (CorrectionsPlayers[index].Resolver || Vars.Aimbot.Accuracy.Resolver)
		return Resolver::GetYaw(pEnt);

	float yaw = CorrectionsPlayers[index].Y;
	if(yaw != 0.f)
		yaw -= .087936f;

	static bool tempBools[65];
	static bool flip[65];
	static bool twitch[65];
	static int angletwitch[65];

	if (CorrectionsPlayers[index].MethodY == 0) { // auto
		bool twitch = tempBools[index];
		tempBools[index] = yaw >= 180.f && yaw <= 360.f;
		if (yaw == 0.f && CorrectionsPlayers[index].AngelAutoCorrection) { // angel ghetto fix
			FixAngel(pEnt, 0, &yaw);
		} else if (twitch && yaw >= 0.f && yaw <= 180.f) yaw += 359.f;
	}
	else if (CorrectionsPlayers[index].MethodY == 1) { // half-left
		yaw -= 270.f;
	}
	else if (CorrectionsPlayers[index].MethodY == 2) { // half right;
		yaw -= 90.f;
	}
	else if (CorrectionsPlayers[index].MethodY == 3) { // inverse
		yaw -= 180.f;
	}
	else if (CorrectionsPlayers[index].MethodY == 4) { // twitch
		Twitch(pEnt, 0, &yaw);
	} else if (CorrectionsPlayers[index].MethodY == 5) // Twitch 2
		FixAngel2(pEnt, 0, &yaw);

	return yaw;
}

void AAA_Yaw(const CRecvProxyData* pData, void* pStruct, void* pOut) {
	float yaw = pData->m_Value.m_Float;
	int index = ((CBaseEntity*)pStruct)->GetNetworkable()->EntIndex();
	CorrectionsPlayers[index].Y = yaw;
	
	OldProxy_Y(pData, pStruct, pOut);
}