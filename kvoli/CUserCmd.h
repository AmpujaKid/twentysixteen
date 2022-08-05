class CUserCmd {
public:
	virtual ~CUserCmd() { };
	int commandnumber;
	int tick_count;
	Vector viewangles;
	Vector aimdirection;
	float forwardmove;
	float sidemove;
	float upmove;
	int buttons;
	unsigned char impulse;
	int weaponselect;
	int weaponsubtype;
	int randomseed;
	short mousedx;
	short mousedy;
	bool hasbeenpredicted;
	byte pad[0x18];
};