class IGameEvent{
public:
	VFUNCR0(GetName, 1, const char*);
	VFUNCR2(GetBool, 5, bool, const char*, bool);
	VFUNCR2(GetInt, 6, int, const char*, int);
	VFUNCR2(GetUint64, 7, UINT64, const char*, UINT64);
	VFUNCR2(GetFloat, 8, float, const char*, float);
	VFUNCR2(GetString, 9, const char*, const char*, const char*);
	VFUNCR2(GetWString, 9, const wchar_t*, const char*, const wchar_t*);
};