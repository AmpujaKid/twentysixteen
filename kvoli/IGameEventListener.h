class IGameEventListener2{
public:
	virtual	~IGameEventListener2(void) {};
	virtual void FireGameEvent(IGameEvent* event) = 0;
};