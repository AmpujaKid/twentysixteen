#include "ASUS.h"

IMaterial* __fastcall hkFindMaterial(void* pThis, void*, const char* pMaterialName, const char* pTextureGroup, bool complain, const char* pComplainPrefix) {
	MaterialSystemVmt->hookTable(false);
	IMaterial* pMaterial = MaterialSystemVmt->getOrig<IMaterial*(__thiscall*)(void*, const char*, const char*, bool, const char*)>(84)(pThis, pMaterialName, pTextureGroup, complain, pComplainPrefix);
	MaterialSystemVmt->hookTable(true);
	if (pMaterial && pMaterialName && pTextureGroup) {

		if (strstr(pTextureGroup, "World"))
			WorldMaterials.push_back(pMaterial);
		else if (strstr(pTextureGroup, "SkyBox"))
			SkyMaterials.push_back(pMaterial);

	}
	return pMaterial;
}