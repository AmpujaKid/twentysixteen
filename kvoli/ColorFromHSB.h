namespace UTIL {
	Color GetFromHSB(float, float, float);
}

Color UTIL::GetFromHSB(float hue, float saturation, float brightness) {
	float h = hue == 1.0f ? 0 : hue * 6.0f;
	float f = h - (int)h;
	float p = brightness * (1.0f - saturation);
	float q = brightness * (1.0f - saturation * f);
	float t = brightness * (1.0f - (saturation * (1.0f - f)));
	byte r = 0, g = 0, b = 0;

	if (h < 1)
	{
		r = (brightness * 255);
		g = (t * 255);
		b = (p * 255);
	}
	else if (h < 2)
	{
		r = (q * 255);
		g = (brightness * 255);
		b = (p * 255);
	}
	else if (h < 3)
	{
		r = (p * 255);
		g = (brightness * 255);
		b = (t * 255);
	}
	else if (h < 4)
	{
		r = (p * 255);
		g = (q * 255);
		b = (brightness * 255);
	}
	else if (h < 5)
	{
		r = (t * 255);
		g = (p * 255);
		b = (brightness * 255);
	}
	else
	{
		r = (brightness * 255);
		g = (p * 255);
		b = (q * 255);
	}

	return Color(r, g, b);
}