#include "IMaterial.h"

class KeyValues;

class IMaterialSystem {
public:
	VFUNCR2(CreateMaterial, 83, IMaterial*, const char*, KeyValues*);
	VFUNCR2(FindMaterial, 84, IMaterial*, const char*, const char*);
};