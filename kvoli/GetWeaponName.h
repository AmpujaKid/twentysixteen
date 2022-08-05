namespace UTIL {
	const char* GetWeaponName(int);
}

const char* UTIL::GetWeaponName(int WeaponID) {
	switch (WeaponID) {
		/*pistols*/
	case 4:
		return "glock";
	case 2:
		return "dual berretas";
	case 36:
		return "p250";
	case 30:
		return "tec9";
	case 1:
		return "deagle";
	case 32:
		return "p2000";
	case 3:
		return "fiveseven";
	case 64:
		return "revolver";
	case 63:
		return "cz75";
	case 61:
		return "usp";
		/*heavy*/
	case 35:
		return "nova";
	case 25:
		return "xm1014";
	case 29:
		return "sawed off";
	case 14:
		return "m249";
	case 28:
		return "negev";
	case 27:
		return "mag7";
		/*smgs*/
	case 17:
		return "mac10";
	case 33:
		return "mp7";
	case 24:
		return "ump45";
	case 19:
		return "p90";
	case 26:
		return "bizon";
	case 34:
		return "mp9";
		/*rifles*/
	case 10:
		return "famas";
	case 16:
		return "m4a1";
	case 40:
		return "scout";
	case 8:
		return "aug";
	case 9:
		return "awp";
	case 38:
		return "scar20";
	case 13:
		return "galil";
	case 7:
		return "ak47";
	case 39:
		return "sg553";
	case 11:
		return "g3sg1";
	case 60:
		return "m4a1-s";
		/*grenades*/
	case 46:
	case 48:
		return "molotov";
	case 44:
		return "grenade";
	case 43:
		return "flashbang";
	case 45:
		return "smoke";
	case 47:
		return "decoy";
		/*other*/
	case 31:
		return "zeus";
	default:
		return "knife";
	}
}