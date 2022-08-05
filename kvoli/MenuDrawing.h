/* mostly just c&p of the drawing manager, using this so i can set alpha for all menu elements without having to rewrite the elements */

int fAlphaToUse = 0.f;

namespace MenuDrawing {
	void DrawString(unsigned long&, int, int, int, int, int, int, const char*, ...);
	void GetTextSize(unsigned long&, int&, int&, const char*, ...);
	void DrawOutlinedRect(int, int, int, int, int, int, int, int = 255);
	void DrawRect(int, int, int, int, int, int, int, int = 255);
	void DrawLine(int, int, int, int, int, int, int, int = 255);
}

void MenuDrawing::DrawOutlinedRect(int x, int y, int w, int h, int r, int g, int b, int a) {
	Interfaces->Surface->DrawSetColor(r, g, b, a - fAlphaToUse);
	Interfaces->Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void MenuDrawing::DrawRect(int x, int y, int w, int h, int r, int g, int b, int a) {
	Interfaces->Surface->DrawSetColor(r, g, b, a - fAlphaToUse);
	Interfaces->Surface->DrawFilledRect(x, y, x + w, y + h);
}

void MenuDrawing::DrawLine(int x, int y, int x2, int y2, int r, int g, int b, int a) {
	Interfaces->Surface->DrawSetColor(r, g, b, a - fAlphaToUse);
	Interfaces->Surface->DrawLine(x, y, x2, y2);
}

void MenuDrawing::DrawString(unsigned long& Font, int x, int y, int r, int g, int b, int a, const char* strText, ...) {
	char buf[1024];
	wchar_t wbuf[1024];

	va_list vlist;
	va_start(vlist, strText);
	vsprintf(buf, strText, vlist);
	va_end(vlist);

	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	Interfaces->Surface->DrawSetTextColor(r, g, b, a - fAlphaToUse);
	Interfaces->Surface->DrawSetTextPos(x, y);
	Interfaces->Surface->DrawSetTextFont(Font);
	Interfaces->Surface->DrawPrintText(wbuf, wcslen(wbuf), 0);
}

void MenuDrawing::GetTextSize(unsigned long& Font, int& w, int& h, const char* strText, ...) {
	char buf[1024];
	wchar_t wbuf[1024];

	va_list vlist;
	va_start(vlist, strText);
	vsprintf(buf, strText, vlist);
	va_end(vlist);

	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);
	Interfaces->Surface->GetTextSize(Font, wbuf, w, h);
}