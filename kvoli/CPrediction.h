class CUserCmd;

class IPrediction {
public:
	VFUNC4(SetupMove, 20, CBaseEntity*, CUserCmd*, void*, void*);
	VFUNC3(FinishMove, 21, CBaseEntity*, CUserCmd*, void*);
};