namespace UTIL {
	int GetPlayersAlive(CBaseEntity*);
}

int UTIL::GetPlayersAlive(CBaseEntity* localPly) {
	int AlivePlayers = 0;
	for (int i = 0; i < 65; i++) {
		if (i == Interfaces->Engine->GetLocalPlayer()) continue;
		CBaseEntity* pEnt = Interfaces->EntList->GetClientEntity(i);
		if (!pEnt) continue;
		if (pEnt->m_iTeamNum() == localPly->m_iTeamNum() || (pEnt->m_iTeamNum() != 3 && pEnt->m_iTeamNum() != 2)) continue;
		if (pEnt->m_lifeState() == 0)
			AlivePlayers++;
	}

	return AlivePlayers;
}