namespace UTIL {
	IMaterial* CreateMaterial(bool, bool);
}

IMaterial* UTIL::CreateMaterial(bool shouldIgnoreZ, bool isWireframe){
	static int created = 0;

	static const char tmp[] =
	{
		"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$envmap\" \"\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$receiveflashlight\" \"1\"\
		\n\t\"$singlepassflashlight\" \"1\"\
		\n\t\"$flat\" \"1\"\
		\n\t\"$nocull\" \"0\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
		\n\t\"$nofog\" \"0\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$wireframe\" \"%i\"\
		\n}\n"
	};

	static const char baseType[] = "VertexLitGeneric";

	char material[512];
	sprintf(material, tmp, baseType, (shouldIgnoreZ) ? 1 : 0, (isWireframe) ? 1 : 0);

	char name[512];
	sprintf(name, "custom_material_%i.vmt", created);
	++created;

	KeyValues *keyValues = new KeyValues(baseType);
	keyValues->LoadFromBuffer(name, material);
	IMaterial *createdMaterial = Interfaces->MaterialSystem->CreateMaterial(name, keyValues);
	createdMaterial->IncrementReferenceCount();
	return createdMaterial;
}
