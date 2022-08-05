#include "DrawingHelper.h"
#include "MaterialCreator.h"
#include "ColorFromHSB.h"

Color pColorT = Color(255, 54, 54);
Color pColorCT = Color(54, 61, 255);
Color pColorWeapons = Color(255, 250, 191);
Color pColorArms = Color(94, 54, 255);
Color pColorViewModel = Color(255, 81, 110);

static float pHueFloat[65];

void __fastcall hkDrawModelExecute(void* pThis, void*, void* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld) {
	static IMaterial* pMaterial, *pMaterialWireframe;
	if (!pMaterial) {
		pMaterial = UTIL::CreateMaterial(false, false);
		pMaterialWireframe = UTIL::CreateMaterial(false, false);

		for (int i = 1; i < 65; i++)
			pHueFloat[i] = (float)((float)rand() / (float)RAND_MAX);
	}
	
	for (int i = 1; i < 65; i++) {
		pHueFloat[i] += Interfaces->Globals->frametime / 100.f;
		while (pHueFloat[i] > 1.f)
			pHueFloat[i] -= 1.f;
	}

	if (Vars.Visuals.Player.Chams.Enabled && pInfo.pModel && pInfo.entity_index && pInfo.entity_index > 0 && pInfo.entity_index < 65 && strstr(Interfaces->ModelInfo->GetModelName(pInfo.pModel), "models/player")) {
		CBaseEntity* pEnt = Interfaces->EntList->GetClientEntity(pInfo.entity_index);
		if (pEnt) {
			Interfaces->ModelRender->ForcedMaterialOverride(pMaterial, OVERRIDE_NORMAL, 0);
			if (Vars.Visuals.Player.Chams.Rainbow) {
				Color pColorHSB = UTIL::GetFromHSB(pHueFloat[pInfo.entity_index], 1.f, 1.f);
				float flColor[3] = { (float)pColorHSB.r / 255.f, (float)pColorHSB.g / 255.f , (float)pColorHSB.b / 255.f };
				Interfaces->RenderView->SetColorModulation(flColor);
			}
			else if (pEnt->m_iTeamNum() == 2) {
				float flColor_T[3] = { (float)pColorT.r / 255.f, (float)pColorT.g / 255.f , (float)pColorT.b / 255.f };
				Interfaces->RenderView->SetColorModulation(flColor_T);
			}
			else{
				float flColor_CT[3] = { (float)pColorCT.r / 255.f, (float)pColorCT.g / 255.f , (float)pColorCT.b / 255.f };
				Interfaces->RenderView->SetColorModulation(flColor_CT);
			}

			Interfaces->RenderView->SetBlend(Vars.Visuals.Player.Chams.Alpha);
		}
	}

	CBaseEntity* localPly = Interfaces->EntList->GetClientEntity(Interfaces->Engine->GetLocalPlayer());

	if (Vars.Visuals.Player.Chams.Enabled && strstr(Interfaces->ModelInfo->GetModelName(pInfo.pModel), "models/weapons/w_")) {
		float flColor_Weapon[3] = { (float)pColorWeapons.r / 255.f, (float)pColorWeapons.g / 255.f , (float)pColorWeapons.b / 255.f };
		Interfaces->RenderView->SetColorModulation(flColor_Weapon);
		Interfaces->ModelRender->ForcedMaterialOverride(pMaterial, OVERRIDE_NORMAL, 0);
	}
	else if (Vars.Visuals.Player.Chams.HandChams && strstr(Interfaces->ModelInfo->GetModelName(pInfo.pModel), "arms") && localPly) {
		if (Vars.Visuals.Player.Chams.HandChams == 3) {
			return;
		}
		else {
			float flColor_Hands[3] = { (float)pColorArms.r / 255.f, (float)pColorArms.g / 255.f, (float)pColorArms.b / 255.f };
			Interfaces->RenderView->SetColorModulation(flColor_Hands);
			if (Vars.Visuals.Player.Chams.HandChams == 1)
				Interfaces->ModelRender->ForcedMaterialOverride(pMaterial, OVERRIDE_NORMAL, 0);
			else {
				pMaterialWireframe->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				Interfaces->ModelRender->ForcedMaterialOverride(pMaterialWireframe, OVERRIDE_NORMAL, 0);
			}
		}
	}
	else if (Vars.Visuals.Player.Chams.ViewModelChams && strstr(Interfaces->ModelInfo->GetModelName(pInfo.pModel), "models/weapons/v_")) {
		if (Vars.Visuals.Player.Chams.ViewModelChams == 3) {
			return;
		}
		else {
			float flColor_Weapon[3] = { (float)pColorViewModel.r / 255.f, (float)pColorViewModel.g / 255.f, (float)pColorViewModel.b / 255.f };
			Interfaces->RenderView->SetColorModulation(flColor_Weapon);

			if (Vars.Visuals.Player.Chams.ViewModelChams == 1) {
				Interfaces->ModelRender->ForcedMaterialOverride(pMaterial, OVERRIDE_NORMAL, 0);
			}
			else {
				pMaterialWireframe->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				Interfaces->ModelRender->ForcedMaterialOverride(pMaterialWireframe, OVERRIDE_NORMAL, 0);
			}
		}
	}

	ModelRenderVmt->hookTable(false);
	ModelRenderVmt->getOrig<void(__thiscall*)(void*, void*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4*)>(21)(pThis, ctx, state, pInfo, pCustomBoneToWorld);
	ModelRenderVmt->hookTable(true);

	Interfaces->ModelRender->ForcedMaterialOverride(0, OVERRIDE_NORMAL, 0);
}