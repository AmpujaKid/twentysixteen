class CViewSetup;

class IBaseClientDLL {
public:
	VFUNCR0(GetAllClasses, 8, ClientClass*);
	VFUNCR1(GetViewSetup, 59, bool, CViewSetup&);
	VFUNC3(RenderView, 28, const CViewSetup&, int, int);
};