#define FLOW_OUTGOING 0
#define FLOW_INCOMING 1
#define MAX_FLOWS 2

class CNetChannelInfo {
public:
	VFUNCR1(GetLatency, 9, float, int);
};