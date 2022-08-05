namespace HitBox {
	struct HitBox_s {
		int bone;
		int group;
		Vector bbmin;
		Vector bbmax;
	};

	HitBox_s* GetHitBox(int iBone);
	HitBox_s retHitBox;;
}

HitBox::HitBox_s* HitBox::GetHitBox(int iBone) {
	if (iBone == 1) { // neck
		retHitBox.bbmin = Vector(0.3317f, -3.0174f, -2.4503f);
		retHitBox.bbmax = Vector(3.4026f, 2.4182f, 2.354f);
		retHitBox.bone = 0;
	}
	else if(iBone == 2) { // Spine3
		retHitBox.bbmin = Vector(-4.28f, -5.029f, -6.0883f);
		retHitBox.bbmax = Vector(3.21f, 5.885f, 5.9813f);
		
		retHitBox.bone = 7;
	}
	else if (iBone == 3) { // Pelvis
		retHitBox.bbmin = Vector(-6.42f, -5.7459f, -6.8587f);
		retHitBox.bbmax = Vector(4.5796f, 4.5796f, 6.8373f);
		retHitBox.bone = 0;
	}
	else if (iBone == 4) { // Spine2
		retHitBox.bbmin = Vector(-4.28f, -4.5796f, -6.3879f);
		retHitBox.bbmax = Vector(3.21f, 5.885f, 6.2809f);
		retHitBox.bone = 4;
	}
	else if (iBone == 5) { // Spine1
		retHitBox.bbmin = Vector(-4.28f, -4.5796f, -6.3879f);
		retHitBox.bbmax = Vector(3.21f, 5.885f, 6.2809f);
		retHitBox.bone = 1;
	}
	else if (iBone == 6) { // L Thigh
		retHitBox.bbmin = Vector(1.819f, -3.959f, -2.14f);
		retHitBox.bbmax = Vector(22.149002f, 3.424f, 4.5796f);
		retHitBox.bone = 73;
	}
	else if (iBone == 7) { // R Thigh
		retHitBox.bbmin = Vector(1.819f, -3.7557f, -4.5796f);
		retHitBox.bbmax = Vector(22.149002f, 3.424f, 2.14f);
		retHitBox.bone = 67;
	}
	else if (iBone == 8) { // L Calf
		retHitBox.bbmin = Vector(2.0758f, -3.21f, -2.1507f);
		retHitBox.bbmax = Vector(19.26f, 2.675f, 3.0495f);
		retHitBox.bone = 70;
	}
	else if (iBone == 9) { // R Calf
		retHitBox.bbmin = Vector(2.0758f, -3.21f, -2.8462f);
		retHitBox.bbmax = Vector(19.26f, 2.675f, 2.247f);
		retHitBox.bone = 64;
	}
	else if (iBone == 10) { // L Upper Arm
		retHitBox.bbmin = Vector(-2.675f, -3.21f, -2.14f);
		retHitBox.bbmax = Vector(12.84f, 3.21f, 2.14f);
		retHitBox.bone = 61;
	}
	else if (iBone == 11) { // R Upper Arm
		retHitBox.bbmin = Vector(-2.675f, -3.21f, -2.14f);
		retHitBox.bbmax = Vector(12.84f, 3.21f, 2.14f);
		retHitBox.bone = 33;
	}
	else if (iBone == 12) { // L Arm 
		retHitBox.bbmin = Vector(-0.f, -2.14f, -2.14f);
		retHitBox.bbmax = Vector(9.63f, 2.14f, 2.14f);
		retHitBox.bone = 59;
	}
	else if (iBone == 13) { // R Arm
		retHitBox.bbmin = Vector(-0.f, -2.14f, -2.14f);
		retHitBox.bbmax = Vector(9.63f, 2.14f, 2.14f);
		retHitBox.bone = 31;
	}
	else if (iBone == 14) { // L Clavicle
		retHitBox.bbmin = Vector(-0.f, -3.21f, -5.35f);
		retHitBox.bbmax = Vector(7.49f, 4.28f, 3.21f);
		retHitBox.bone = 36;
	}
	else if (iBone == 15) { // R Clavicle
		retHitBox.bbmin = Vector(-0.f, -3.21f, -3.21f);
		retHitBox.bbmax = Vector(7.49f, 4.28f, 5.35f);
		retHitBox.bone = 8;
	}
	return &retHitBox;
}

//int HitScanBoxes[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19 }; 