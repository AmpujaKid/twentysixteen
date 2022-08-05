/* layout
1) vector
2) float
3) int
4) byte
5) vfuncs
6) functions
*/

class CBaseCombatWeapon {
public:
	/*vector*/
	/*float*/
	NETVAR(float, DT_WeaponCSBaseGun, m_fAccuracyPenalty);
	NETVAR(float, DT_WeaponCSBaseGun, m_flNextPrimaryAttack);
	NETVAR(float, DT_WeaponCSBaseGun, m_flNextSecondaryAttack);
	/*int*/
	NETVAR(int, DT_BaseCombatWeapon, m_iItemDefinitionIndex);
	NETVAR(int, DT_WeaponCSBaseGun, m_iClip1);
	/*byte*/
	/*vfuncs*/
	/*functions*/
	bool IsKnife() {
		switch (m_iItemDefinitionIndex()) {
			case 59:
			case 42:
			case 47:
			case 43:
			case 44:
			case 46:
			case 45:
			case 48:
				return true;
			default:
				return false;
		}
	}

	bool CanFire(CBaseEntity* localPly) {
		if (IsKnife()) return false;
		if (m_iClip1() < 1) return false;

		float flServerTime = (float)localPly->m_nTickBase() * Interfaces->Globals->interval_per_tick;
		
		if(m_iItemDefinitionIndex() != 64/*revolver*/)
			return (flServerTime >= m_flNextPrimaryAttack());
		else
			return (flServerTime >= m_flNextSecondaryAttack());
	}
	bool CanFire() {
		return CanFire(Interfaces->EntList->GetClientEntity(Interfaces->Engine->GetLocalPlayer()));
	}
};