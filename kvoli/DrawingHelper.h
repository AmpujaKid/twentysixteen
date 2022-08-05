class Color {
public:
	Color(){}
	Color(byte r, byte g, byte b, byte a = 255) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	byte r, g, b, a;
	byte& operator[](int i) {
		return *(byte*)((DWORD)this + i);
	}
	bool operator ==(Color otherColor) {
		return (otherColor.r == r && otherColor.g == g && otherColor.b == b && otherColor.a == a);
	}
};

namespace Drawing {
	void DrawString(unsigned long&, int, int, int, int, int, int, const char*, ...);
	void GetTextSize(unsigned long&, int&, int&, const char*, ...);
	void DrawOutlinedRect(int, int, int, int, int, int, int, int = 255);
	void DrawRect(int, int, int, int, int, int, int, int = 255);
	void DrawLine(int, int, int, int, int, int, int, int = 255);
	void DrawGradientHorizontal(int, int, int, int, Color, Color);
	void DrawGradientVertical(int, int, int, int, Color, Color);
}

void Drawing::DrawGradientVertical(int x, int y, int w, int h, Color from, Color to) {
	int diff_r, diff_g, diff_b, diff_a;
	int val_r, val_g, val_b, val_a;
	Color color;

	diff_r = from.r - to.r;
	diff_g = from.g - to.g;
	diff_b = from.b - to.b;
	diff_a = from.a - to.a;

	if (from.r > to.r && diff_r < 0) diff_r = -diff_r;
	if (from.g > to.g && diff_g < 0) diff_g = -diff_g;
	if (from.b > to.b && diff_b < 0) diff_b = -diff_b;
	if (from.a > to.a && diff_a < 0) diff_a = -diff_a;

	val_r = diff_r / w;
	val_g = diff_g / w;
	val_b = diff_b / w;
	val_a = diff_a / w;

	for (int i = 0; i < w; i++)
	{
		color = Color(
			from.r - (val_r * i),
			from.g - (val_g * i),
			from.b - (val_b * i),
			from.a - (val_a * i));
		DrawLine(x + i, y, x + i, y + h, color.r, color.g, color.b, color.a);
	}
}

void Drawing::DrawGradientHorizontal(int x, int y, int w, int h, Color start, Color end) {
	float diff_r, diff_g, diff_b, diff_a;
	float val_r, val_g, val_b, val_a;
	Color color;

	diff_r = start.r - end.r;
	diff_g = start.g - end.g;
	diff_b = start.b - end.b;
	diff_a = start.a - end.a;

	if (start.r > end.r && diff_r < 0) diff_r = -diff_r;
	if (start.g > end.g && diff_g < 0) diff_g = -diff_g;
	if (start.b > end.b && diff_b < 0) diff_b = -diff_b;
	if (start.a > end.a && diff_a < 0) diff_a = -diff_a;

	val_r = diff_r / h;
	val_g = diff_g / h;
	val_b = diff_b / h;
	val_a = diff_a / h;

	for (int i = 0; i < h; i++)
	{
		color = Color(
			start.r - (val_r * i),
			start.g - (val_g * i),
			start.b - (val_b * i),
			start.a - (val_a * i));
		DrawLine(x, y + i, x + w, y + i, color.r, color.g, color.b, color.a);
	}
}

void Drawing::DrawOutlinedRect(int x, int y, int w, int h, int r, int g, int b, int a) {
	Interfaces->Surface->DrawSetColor(r, g, b, a);
	Interfaces->Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Drawing::DrawRect(int x, int y, int w, int h, int r, int g, int b, int a) {
	Interfaces->Surface->DrawSetColor(r, g, b, a);
	Interfaces->Surface->DrawFilledRect(x, y, x + w, y + h);
}

void Drawing::DrawLine(int x, int y, int x2, int y2, int r, int g, int b, int a) {
	Interfaces->Surface->DrawSetColor(r, g, b, a);
	Interfaces->Surface->DrawLine(x, y, x2, y2);
}

void Drawing::DrawString(unsigned long& Font, int x, int y, int r, int g, int b, int a, const char* strText, ...) {
	char buf[1024];
	wchar_t wbuf[1024];

	va_list vlist;
	va_start(vlist, strText);
	vsprintf(buf, strText, vlist);
	va_end(vlist);

	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	Interfaces->Surface->DrawSetTextColor(r, g, b, a);
	Interfaces->Surface->DrawSetTextPos(x, y);
	Interfaces->Surface->DrawSetTextFont(Font);
	Interfaces->Surface->DrawPrintText(wbuf, wcslen(wbuf), 0);
}

void Drawing::GetTextSize(unsigned long& Font, int& w, int& h, const char* strText, ...) {
	char buf[1024];
	wchar_t wbuf[1024];

	va_list vlist;
	va_start(vlist, strText);
	vsprintf(buf, strText, vlist);
	va_end(vlist);

	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);
	Interfaces->Surface->GetTextSize(Font, wbuf, w, h);
}