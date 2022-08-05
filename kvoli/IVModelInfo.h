class IVModelInfo {
public:
	VFUNCR1(GetStudioModel, 30, studiohdr_t*, const model_t*);
	VFUNCR1(GetModelName, 3, const char*, const model_t*);
	VFUNC3(GetModelMaterials, 17, const model_t*, int, IMaterial**);
};