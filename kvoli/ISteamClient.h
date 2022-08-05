class ISteamClient {
public:
	VFUNCR0(CreateSteamPipe, 0, int);
	VFUNCR1(ReleaseSteamPipe, 1, bool, int);
	VFUNCR1(ConnectToGlobalUser, 2, int, int);
	VFUNC2(ReleaseUser, 4, int, int);
	VFUNCR3(GetSteamUser, 5, ISteamUser*, int, int, const char*);
};