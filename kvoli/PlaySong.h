void __fastcall hkPlaySound(void* pThis, void*, const char* pSound) {
	SurfaceVmt->getOrig<void(__thiscall*)(void*, const char*)>(82)(pThis, pSound);
}

int __fastcall hkEmitAmbientSound(void* pThis, void*, const char* pSample, float flVolume, int iPitch, int flags, float soundtime) {
	return EngineSoundVmt->getOrig<int(__thiscall*)(void*, const char*, float, int, int, float)>(12)(pThis, pSample, flVolume, iPitch, flags, soundtime);
}