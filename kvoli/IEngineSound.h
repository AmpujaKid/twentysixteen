class IEngineSound {
public:
	VFUNCR3(PrecacheSound, 0, bool, const char*, bool, bool);
	VFUNCR1(IsSoundPrecached, 1, bool, const char*);
	VFUNCR1(GetSoundDuration, 4, float, const char*);
	VFUNC1(StopSound, 8, int);
};