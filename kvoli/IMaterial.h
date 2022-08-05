class IMaterial {
public:
	VFUNCR0(GetName, 0, const char*);
	VFUNCR0(GetTextureGroupName, 1, const char*);
	VFUNC0(IncrementReferenceCount, 12);
	VFUNC1(AlphaModulate, 27, float);
	VFUNC3(ColorModulate, 28, float, float, float);
	VFUNC2(SetMaterialVarFlag, 29, MaterialVarFlags_t, bool);
	VFUNC1(GetMaterialVarFlag, 30, MaterialVarFlags_t);
	VFUNCR0(GetAlphaModulation, 44, float);
	VFUNC3(GetColorModulation, 45, float*, float*, float*);
};