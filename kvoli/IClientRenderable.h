class IClientRenderable {
public:
	VFUNCR0(GetModel, 8, const model_t*);
	VFUNCR4(SetupBones, 13, bool, matrix3x4*, int, int, float);
	VFUNCR0(GetRenderAngles, 2, Vector&);
};