class CNetChannelInfo;

class IVEngineClient {
public:
	VFUNCR2(GetPlayerInfo, 8, bool, int, player_info_s*);
	VFUNCR0(GetLocalPlayer, 12, int);
	VFUNC1(SetViewAngles, 19, Vector&);
	VFUNCR0(IsInGame, 26, bool);
	VFUNCR0(IsConnected, 27, bool);
	VFUNCR0(GetMaxPlayers, 20, int);
	VFUNC1(ExecuteClientCmd, 108, const char*);
	VFUNCR0(GetNetChannelInfo, 78, CNetChannelInfo*);
	VFUNCR1(GetPlayerForUserID, 9, int, int);
};