struct ModelRenderInfo_t
{
	Vector origin;
	Vector angles;
	IClientRenderable *pRenderable;
	const model_t *pModel;
	const matrix3x4 *pModelToWorld;
	const matrix3x4 *pLightingOffset;
	const Vector *pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	unsigned short instance;

	ModelRenderInfo_t()
	{
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};