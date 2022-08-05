class CNetVarManager {
public:
	CNetVarManager();
	~CNetVarManager();
	int getOffset(const char*, const char*);
	DWORD hookProp(const char*, const char*, void*, void* = 0);
private:
	int getProp(const char*, const char*, RecvProp** = 0);
	int getProp(RecvTable*, const char*, RecvProp** = 0);
	RecvTable* getTable(const char*);
	std::vector<RecvTable*> recvTables;
};

CNetVarManager::CNetVarManager() {
	ClientClass* cClass = Interfaces->Client->GetAllClasses();
	while (cClass) {
		recvTables.push_back(cClass->m_pRecvTable);
		cClass = cClass->m_pNext;
	}
}

CNetVarManager::~CNetVarManager() {
	recvTables.clear();
}

DWORD CNetVarManager::hookProp(const char* tableName, const char* propName, void* hkFunc, void* oldFn) {
	RecvProp* recvProp;
	getProp(tableName, propName, &recvProp);

	if (!recvProp)
		return false;

	DWORD old = (DWORD)recvProp->m_ProxyFn;
	recvProp->m_ProxyFn = (RecvVarProxyFn)hkFunc;
	return old;
}

int CNetVarManager::getOffset(const char* tableName, const char* propName) {
	return getProp(tableName, propName);
}

int CNetVarManager::getProp(const char* tableName, const char* propName, RecvProp** prop) {
	RecvTable* recvTable = getTable(tableName);
	if (!recvTable)
		return 0;

	return getProp(recvTable, propName, prop);
}

int CNetVarManager::getProp(RecvTable* recvTable, const char* propName, RecvProp** prop) {
	int curOffset = 0;

	for (int i = 0; i < recvTable->m_nProps; i++) {
		RecvProp *recvProp = &recvTable->m_pProps[i];
		RecvTable *child = recvProp->m_pDataTable;

		curOffset += (child && (child->m_nProps > 0) && getProp(child, propName, prop)) ? recvProp->m_Offset + getProp(child, propName, prop) : 0;

		if (_stricmp(recvProp->m_pVarName, propName))
			continue;

		if (prop)
			*prop = recvProp;

		return recvProp->m_Offset + curOffset;
	}

	return curOffset;
}

RecvTable* CNetVarManager::getTable(const char* tableName) {
	for (auto table : recvTables) {
		if (!table)
			continue;

		if (!_stricmp(table->m_pNetTableName, tableName))
			return table;
	}

	return NULL;
}

CNetVarManager* NetVarManager;

#define NETVAR(t, cn, vn)\
__forceinline t& vn()\
{ \
	static int offset = NetVarManager->getOffset(TOSTR(cn), TOSTR(vn));\
	return *(t*)((DWORD)this + offset);\
}