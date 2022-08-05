class CHooker {
public:
	CHooker(void*);
	~CHooker();
	void hookFunc(void*, DWORD);
	void unhookFunc(DWORD);
	void hookTable(bool);
	template< typename Fn >
	Fn getOrig(DWORD);
private:
	DWORD ***baseTable, **origVmt, **newVmt, tableCount;
};

CHooker::CHooker(void* vmt) {
	baseTable = (DWORD***)vmt;
	origVmt = *baseTable;

	while (origVmt[tableCount]) tableCount++;

	newVmt = new DWORD*[tableCount];
	memcpy(newVmt, origVmt, sizeof(DWORD*) * tableCount);

	*baseTable = origVmt;
}

CHooker::~CHooker() {
	delete[] newVmt;
}

void CHooker::hookFunc(void* hkFunc, DWORD index) {
	DWORD oldProtect;
	VirtualProtect(newVmt[index], sizeof(DWORD*), PAGE_EXECUTE_READWRITE, &oldProtect);
	newVmt[index] = (DWORD*)hkFunc;
	VirtualProtect(newVmt[index], sizeof(DWORD*), oldProtect, 0);
}

void CHooker::unhookFunc(DWORD index) {
	DWORD oldProtect;
	VirtualProtect(newVmt[index], sizeof(DWORD*), PAGE_EXECUTE_READWRITE, &oldProtect);
	newVmt[index] = origVmt[index];
	VirtualProtect(newVmt[index], sizeof(DWORD*), oldProtect, 0);
}

void CHooker::hookTable(bool hooked) {
	*baseTable = hooked ? newVmt : origVmt;
}

template< typename Fn >
Fn CHooker::getOrig(DWORD index) {
	return (Fn)origVmt[index];
}