namespace Resolver {
	struct ResolverPlayer_s {
		float LastY, LastX;
		int CurrentY, CurrentX;
		int BulletsMissed;
		int ResolverMethod = 2;
		float LastFrame;
		std::vector<float> MissedShotsVector;
		void Init() {
			LastY = 0;
			LastX = 0;
			CurrentX = 0;
			CurrentY = 0;
			BulletsMissed = 0;
			MissedShotsVector.clear();
			ResolverMethod = 2;
		}
	};
	ResolverPlayer_s ResolverPlayers[65];
	void UpdatePlayer(CBaseEntity*, bool = false);
	void UpdatePlayers();
	void AddShot(CBaseEntity*, float);
	float GetPitch(CBaseEntity*);
	float GetPitch(int);
	float GetYaw(CBaseEntity*);
	float GetYaw(int);
}

float Resolver::GetPitch(CBaseEntity* pEnt) {
	return GetPitch(pEnt->GetNetworkable()->EntIndex());
}

float Resolver::GetYaw(CBaseEntity* pEnt) {
	return GetYaw(pEnt->GetNetworkable()->EntIndex());
}

void Resolver::UpdatePlayer(CBaseEntity* pEnt, bool bAddMissed) {
	ResolverPlayer_s& sPlayer = ResolverPlayers[pEnt->GetNetworkable()->EntIndex()];

	if (sPlayer.MissedShotsVector.empty()) return;

	if (bAddMissed) {
		sPlayer.BulletsMissed++;
		sPlayer.MissedShotsVector.erase(sPlayer.MissedShotsVector.begin());
	}

	if (sPlayer.BulletsMissed >= CorrectionsPlayers[pEnt->GetNetworkable()->EntIndex()].ResolverMaxShots) {
		sPlayer.MissedShotsVector.clear();
		sPlayer.CurrentY++;
		sPlayer.CurrentX++;
		sPlayer.BulletsMissed = 0;
	}
}

void Resolver::UpdatePlayers(){
	for (int i = 1; i < 65; i++) {
		CBaseEntity* pEnt = Interfaces->EntList->GetClientEntity(i);
		if (!pEnt) {
			ResolverPlayers[i].Init();
			continue;
		}

		ResolverPlayer_s& sPlayer = ResolverPlayers[i];
		/*if (sPlayer.MissedShotsVector.empty()) continue;

		float& Shot = sPlayer.MissedShotsVector.at(0);

		Shot -= Interfaces->Globals->frametime;

		if (Shot <= 0.f)
			UpdatePlayer(pEnt, true);*/

		sPlayer.LastFrame -= Interfaces->Globals->frametime;

		if (sPlayer.LastFrame <= 0.f) {
			sPlayer.CurrentX++;
			sPlayer.CurrentY++;
			sPlayer.LastFrame = 0.08f;
		}
	}
}

void Resolver::AddShot(CBaseEntity* pEnt, float fTime) {
	ResolverPlayer_s& sPlayer = ResolverPlayers[pEnt->GetNetworkable()->EntIndex()];
	sPlayer.MissedShotsVector.push_back(fTime);
}

float Resolver::GetPitch(int iEntIndex) {
	ResolverPlayer_s& sPlayer = ResolverPlayers[iEntIndex];

	if (sPlayer.CurrentX >= 8)
		sPlayer.CurrentX = 0;

	if (sPlayer.CurrentX < 4)
		return 89.f;
	else if (sPlayer.CurrentX < 8)
		return -89.f;

	return 89.f;
}

float Resolver::GetYaw(int iEntIndex) {
	float flYaw = CorrectionsPlayers[iEntIndex].Y;

	ResolverPlayer_s& sPlayer = ResolverPlayers[iEntIndex];

	int iResolverMethod = (CorrectionsPlayers[iEntIndex].Resolver) ? sPlayer.ResolverMethod : Vars.Aimbot.Accuracy.Resolver - 1;
	
	if (iResolverMethod == 0) { // Full Spin
		if (sPlayer.CurrentY >= 4)
			sPlayer.CurrentY = 0;

		return flYaw - (float)(sPlayer.CurrentY * 90);
	}
	else if (iResolverMethod == 1) { // 45 Step
		if (sPlayer.CurrentY >= 8)
			sPlayer.CurrentY = 0;

		return flYaw + (float)(sPlayer.CurrentY * 45);
	}
	else if (iResolverMethod == 2) { // Jitter
		if (sPlayer.CurrentY >= 2)
			sPlayer.CurrentY = 0;

		if (sPlayer.CurrentY)
			return flYaw - 60.f;
		else
			return flYaw + 60.f;
	}

	return flYaw;
}