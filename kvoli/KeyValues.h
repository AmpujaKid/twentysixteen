class KeyValues{
public:
	KeyValues() {}
	KeyValues(const char*);
	bool LoadFromBuffer(char const *resourceName, const char *pBuffer, void* pFileSystem = NULL, const char *pPathID = NULL, void* pfnEvaluateSymbolProc = NULL);
	byte pad[64U];
};

KeyValues::KeyValues(const char* name) {
	static DWORD dwAddress = UTIL::dwFindPattern((DWORD_PTR)GetModuleHandleA("client.dll"), 0xFFFFFF, (BYTE*)"\x55\x8B\xEC\x51\x33\xC0\xC7\x45\x00\x00\x00\x00\x00\x56\x8B\xF1\x81\x26\x00\x00\x00\x00\xC6\x46\x03\x00\x89\x46\x10\x89\x46\x18\x89\x46\x14\x89\x46\x1C\x89\x46\x04\x89\x46\x08\x89\x46\x0C\xFF\x15\x00\x00\x00\x00\x6A\x01\xFF\x75\x08\x8D\x4D\xFC\x8B\x10\x51\x8B\xC8\xFF\x52\x24\x8B\x0E\x33\x4D\xFC\x81\xE1\x00\x00\x00\x00\x31\x0E\x88\x46\x03", "xxxxxxxx?????xxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxx????xxxxx");
	typedef void(__thiscall* InitKeyValues)(KeyValues*, const char*);
	((InitKeyValues)(dwAddress))(this, name);
}

bool KeyValues::LoadFromBuffer(char const *resourceName, const char *pBuffer, void* pFileSystem, const char *pPathID, void* pfnEvaluateSymbolProc){
	static DWORD dwAddress = UTIL::dwFindPattern((DWORD_PTR)GetModuleHandleA("client.dll"), 0xFFFFFF, (BYTE*)"\x55\x8B\xEC\x83\xEC\x48\x53\x56\x57\x8B\xF9\x89\x7D\xF4", "xxxxxxxxxxxxxx");
	typedef bool(__thiscall* LoadFromBuffer)(void*, const char*, const char*, void*, const char*, void*);
	return ((LoadFromBuffer)(dwAddress))(this, resourceName, pBuffer, 0, 0, 0);
}