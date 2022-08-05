namespace UTIL {
	bool bDataCompare(const BYTE*, const BYTE*, const char*);
	DWORD dwFindPattern(DWORD, DWORD, BYTE *, char *);
}

bool UTIL::bDataCompare(const BYTE *pData, const BYTE *bMask, const char *szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;
	return (*szMask) == NULL;
}

DWORD UTIL::dwFindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask)
{
	for (DWORD i = 0; i < dwLen; i++)// ye not good cus u need that version of visual studio,  u can send this to like syrah and they can build it if they want to
		if (bDataCompare((BYTE*)(dwAddress + i), bMask, szMask))
			return (DWORD)(dwAddress + i);
	return 0;
}