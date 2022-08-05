#undef PlaySong

struct Vector2D {
	Vector2D() {}
	Vector2D(float x, float y) { this->x = x; this->y = y; }
	float x;
	float y;
};

struct Vertex_t
{
	Vertex_t() {}
	Vertex_t(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}
	void Init(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}

	Vector2D	m_Position;
	Vector2D	m_TexCoord;
};


class ISurface {
public:
	VFUNC4(DrawSetColor, 15, int, int, int, int);
	VFUNC4(DrawFilledRect, 16, int, int, int, int);
	VFUNC4(DrawOutlinedRect, 18, int, int, int, int);
	VFUNC4(DrawLine, 19, int, int, int, int);
	VFUNC1(DrawSetTextFont, 23, unsigned long);
	VFUNC4(DrawSetTextColor, 25, int, int, int, int);
	VFUNC2(DrawSetTextPos, 26, int, int);
	VFUNC3(DrawPrintText, 28, const wchar_t*, int, int);
	VFUNC2(GetScreenSize, 44, int&, int&);
	VFUNCR0(CreateFont, 71, unsigned long);
	VFUNCR9(SetFontGlyphSet, 72, bool, unsigned long, const char*, int, int, int, int, int, int, int);
	VFUNCR1(AddCustomFontFile, 73, bool, const char*);
	VFUNC4(GetTextSize, 79, unsigned long, const wchar_t*, int&, int&);
	VFUNC2(GetCursorPos, 100, int&, int&);
	VFUNC3(DrawTexturedPolygon, 106, int, Vertex_t*, bool);
	VFUNCR1(CreateNewTextureID, 43, int, bool);
	VFUNC4(DrawSetTextureRGBA, 36, int, const unsigned char*, int, int);//30
	VFUNC1(DrawSetTexture, 38, int);
};