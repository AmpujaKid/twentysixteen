#include "studiohdr.h"

class model_t {
};

#include "DrawModelState_t.h"
#include "ModelRenderInfo_t.h"

enum OverrideType_t
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
};

class IVModelRender {
public:
	VFUNC3(ForcedMaterialOverride, 1, IMaterial*, OverrideType_t, int);
};