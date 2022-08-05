std::vector<IMaterial*> WorldMaterials;
std::vector<IMaterial*> SkyMaterials;

void ASUS() {
	static bool alreadyReset = true;
	static bool alreadyChanged;
	static float oldFloat = 0.f;

	if (!Interfaces->Engine->IsInGame() || !Interfaces->Engine->IsConnected()) {
		if (!alreadyReset) {
			WorldMaterials.clear();
			SkyMaterials.clear();
			alreadyReset = true;
			alreadyChanged = false;
		}
		return;
	}

	alreadyReset = false;

	if (oldFloat != Vars.Visuals.World.ASUS) {
		oldFloat = Vars.Visuals.World.ASUS;
		alreadyChanged = false;
	}

	if (!Interfaces->Engine->IsConnected() || !Interfaces->Engine->IsInGame()) return;

	if (alreadyChanged) return;

	for (IMaterial* pMaterial : WorldMaterials) {
		pMaterial->SetMaterialVarFlag(MATERIAL_VAR_FLAT, !(Vars.Visuals.World.ASUS == 1.f) ? true : false);
		pMaterial->SetMaterialVarFlag(MATERIAL_VAR_VERTEXALPHA, !(Vars.Visuals.World.ASUS == 1.f) ? true : false);
		pMaterial->AlphaModulate(Vars.Visuals.World.ASUS);
	}

	for (IMaterial* pMaterial : SkyMaterials) {
		if(Vars.Visuals.World.ASUS != 1.f)
			pMaterial->ColorModulate(0.f, 0.f, 0.f);
		else
			pMaterial->ColorModulate(1.f, 1.f, 1.f);
		pMaterial->SetMaterialVarFlag(MATERIAL_VAR_FLAT, !(Vars.Visuals.World.ASUS == 1.f) ? true : false);
	}

	alreadyChanged = true;
}