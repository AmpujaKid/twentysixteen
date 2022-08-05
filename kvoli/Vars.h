struct Vars {
	struct AntiAim {
		struct Normal{
			int Enabled = 1;
			int Yaw = 2;
			int Pitch = 1;
			int YawMode = 1;
			int TargetMode = 1;
		} Normal;
		struct Duck {
			int Enabled;
			int Yaw;
			int Pitch;
			int YawMode;
			int TargetMode = 2;
		} Duck;
		struct Wall {
			int Enabled = 1;
			int Pitch = 1;
			int YawMode = 2;
		} Wall;
	} AntiAim;
	struct Fakelag {
		int Enabled = 1;
		int Mode = 2;
		int Factor = 14;
	} Fakelag;
	struct Visuals {
		struct Player {
			struct Chams {
				int Enabled = 0;
				float Alpha = 1.f;
				int ViewModelChams = 0;
				int HandChams = 0;
				int Rainbow;
			} Chams;
			int Box = 1;
			int Name = 1;
			int Weapon = 1;
			int Health = 1;
			int EnemyOnly;
			int Aimspot;
			int Outline = 3;
		} Player;
		struct World {
			float ASUS = 1.f;
		} World;
	} Visuals;
	struct Aimbot {
		struct Accuracy {
			int LagFix = 1;
			int NoRecoil = 1;
			int Resolver = 1;
		} Accuracy;
		int Enabled = 1;
		int Legit = 1;
		int MultiSpot = 5;
		int MultiBox = 5;
		int Hitscan;
		int Bodyaim;
		int AWPBody = 1;
		int Silent = 2;
		int AutoShoot = 1;
		int Autowall = 1;
		//int NewAutoWall = 0;
		int TargetSelection = 0;
		int Aimspot = 0;
		float hitchance = 0;
		float PointScale = .49f;
	} Aimbot;
	struct Misc {
		int RoundSay;
		int Bunnyhop = 1;
	} Misc;
} Vars;