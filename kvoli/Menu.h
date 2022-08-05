#include "MenuDrawing.h"

#undef __DEBUGMENU

#ifdef __DEBUGMENU
#define MSG_MENU(x, _x)\
char buf[64];\
sprintf(buf, "Deleted %s (0x%X)\n", x, _x);\
Msg(buf);
#else
#define MSG_MENU(x, _x)
#endif

namespace Menu {
	void Draw();
	bool InBox(int, int, int, int, bool = false);
	int ScrW, ScrH;
	int MenuX, MenuY;
	int MouseX, MouseY;
	bool MouseDown;
	bool _MouseDown, _MouseDownLastFrame;
	bool bIsDrawingDropdown;

	struct CBaseObject;
	struct CCheckbox;
	struct CButton;
	struct CSlider;
	struct CArea;
	struct CTab;
	struct CTabSelection;
	struct CSeperator;
	struct CDropdown;
	struct CMultiDropdown;
	struct CFrame;
	struct CLabel;
	struct CColorSelector;
	struct CColorPallete;
	struct CPlayerList;

	enum ObjectTypes {
		OBJECT_BASE,
		OBJECT_CHECKBOX,
		OBJECT_SLIDER,
		OBJECT_BUTTON,
		OBJECT_AREA,
		OBJECT_TAB,
		OBJECT_TABSELECTION,
		OBJECT_SEPERATOR,
		OBJECT_DROPDOWN,
		OBJECT_MULTIDROPDOWN,
		OBJECT_LABEL,
		OBJECT_COLORSELECTOR,
		OBJECT_COLORPALLETE,
		OBJECT_PLAYERLIST,
	};

	const char* obTranslate[] = { "CBaseObject", "CCheckbox", "CSlider", "CButton", "CArea", "CTab", "CTabSelection", "CSeperator", "CDropdown", "CMultiDropdown", "CLabel" };

	std::vector<CFrame*> vecFrames;
	std::vector<void*> pObjects;

	void AddFrame(CFrame* pFrame) { vecFrames.push_back(pFrame); pObjects.push_back(pFrame); }
	CFrame* CreateFrame(const char*, int, int, bool = false);

	void DeleteObjects();

	CCheckbox* CreateCheckbox(const char* pTitle, int x, int y, int w, int h, int spacer, int* pValue, int iLabelOffset = 0, const char* pTooltip = 0);
	CSlider* CreateSlider(const char* pTitle, int x, int y, int w, int h, int spacer, int iSliderType, void* pValue, int iLabelOffset = 0, bool bDrawValue = true, const char* pTooltip = 0);
	CSlider* CreateSliderInt(const char* pTitle, int x, int y, int w, int h, int spacer, void* pValue, int iMin, int iMax, int iLabelOffset = 0, bool bDrawValue = true, const char* pTooltip = 0);
	CSlider* CreateSliderFloat(const char* pTitle, int x, int y, int w, int h, int spacer, void* pValue, float flMin, float flMax, int iLabelOffset = 0, bool bDrawValue = true, const char* pTooltip = 0);
	CArea* CreateArea(const char* pTitle, int x, int y, int w, int h);
	CSeperator* CreateSeperator(const char* pTitle, int x, int y, int w);
	CTab* CreateTab(const char* pTitle, int x, int y, int w, int h, bool bDrawOutline = false, Color pColor = Color(50, 50, 50, 255), const char* pTooltip = 0);
	CTabSelection* CreateTabSelection(int x, int y, int w, int h);
	CMultiDropdown* CreateMultiDropdown(const char* pTitle, int x, int y, int w, int h, int spacer, int* pRequired, int* pValue, int iLabelOffset = 0, const char* pTooltip = 0);
	CDropdown* CreateDropdown(const char* pTitle, int x, int y, int w, int h, int spacer, int* pValue, int iLabelOffset = 0, const char* pTooltip = 0);
	CButton* CreateButton(const char* pTitle, int x, int y, int w, int h, int spacer, void* OnClick, const char* pTooltip = 0);
	CLabel* CreateLabel(const char* pTitle, int x, int y, int iAlignHorizontal, int iAlignVertical, unsigned long Font, Color pColor = Color(220, 220, 220));
	CColorSelector* CreateColorSelector(int x, int y, int w, int h, int type, Color* pColor, const char* pTooltip = 0);
	CColorPallete* CreateColorPallete(int x, int y, int w, int h, int iSquareAmountHorizonal, int iSquareAmountVertical, bool bDrawBrightnessSlider, int iBrightnessSliderOffset, Color* pColor, const char* pTooltip = 0, bool bDrawOutline = false);
	CPlayerList* CreatePlayerlist();

	unsigned long Font[12];
	enum Fonts {
		FONT_BAR,
		FONT_FRAMETITLE,
		FONT_CHECKBOX,
		FONT_AREA,
		FONT_SEPERATOR,
		FONT_TABSELECTION,
		FONT_DROPDOWN,
		FONT_LABEL,
		FONT_BUTTON,
		FONT_SLIDER,
		FONT_CLOCK,
		FONT_TOOLTIP,
	};

	void CreateFont(Fonts fIndex, const char* pFontName, int iSize, int iWeight, int iFlags) {
		Font[fIndex] = Interfaces->Surface->CreateFont();
		Interfaces->Surface->SetFontGlyphSet(Font[fIndex], pFontName, iSize, iWeight, 0, 0, iFlags, 0, 0);
	}

	void DrawMouse();

	int tw, th;

	Color MenuColor = Color(0, 0, 0);

	const char* pTooltipText;
	float flTooltipTime;
	bool bGotTooltipThisFrame;

	bool DrawTooltip(const char*);
}

bool Menu::DrawTooltip(const char* pTooltip) {
	if (!pTooltip) return false;
	bGotTooltipThisFrame = true;
	flTooltipTime += Interfaces->Globals->frametime;
	if (flTooltipTime < .8f) return false;
	pTooltipText = pTooltip;
	return true;
}

struct Menu::CBaseObject {
	CBaseObject() { obType = OBJECT_BASE; }
	int iX, iY;
	int iW, iH;
	int iSpacer;
	float* flValue;
	int* iValue;
	const char* pName;
	ObjectTypes obType = OBJECT_BASE;
	virtual void Draw() {};
	bool bShouldDraw = true;
	int iParentX, iParentY;
	bool bDrawTooltipAlways;
	int OldMouse[2];
	int iLabelTextOffset = 0;
};

struct Menu::CFrame{
	void Delete();
	std::vector<CBaseObject*> vecObjects;
	int PosX, PosY;
	bool bActive;
	const char* pTitle;
	void AddObject(CBaseObject* pObject) { vecObjects.push_back(pObject); pObjects.push_back(pObject); }
	void Draw();
	void Drag();
	bool bSavePos;
	int iSavedX;
	int iSavedY;
};

void Menu::DeleteObjects() { 
	static int i = 0;
	for (auto pFrame : pObjects) { delete pFrame; }

}

struct Menu::CLabel : CBaseObject {
	CLabel() { obType = OBJECT_LABEL; iTextAlign[0] = ALIGN_NONE; iTextAlign[1] = ALIGN_NONE; }
	virtual void Draw();
	int iTextAlign[2];
	unsigned long ulFont;
	enum TEXT_ALIGN {
		ALIGN_NONE,
		ALIGN_RIGHT,
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_UP,
		ALIGN_DOWN,
	};
	Color TextColor;
};

void Menu::CLabel::Draw() {
	if (!ulFont) ulFont = Font[FONT_LABEL];

	MenuDrawing::GetTextSize(ulFont, tw, th, this->pName);

	int x = iX, y = iY;

	if (iTextAlign[0] == ALIGN_RIGHT)
		x -= tw;
	else if (iTextAlign[0] == ALIGN_CENTER)
		x -= tw / 2;

	if (iTextAlign[1] == ALIGN_DOWN)
		y += th;
	else if (iTextAlign[1] ==  ALIGN_UP)
		y -= th;
	else if (iTextAlign[1] == ALIGN_CENTER)
		y -= th / 2;

	MenuDrawing::DrawString(ulFont, x, y, TextColor.r, TextColor.g, TextColor.b, TextColor.a, this->pName);
}

struct Menu::CCheckbox : CBaseObject {
	CCheckbox() { obType = OBJECT_CHECKBOX; }
	virtual void Draw();
	const char* pTooltip;
	void SetValue(int* iValue) { this->iValue = iValue; }
};

void Menu::CCheckbox::Draw() {
	MenuDrawing::GetTextSize(Font[FONT_CHECKBOX], tw, th, this->pName);
	MenuDrawing::DrawString(Font[FONT_CHECKBOX], iParentX + iX, iParentY + iY + iH / 2 - th / 2 + iLabelTextOffset, 220, 220, 220, 255, this->pName);

	MenuDrawing::DrawOutlinedRect(iParentX + iX + iSpacer, iParentY + iY, iW, iH, 0, 0, 0, 255);

	bool inBox = InBox(iParentX + iX + iSpacer, iParentY + iY, iW, iH);

	if (*(this->iValue)) {
		MenuDrawing::DrawRect(iParentX + iX + iSpacer + 1, iParentY + iY + 1, iW - 2, iH - 2, MenuColor.r, MenuColor.g, MenuColor.b, 255);
	}
	else if (inBox) {
		MenuDrawing::DrawRect(iParentX + iX + iSpacer + 1, iParentY + iY + 1, iW - 2, iH - 2, MenuColor.r, MenuColor.g, MenuColor.b, 150);
	}

	if (inBox && pTooltip && ((OldMouse[0] == MouseX && OldMouse[1] == MouseY) || bDrawTooltipAlways)) {
		if (DrawTooltip(pTooltip))
			bDrawTooltipAlways = true;
	}
	else
		bDrawTooltipAlways = false;

	OldMouse[0] = MouseX;
	OldMouse[1] = MouseY;

	if (inBox && MouseDown) {
		*(this->iValue) = !*(this->iValue);
	}
}

struct Menu::CButton : CBaseObject {
	CButton() { obType = OBJECT_BUTTON; }
	virtual void Draw();
	void* OnClick;
	const char* pTooltip;
};

void Menu::CButton::Draw() {
	bool inBox = InBox(iParentX + iX + iSpacer, iParentY + iY, iW, iH);

	if (inBox)
		MenuDrawing::DrawRect(iParentX + iX + iSpacer, iParentY + iY, iW, iH, 110, 110, 110, 255);
	else
		MenuDrawing::DrawRect(iParentX + iX + iSpacer, iParentY + iY, iW, iH, 90, 90, 90, 255);

	MenuDrawing::DrawOutlinedRect(iParentX + iX + iSpacer, iParentY + iY, iW, iH, 0, 0, 0, 255);

	MenuDrawing::GetTextSize(Font[FONT_BUTTON], tw, th, this->pName);
	MenuDrawing::DrawString(Font[FONT_BUTTON], iParentX + iX + iSpacer + iW / 2 - tw / 2, iParentY + iY + iH / 2 - th / 2 + iLabelTextOffset, 220, 220, 220, 255, this->pName);

	if(inBox && MouseDown && OnClick)
		((void(*)())(DWORD)OnClick)();

	if (inBox && pTooltip && ((OldMouse[0] == MouseX && OldMouse[1] == MouseY) || bDrawTooltipAlways)) {
		if (DrawTooltip(pTooltip))
			bDrawTooltipAlways = true;
	}
	else
		bDrawTooltipAlways = false;

	OldMouse[0] = MouseX;
	OldMouse[1] = MouseY;
}

struct Menu::CSlider : CBaseObject {
	enum SliderType {
		SLIDER_INT,
		SLIDER_FLOAT,
	};
	CSlider() { obType = OBJECT_SLIDER; iSliderType = SLIDER_INT; }
	CSlider(SliderType iSliderType) { CSlider(); this->iSliderType = iSliderType; }
	virtual void Draw();
	int iMin, iMax;
	float flMin, flMax;
	SliderType iSliderType;
	const char* pTooltip;
	bool bDrawValue;
};

void Menu::CSlider::Draw() {
	if (iSliderType == SLIDER_FLOAT) {
		if (*flValue > flMax)
			*flValue = flMax;
		else if (*flValue < flMin)
			*flValue = flMin;
	}
	else {
		if (*iValue > iMax)
			*iValue = iMax;
		else if (*iValue < iMin)
			*iValue = iMin;
	}

	int iCurSize = 0;

	if (iSliderType == SLIDER_FLOAT) {
		iCurSize = (float)(*flValue - (float)flMin) * (float)iW / (float)(flMax - flMin);
	}
	else {
		iCurSize = (float)(*iValue - iMin) * (float)iW / (float)(iMax - iMin);
	}

	bool inBox = InBox(iParentX + iX + iSpacer, iParentY + iY - 2, iW, iH + 4);

	MenuDrawing::DrawRect(iParentX + iX + iSpacer, iParentY + iY, iW, iH, 90, 90, 90, 255);
	MenuDrawing::DrawRect(iParentX + iX + iSpacer, iParentY + iY, iCurSize, iH, MenuColor.r, MenuColor.g, MenuColor.b, 255);
	MenuDrawing::DrawOutlinedRect(iParentX + iX + iSpacer, iParentY + iY, iW, iH, 0, 0, 0, 255);

	MenuDrawing::DrawRect(iParentX + iX + iSpacer + iCurSize - 2, iParentY + iY - 2, 4, iH + 4, MenuColor.r, MenuColor.g, MenuColor.b, 255);
	MenuDrawing::DrawOutlinedRect(iParentX + iX + iSpacer + iCurSize - 2, iParentY + iY - 2, 4, iH + 4, 0, 0, 0, 255);

	MenuDrawing::GetTextSize(Font[FONT_LABEL], tw, th, this->pName);
	MenuDrawing::DrawString(Font[FONT_LABEL], iParentX + iX, iParentY + iY + iH / 2 - th / 2 + iLabelTextOffset, 220, 220, 220, 255, this->pName);
	if (bDrawValue) {
		if (iSliderType == SLIDER_FLOAT) {
			char buf[12];
			sprintf(buf, "%.2f", *flValue);
			MenuDrawing::GetTextSize(Font[FONT_SLIDER], tw, th, buf);
			MenuDrawing::DrawString(Font[FONT_SLIDER], iParentX + iX + iSpacer + iW + 5, iParentY + iY + iH / 2 - th / 2, 220, 220, 220, 255, buf);
		}
		else {
			char buf[12];
			sprintf(buf, "%d", *iValue);
			MenuDrawing::GetTextSize(Font[FONT_SLIDER], tw, th, buf);
			MenuDrawing::DrawString(Font[FONT_SLIDER], iParentX + iX + iSpacer + iW + 5, iParentY + iY + iH / 2 - th / 2, 220, 220, 220, 255, buf);
		}
	}
	if (inBox && _MouseDown) {
		if (iSliderType == SLIDER_FLOAT) {
			*flValue = (float)(MouseX - iX - iParentX - iSpacer) * (float)(flMax - flMin) / (float)iW;
			*flValue += flMin;
		}
		else {
			*iValue = (MouseX - iX - iParentX - iSpacer) * (iMax - iMin) / iW;
			*iValue += iMin;
		}
	}

	if (inBox && pTooltip && ((OldMouse[0] == MouseX && OldMouse[1] == MouseY) || bDrawTooltipAlways)) {
		if (DrawTooltip(pTooltip))
			bDrawTooltipAlways = true;
	}
	else
		bDrawTooltipAlways = false;

	OldMouse[0] = MouseX;
	OldMouse[1] = MouseY;
}

struct Menu::CArea : CBaseObject {
	CArea() { obType = OBJECT_AREA; }
	virtual void Draw();
	std::vector<CBaseObject*> vecObjects;
	void AddObject(CBaseObject* pObject) { vecObjects.push_back(pObject); pObjects.push_back(pObject); }
};

void Menu::CArea::Draw() {
	MenuDrawing::DrawRect(iParentX + iX, iParentY + iY, iW, iH, 70, 70, 70, 255);
	MenuDrawing::DrawOutlinedRect(iParentX + iX, iParentY + iY, iW, iH, 0, 0, 0, 255);

	MenuDrawing::GetTextSize(Font[FONT_AREA], tw, th, this->pName);
	MenuDrawing::DrawString(Font[FONT_AREA], iParentX + iX + 10, iParentY + iY - th / 2 + iLabelTextOffset, 220, 220, 220, 255, this->pName);

	for (auto pElem : vecObjects) {
		pElem->iParentX = iParentX + iX;
		pElem->iParentY = iParentY + iY;
		if (pElem->bShouldDraw) pElem->Draw();
	}
}

struct Menu::CTab : CBaseObject {
	CTab() { obType = OBJECT_TAB; }
	void Delete() { for (auto pObject : vecObjects) { MSG_MENU(obTranslate[pObject->obType], pObject); delete pObject; } }
	std::vector<CBaseObject*> vecObjects;
	virtual void Draw();
	Color BackgroundColor = Color(50, 50, 50, 255);
	bool bDrawOutline;
	void AddObject(CBaseObject* pObject) { vecObjects.push_back(pObject); pObjects.push_back(pObject); }
	const char* pTooltip;
};

void Menu::CTab::Draw() {
	MenuDrawing::DrawRect(iParentX + iX, iParentY + iY, iW, iH, BackgroundColor.r, BackgroundColor.g, BackgroundColor.b, BackgroundColor.a);

	if (bDrawOutline)
		MenuDrawing::DrawOutlinedRect(iParentX + iX, iParentY + iY, iW, iH, 0, 0, 0, 255);

	for (auto pElem : vecObjects) {
		if (pElem->bShouldDraw) {
			pElem->iParentX = iParentX + iX;
			pElem->iParentY = iParentY + iY;
			pElem->Draw();
		}
	}
}

struct Menu::CTabSelection : CBaseObject {
	CTabSelection() { obType = OBJECT_TABSELECTION; }
	void Delete() { for (auto& pElem : vecTabs) { MSG_MENU(obTranslate[pElem.pTab->obType], pElem.pTab); delete pElem.pTab; } }
	struct CTabSelect {
		CTab* pTab;
		bool bDraw;
	};
	std::vector<CTabSelect> vecTabs;
	virtual void Draw();
	void AddTab(CTab* pTab, bool bShow = false) {
		vecTabs.push_back({ pTab, bShow });
	}
};

void Menu::CTabSelection::Draw() {
	static int iTexture = Interfaces->Surface->CreateNewTextureID(true);
	int iSizePer = floor((float)iW / (float)vecTabs.size());

	int iOffset = 0;
	
	bool DoSelect = false;
	DWORD bAddress = 0;

	bool bDrawOneTooltip = false;

 	for (auto& pTab : vecTabs) {
		bool inBox = InBox(iParentX + iX + iOffset, iParentY + iY, iSizePer, iH);
		Vertex_t pVertex[4];
		Interfaces->Surface->DrawSetTexture(iTexture);
		if(pTab.bDraw || inBox)
			Interfaces->Surface->DrawSetColor(120, 120, 120, 255 - fAlphaToUse);
		else
			Interfaces->Surface->DrawSetColor(90, 90, 90, 255 - fAlphaToUse);

		pVertex[0].Init(Vector2D(iParentX + iX + iOffset, iParentY + iY + iH), Vector2D(iParentX + iX + iOffset + 10, iParentY + iY));
		pVertex[1].Init(Vector2D(iParentX + iX + iOffset + 10, iParentY + iY), Vector2D(iParentX + iX + iOffset - 10 + iSizePer, iParentY + iY));
		pVertex[2].Init(Vector2D(iParentX + iX + iOffset - 10 + iSizePer, iParentY + iY), Vector2D(iParentX + iX + iOffset + iSizePer, iParentY + iY + iH));
		pVertex[3].Init(Vector2D(iParentX + iX + iOffset + iSizePer, iParentY + iY + iH), Vector2D(iParentX + iX + iOffset, iParentY + iY + iH));

		Interfaces->Surface->DrawTexturedPolygon(4, pVertex, true);

		MenuDrawing::GetTextSize(Font[FONT_TABSELECTION], tw, th, pTab.pTab->pName);
		MenuDrawing::DrawString(Font[FONT_TABSELECTION], iParentX + iX + iOffset + iSizePer / 2 - tw / 2, iParentY + iY + iH / 2 - th / 2, 220, 220, 220, 255, pTab.pTab->pName);
		
		if (pTab.bDraw) {
			pTab.pTab->iParentX = iParentX + iX;
			pTab.pTab->iParentY = iParentY + iY + iH;
			pTab.pTab->iW = iW;
			pTab.pTab->Draw();
		}

		if (inBox && MouseDown) {
			DoSelect = true;
			bAddress = (DWORD)(pTab.pTab);
		}

		if (inBox && pTab.pTab->pTooltip && ((OldMouse[0] == MouseX && OldMouse[1] == MouseY) || bDrawTooltipAlways)) {
			if (DrawTooltip(pTab.pTab->pTooltip))
				bDrawTooltipAlways = true;
			bDrawOneTooltip = true;
		}

		iOffset += iSizePer;
	}

	if (DoSelect && bAddress != NULL) {
		DoSelect = false;
		for (auto& pTab : vecTabs) {
			if((DWORD)(pTab.pTab) == bAddress)
				pTab.bDraw = true;
			else
				pTab.bDraw = false;
		}
	}

	if (!bDrawOneTooltip) bDrawTooltipAlways = false;

	OldMouse[0] = MouseX;
	OldMouse[1] = MouseY;
}

struct Menu::CSeperator : CBaseObject {
	CSeperator() { obType = OBJECT_SEPERATOR; }
	virtual void Draw();
};

void Menu::CSeperator::Draw() {
	MenuDrawing::GetTextSize(Font[FONT_SEPERATOR], tw, th, this->pName);
	MenuDrawing::DrawString(Font[FONT_SEPERATOR], iParentX + iX + 15, iParentY + iY - th / 2 + iLabelTextOffset, 220, 220, 220, 255, this->pName);
	MenuDrawing::DrawLine(iParentX + iX, iParentY + iY, iParentX + iX + 10, iParentY + iY, 80, 80, 80, 255);
	MenuDrawing::DrawLine(15 + iParentX + iX + tw + 5, iParentY + iY, iParentX + iX + iW, iParentY + iY, 80, 80, 80, 255);
}

struct Menu::CMultiDropdown : public CBaseObject {
	CMultiDropdown() { obType = OBJECT_MULTIDROPDOWN; }
	void Delete() { for (auto pElem : vecOptions) { MSG_MENU("DropdownOptionSegmnet", pElem); delete pElem; } }
	struct DropdownOptions {
		int iValRequired;
		std::vector<const char*> vecOptions;
		void AddOption(const char* pOption) { vecOptions.push_back(pOption); }
	};
	std::vector<DropdownOptions*> vecOptions;
	void AddOption(DropdownOptions* pOption) { vecOptions.push_back(pOption); pObjects.push_back(pOption); }
	void AddOption(std::vector<const char*> pOptions, int iIndex = -1) {
		int iUseIndex = iIndex;
		if (iUseIndex = -1) iUseIndex = iAddIndex;
		DropdownOptions* pNew = new DropdownOptions;
		pNew->iValRequired = iUseIndex;
		for (auto pChar : pOptions)
			pNew->vecOptions.push_back(pChar);

		AddOption(pNew);
		iAddIndex++;
	}
	int* pValueRequired;
	virtual void Draw();
	bool bDrawDropdown = false;
	const char* pTooltip;
	int iAddIndex = 0;
};

void Menu::CMultiDropdown::Draw() {
	MenuDrawing::GetTextSize(Font[FONT_LABEL], tw, th, this->pName);
	MenuDrawing::DrawString(Font[FONT_LABEL], iParentX + iX, iParentY + iY + iH / 2 - th / 2 + iLabelTextOffset, 220, 220, 220, 255, this->pName);
	
	bool inBox = InBox(iParentX + iX + iSpacer, iParentY + iY, iW, iH);

	if (inBox && pTooltip && ((OldMouse[0] == MouseX && OldMouse[1] == MouseY) || bDrawTooltipAlways)) {
		if (DrawTooltip(pTooltip))
			bDrawTooltipAlways = true;
	}
	else
		bDrawTooltipAlways = false;

	OldMouse[0] = MouseX;
	OldMouse[1] = MouseY;

	if (inBox || bDrawDropdown)
		MenuDrawing::DrawRect(iParentX + iX + iSpacer, iParentY + iY, iW, iH, 0, 0, 0, 150);

	MenuDrawing::DrawOutlinedRect(iParentX + iX + iSpacer, iParentY + iY, iW, iH, 0, 0, 0, 255);

	const char* pCurrent = 0;

	for (auto pElem : vecOptions) {
		if (pElem->iValRequired != *pValueRequired) continue;
		for (int i = 0; i < pElem->vecOptions.size(); i++) {
			if (i == *iValue) {
				pCurrent = pElem->vecOptions.at(i);
				break;
			}
		}
	}

	if (pCurrent) {
		MenuDrawing::GetTextSize(Font[FONT_DROPDOWN], tw, th, pCurrent);
		MenuDrawing::DrawString(Font[FONT_DROPDOWN], iParentX + iX + iSpacer + iW / 2 - tw / 2, iParentY + iY + iH / 2 - th / 2, 220, 220, 220, 255, pCurrent);
	}
	else {
		*iValue = 0;
	}

	if (pCurrent && ((inBox && MouseDown) || bDrawDropdown)) {
		bDrawDropdown = true;
		bIsDrawingDropdown = true;

		bool wasInBoxOnce = inBox;

		int iOutlineH;

		for (auto pElem : vecOptions) {
			if (pElem->iValRequired != *pValueRequired) continue;
			for (int i = 0; i < pElem->vecOptions.size(); i++) {
				MenuDrawing::DrawRect(iParentX + iX + iSpacer, iParentY + iY + iH * (i + 1), iW, iH, 55, 55, 55, 255);
				MenuDrawing::GetTextSize(Font[FONT_DROPDOWN], tw, th, pElem->vecOptions.at(i));
				MenuDrawing::DrawString(Font[FONT_DROPDOWN], iParentX + iX + iSpacer + iW / 2 - tw / 2, iParentY + iY + iH / 2 + iH * (i + 1) - th / 2, 220, 220, 220, 255, pElem->vecOptions.at(i));
				bool bInBox = InBox(iParentX + iX + iSpacer, iParentY + iY + iH * (i + 1), iW, iH, true);
				if (bInBox) {
					wasInBoxOnce = inBox;
					MenuDrawing::DrawOutlinedRect(iParentX + iX + iSpacer, iParentY + iY + iH * (i + 1) - ((i == 0) ? 1 : 0), iW, iH, 0, 0, 0, 255);
					if (MouseDown) {
						*iValue = i;
						bDrawDropdown = false;
						bIsDrawingDropdown = false;
					}
				}

				iOutlineH = iH * (i + 2);
			}

			MenuDrawing::DrawOutlinedRect(iParentX + iX + iSpacer, iParentY + iY, iW, iOutlineH, 0, 0, 0, 255);

			if (!wasInBoxOnce && MouseDown) {
				bDrawDropdown = false;
				bIsDrawingDropdown = false;
			}
		}
	}
}

struct Menu::CDropdown : public CBaseObject {
	std::vector<const char*> vecOptions;
	void AddOption(const char* pOption) { vecOptions.push_back(pOption); }
	void AddOption(std::vector<const char*> pOptions) { for (auto pChar : pOptions) AddOption(pChar); }
	virtual void Draw();
	bool bDrawDropdown = false;
	const char* pTooltip;
};

void Menu::CDropdown::Draw() {
	MenuDrawing::GetTextSize(Font[FONT_LABEL], tw, th, this->pName);
	MenuDrawing::DrawString(Font[FONT_LABEL], iParentX + iX, iParentY + iY + iH / 2 - th / 2 + iLabelTextOffset, 220, 220, 220, 255, this->pName);

	bool inBox = InBox(iParentX + iX + iSpacer, iParentY + iY, iW, iH);

	if (inBox && pTooltip && ((OldMouse[0] == MouseX && OldMouse[1] == MouseY) || bDrawTooltipAlways)) {
		if (DrawTooltip(pTooltip))
			bDrawTooltipAlways = true;
	}
	else
		bDrawTooltipAlways = false;

	OldMouse[0] = MouseX;
	OldMouse[1] = MouseY;

	if (inBox || bDrawDropdown)
		MenuDrawing::DrawRect(iParentX + iX + iSpacer, iParentY + iY, iW, iH, 0, 0, 0, 150);

	MenuDrawing::DrawOutlinedRect(iParentX + iX + iSpacer, iParentY + iY, iW, iH, 0, 0, 0, 255);

	const char* pCurrent = 0;

	for (int i = 0; i < vecOptions.size(); i++) {
		if (i != *iValue) continue;
		pCurrent = vecOptions.at(i);
		break;
	}

	if (pCurrent) {
		MenuDrawing::GetTextSize(Font[FONT_DROPDOWN], tw, th, pCurrent);
		MenuDrawing::DrawString(Font[FONT_DROPDOWN], iParentX + iX + iSpacer + iW / 2 - tw / 2, iParentY + iY + iH / 2 - th / 2, 220, 220, 220, 255, pCurrent);
	}
	else
		*iValue = 0;

	if (pCurrent && ((inBox && MouseDown) || bDrawDropdown)) {
		bDrawDropdown = true;
		bIsDrawingDropdown = true;

		bool wasInBoxOnce = inBox;

		for (int i = 0; i < vecOptions.size(); i++) {
			MenuDrawing::DrawRect(iParentX + iX + iSpacer, iParentY + iY + iH * (i + 1), iW, iH, 55, 55, 55, 255);
			MenuDrawing::GetTextSize(Font[FONT_DROPDOWN], tw, th, vecOptions.at(i));
			MenuDrawing::DrawString(Font[FONT_DROPDOWN], iParentX + iX + iSpacer + iW / 2 - tw / 2, iParentY + iY + iH / 2 + iH * (i + 1) - th / 2, 220, 220, 220, 255, vecOptions.at(i));
			bool bInBox = InBox(iParentX + iX + iSpacer, iParentY + iY + iH * (i + 1), iW, iH, true);
			if (bInBox) {
				wasInBoxOnce = true;
				MenuDrawing::DrawOutlinedRect(iParentX + iX + iSpacer, iParentY + iY + iH * (i + 1) - ((i == 0) ? 1 : 0), iW, iH, 0, 0, 0, 255);
				if (MouseDown) {
					*iValue = i;
					bDrawDropdown = false;
					bIsDrawingDropdown = false;
				}
			}
		}

		MenuDrawing::DrawOutlinedRect(iParentX + iX + iSpacer, iParentY + iY, iW, iH * (vecOptions.size() + 1), 0, 0, 0, 255);

		if (!wasInBoxOnce && MouseDown) {
			bDrawDropdown = false;
			bIsDrawingDropdown = false;
		}
	}
}

struct Menu::CColorSelector : CBaseObject {
	CColorSelector() { obType = OBJECT_COLORSELECTOR; clType = COLORSELECTOR_HORIZONTAL; }
	std::vector<Color> vecColors;
	int iSelectedColor;

	Color GetColor(){
		for (int i = 0; i < vecColors.size(); i++) {
			if (i != iSelectedColor) continue;
			return vecColors.at(i);
		}
	}

	void AddColor(Color pColor) { vecColors.push_back(pColor); }
	virtual void Draw();

	enum COLORSELECTOR_TYPE {
		COLORSELECTOR_HORIZONTAL,
		COLORSELECTOR_VERTICAL,
	};

	COLORSELECTOR_TYPE clType;

	Color* pColor;
	const char* pTooltip;
};

void Menu::CColorSelector::Draw() {
	for (int i = 0; i < vecColors.size(); i++) {
		Color& pCol = vecColors.at(i);
		if (clType == COLORSELECTOR_HORIZONTAL) {
			MenuDrawing::DrawRect(iParentX + iX + i * iW, iParentY + iY, iW, iH, pCol.r, pCol.g, pCol.b, pCol.a);
			MenuDrawing::DrawOutlinedRect(iParentX + iX + i * iW, iParentY + iY, iW, iH, 0, 0, 0, 255);
			bool inBox = InBox(iParentX + iX + i * iW, iParentY + iY, iW, iH);
		}
		else {
			MenuDrawing::DrawRect(iParentX + iX, iParentY + iY + iH * i, iW, iH, pCol.r, pCol.g, pCol.b, pCol.a);
			MenuDrawing::DrawOutlinedRect(iParentX + iX, iParentY + iY + iH * i, iW, iH, 0, 0, 0, 255);
			bool inBox = InBox(iParentX + iX, iParentY + iY + iH * i, iW, iH);
			if (inBox && MouseDown) {
				memcpy(pColor, &pCol, sizeof(Color));
			}
		}
	}

	if (clType == COLORSELECTOR_HORIZONTAL) {
		bool inBox = InBox(iParentX + iX, iParentY + iY, iW * vecColors.size(), iH);

		if (inBox && pTooltip && ((OldMouse[0] == MouseX && OldMouse[1] == MouseY) || bDrawTooltipAlways)) {
			if (DrawTooltip(pTooltip))
				bDrawTooltipAlways = true;
		}
		else
			bDrawTooltipAlways = false;
	}
	else if (clType == COLORSELECTOR_VERTICAL) {
		
		bool inBox = InBox(iParentX + iX, iParentY + iY, iW, iH * vecColors.size());

		if (inBox && pTooltip && ((OldMouse[0] == MouseX && OldMouse[1] == MouseY) || bDrawTooltipAlways)) {
			if (DrawTooltip(pTooltip))
				bDrawTooltipAlways = true;
		}
		else
			bDrawTooltipAlways = false;
	}

	OldMouse[0] = MouseX;
	OldMouse[1] = MouseY;
}

struct Menu::CColorPallete : CBaseObject {
	CColorPallete() { obType = OBJECT_COLORPALLETE; }
	Color* pColor;
	virtual void Draw();
	int iSquareW, iSquareH;
	int iSquareAmount[2];
	float flBrightness = 1.f;
	float flOldBrightness = 1.f;
	Color curHSVCol;
	float fSelectedSaturation = 0.f;
	float fSelectedHue = 0.f;
	bool DrawBrightnessSlider;
	const char* pTooltip;
	int iBrightnessSliderOffset;
	bool ___bDrawOutline;
};

void Menu::CColorPallete::Draw() {
	iSquareW = iW / iSquareAmount[0];
	iSquareH = iH / iSquareAmount[1];

	float fAddHue = (1.f / (float)iW) * (float)iSquareW;
	float fHue = 0.f;

	bool bDrawOutline = false;
	int x = 0, y = 0;

	float fRemoveSaturation = (1.f / (float)iH) * (float)iSquareH;
	fRemoveSaturation *= 0.75f;

	bool bColorSelected = false;
	bool bNoMoreSet = false;

	int iLargest = 0;
	int iTallest = 0;

	for (int i = 0; i <= iW; i += iSquareW) {
		iLargest = i + iSquareW;
		Color HSBColor = UTIL::GetFromHSB(fHue, 1.f, 1.f);
		MenuDrawing::DrawRect(iParentX + iX + i, iParentY + iY, iSquareW, iSquareH, HSBColor.r, HSBColor.g, HSBColor.b, 255);

		if (HSBColor == curHSVCol) {
			x = iParentX + iX + i - 1;
			y = iParentY + iY - 1;
			if (!bNoMoreSet) {
				fSelectedHue = fHue;
				fSelectedSaturation = 1.f;
			}
			bDrawOutline = true;
		}

		bool inBox = InBox(iParentX + iX + i, iParentY + iY, iSquareW, iSquareH);
		if (inBox && MouseDown) {
			fSelectedHue = fHue;
			fSelectedSaturation = 1.f;
			bColorSelected = true;
			bNoMoreSet = true;
		}

		float fSaturation = 1.f - fRemoveSaturation;

		for (int _i = 1; _i <= iH; _i += iSquareH) {
			fSaturation -= fRemoveSaturation;
			HSBColor = UTIL::GetFromHSB(fHue, fSaturation, 1.f);
			if (HSBColor == curHSVCol) {
				x = iParentX + iX + i - 1;
				y = iParentY + iY + _i - 1;
				bDrawOutline = true;
				if (!bNoMoreSet) {
					fSelectedHue = fHue;
					fSelectedSaturation = fSaturation;
				}
			}
			iTallest = _i + iSquareH;
			MenuDrawing::DrawRect(iParentX + iX + i, iParentY + iY + _i, iSquareW, iSquareH, HSBColor.r, HSBColor.g, HSBColor.b, 255);

			bool inBox = InBox(iParentX + iX + i, iParentY + iY + _i, iSquareW, iSquareH);
			if (inBox && _MouseDown) {
				//Msg("%d\t%d\t%d\n", curHSVCol.r, curHSVCol.g, curHSVCol.b);
				fSelectedHue = fHue;
				fSelectedSaturation = fSaturation;
				bColorSelected = true;
				bNoMoreSet = true;
			}
		}

		fHue += fAddHue;
	}

	if (___bDrawOutline)
		MenuDrawing::DrawOutlinedRect(iParentX + iX, iParentY + iY, iLargest, iTallest, 0, 0, 0, 255);


	float flDecreaseBrightness = 1.f / (float)iH;
	flDecreaseBrightness *= .8f;
	float flCurBrightness = 1.f;
	
	if (DrawBrightnessSlider) {
		bool _bDrawOutline = false;
		int _x = 0;
		int _y = 0;
		for (int i = 0; i <= iH; i++) {
			Color pCurCol = UTIL::GetFromHSB(fSelectedHue, fSelectedSaturation, flCurBrightness);
			MenuDrawing::DrawLine(iParentX + iX + iW + 10 - iBrightnessSliderOffset, iParentY + iY + i, iParentX + iX + iW + 15 - iBrightnessSliderOffset, iParentY + iY + i, pCurCol.r, pCurCol.g, pCurCol.b, 255);
			bool inBox = InBox(iParentX + iX + iW + 8 - iBrightnessSliderOffset, iParentY + iY + i, 9, 0);
			if (inBox && _MouseDown)
				flBrightness = flCurBrightness;

			if (flBrightness == flCurBrightness) {
				_bDrawOutline = true;
				_x = iParentX + iX + iW + 8 - iBrightnessSliderOffset;
				_y = iParentY + iY + i - 1;
			}
			flCurBrightness -= flDecreaseBrightness;
		}

		if (_bDrawOutline)
			MenuDrawing::DrawRect(_x, _y, 9, 3, 255, 255, 255, 255);
	}

	if (bDrawOutline) {
		MenuDrawing::DrawOutlinedRect(x, y, iSquareW + 2, iSquareH + 2, 0, 0, 0);
	}

	Color pCol = UTIL::GetFromHSB(fSelectedHue, fSelectedSaturation, 1.f);
	memcpy(&curHSVCol, &pCol, sizeof(Color));
	pCol = UTIL::GetFromHSB(fSelectedHue, fSelectedSaturation, flBrightness);
	memcpy(pColor, &pCol, sizeof(Color));

	if (flBrightness != flOldBrightness) {
		Color pCol = UTIL::GetFromHSB(fSelectedHue, fSelectedSaturation, flBrightness);
		memcpy(pColor, &pCol, sizeof(Color));
	}

	bool inBox = InBox(iParentX + iX, iParentY + iY, iW + 15, iH);
	if (inBox && pTooltip && ((OldMouse[0] == MouseX && OldMouse[1] == MouseY) || bDrawTooltipAlways)) {
		if (DrawTooltip(pTooltip))
			bDrawTooltipAlways = true;
	}
	else
		bDrawTooltipAlways = false;

	OldMouse[0] = MouseX;
	OldMouse[1] = MouseY;

	flOldBrightness = flBrightness;
}

struct Menu::CPlayerList : CBaseObject {
	virtual void Draw();
	bool bAdded;
	void SetPos(CBaseObject* pObject) { pObject->iParentX = iParentX; pObject->iParentY = iParentY; }
	int iSelectedPlayer;
};

void Menu::CPlayerList::Draw() {
	static CArea* pPlayerArea = CreateArea("Players", 5, 30, 140, 305);
	if (!bAdded) {
		bAdded = true;
		pObjects.push_back(pPlayerArea);
	}
	SetPos(pPlayerArea);
	pPlayerArea->Draw();
	int iTotalPlayers = 0;
	bool bFoundPlayer = false;
	for (int i = 1; i < 65; i++) {
		if (i == Interfaces->Engine->GetLocalPlayer()) continue;
		CBaseEntity* pEnt = Interfaces->EntList->GetClientEntity(i);
		if (!pEnt) continue;
		player_info_s pInfo;
		if (!Interfaces->Engine->GetPlayerInfo(i, &pInfo)) continue;

		bool inBox = InBox(iParentX + 5, iParentY + 30 + iTotalPlayers * 15 + 10, 140, 15);
		if (inBox || iSelectedPlayer == i)
			MenuDrawing::DrawRect(iParentX + 5, iParentY + 30 + iTotalPlayers * 15 + 10, 140, 15, 110, 110, 110, 255);
		else
			MenuDrawing::DrawRect(iParentX + 5, iParentY + 30 + iTotalPlayers * 15 + 10, 140, 15, 90, 90, 90, 255);
		MenuDrawing::GetTextSize(Font[FONT_BUTTON], tw, th, pInfo.name);
		MenuDrawing::DrawString(Font[FONT_BUTTON], iParentX + 5 + 140 / 2 - tw / 2, +10 + iParentY + 30 + iTotalPlayers * 15 + 15 / 2 - th / 2, 220, 220, 220, 255, pInfo.name);
		MenuDrawing::DrawLine(iParentX + 5, iParentY + 30 + iTotalPlayers * 15 + 10, iParentX + 5 + 140, iParentY + 30 + iTotalPlayers * 15 + 10, 0, 0, 0, 255);
		//MenuDrawing::DrawOutlinedRect(iParentX + 5, iParentY + 30 + iTotalPlayers * 15, 140, 15, 0, 0, 0, 255);

		if (inBox && MouseDown)
			iSelectedPlayer = i;

		if (iSelectedPlayer == i) bFoundPlayer = true;

		iTotalPlayers++;
	}

	if(iTotalPlayers) MenuDrawing::DrawLine(iParentX + 5, iParentY + 30 + iTotalPlayers * 15 + 10, iParentX + 5 + 140, iParentY + 30 + iTotalPlayers * 15 + 10, 0, 0, 0, 255);
	
	if (iTotalPlayers) {
		MenuDrawing::DrawOutlinedRect(iParentX + 5, iParentY + 30 + 10, 140, 305 - 10, 0, 0, 0, 255);
	}

	if (!bFoundPlayer) { iSelectedPlayer = 0; return; }

	static CCheckbox* pFriendCheckbox = CreateCheckbox("Friend", 140 + 10, 30, 12, 12, 85, 0, 0, "Whitelist Player");
	static CCheckbox* pBodyaimCheckbox = CreateCheckbox("Bodyaim", 140 + 10, 30 + 14, 12, 12, 85, 0, 0, "Toggle Bodyaim for this Player");
	static CCheckbox* pResolverCheckbox = CreateCheckbox("Resolver", 140 + 10, 30 + 14 * 2, 12, 12, 85, 0, 0, "Toggle Resolver for this Player");
	static CCheckbox* pAngelCheckbox = CreateCheckbox("Angel Correction", 140 + 10, 30 + 14 * 3, 12, 12, 85, 0, -1, "Toggle Angel Correction for this Player");
	static CDropdown* pPitchDropdown = CreateDropdown("Pitch", 140 + 10, 30 + 14 * 3 + 10 + 10 + 10 + 5, 50, 15, 85 - 40, 0, 0, "Set Pitch Correction for this Player");
	static CDropdown* pYawDropdown = CreateDropdown("Yaw", 140 + 10, 30 + 14 * 3 + 10 + 10 + 10 + 5 + 20, 50, 15, 85 - 40, 0, 0, "Set Yaw Correction for this Player");
	static CSeperator* pNormalSeperator = CreateSeperator("Normal", 140 + 10, 30 + 14 * 3 + 10 + 10 + 5, 95);
	static CSeperator* pAngelSeperator = CreateSeperator("Angel", 140 + 10, 147 + 5, 95);
	static CDropdown* pAngelPitchDropdown = CreateDropdown("Pitch", 140 + 10, 147 + 10 + 5, 50, 15, 85 - 40, 0, 0, "Set Angel Pitch Correction for this Player");
	static CDropdown* pAngelYawDropdown = CreateDropdown("Yaw", 140 + 10, 147 + 10 + 20 + 5, 50, 15, 85 - 40, 0, 0, "Set Angel Yaw Correction for this Player");
	static CSeperator* pResolverSeperator = CreateSeperator("Resolver", 140 + 10, 147 + 10 + 20 + 5 + 5 + 10 + 10, 95);
	static CDropdown* pResolverModeDropdown = CreateDropdown("Mode", 140 + 10, 147 + 10 + 20 + 5 + 5 + 10 + 10 + 10, 50, 15, 85 - 40, 0, 0, "Set Resolver Mode for this Player");
	static bool bAddOptionsOnce;
	if (!bAddOptionsOnce) {
		pPitchDropdown->AddOption(std::vector<const char*>{"Auto", "Down", "Up", "Inverse", "Twitch"});
		pYawDropdown->AddOption(std::vector<const char*>{"Auto", "Half Left", "Half Right", "Inverse", "Twitch", "Twitch 2"});
		pAngelPitchDropdown->AddOption(std::vector<const char*>{"Auto", "Twitch", "Down", "Up"});
		pAngelYawDropdown->AddOption(std::vector<const char*>{"Auto", "Half Left", "Half Right", "Twitch", "Twitch 2"});
		pResolverModeDropdown->AddOption(std::vector<const char*>{"Full Spin", "45 Step", "Jitter"});
		bAddOptionsOnce = true;
	}

	SetPos(pFriendCheckbox);
	SetPos(pBodyaimCheckbox);
	SetPos(pResolverCheckbox);
	SetPos(pAngelCheckbox);
	SetPos(pPitchDropdown);
	SetPos(pNormalSeperator);
	SetPos(pYawDropdown);
	SetPos(pAngelSeperator);
	SetPos(pAngelPitchDropdown);
	SetPos(pAngelYawDropdown);
	SetPos(pResolverSeperator);
	SetPos(pResolverModeDropdown);
	pFriendCheckbox->SetValue(&CorrectionsPlayers[iSelectedPlayer].Friend);
	pBodyaimCheckbox->SetValue(&CorrectionsPlayers[iSelectedPlayer].Bodyaim);
	pResolverCheckbox->SetValue(&CorrectionsPlayers[iSelectedPlayer].Resolver);
	pAngelCheckbox->SetValue(&CorrectionsPlayers[iSelectedPlayer].AngelAutoCorrection);
	pPitchDropdown->iValue = &CorrectionsPlayers[iSelectedPlayer].MethodX;
	pYawDropdown->iValue = &CorrectionsPlayers[iSelectedPlayer].MethodY;
	pAngelPitchDropdown->iValue = &CorrectionsPlayers[iSelectedPlayer].AngelX;
	pAngelYawDropdown->iValue = &CorrectionsPlayers[iSelectedPlayer].AngelY;
	pResolverModeDropdown->iValue = &Resolver::ResolverPlayers[iSelectedPlayer].ResolverMethod;

	pFriendCheckbox->Draw();
	pBodyaimCheckbox->Draw();
	pResolverCheckbox->Draw();
	pAngelCheckbox->Draw();
	pNormalSeperator->Draw();
	pAngelSeperator->Draw();
	pResolverSeperator->Draw();
	pResolverModeDropdown->Draw();
	pAngelYawDropdown->Draw();
	pAngelPitchDropdown->Draw();
	pYawDropdown->Draw();
	pPitchDropdown->Draw();

	static bool bAddedBox;

	if (!bAddedBox) {
		pObjects.push_back(pFriendCheckbox);
		pObjects.push_back(pBodyaimCheckbox);
		pObjects.push_back(pResolverCheckbox);
		pObjects.push_back(pAngelCheckbox);
		pObjects.push_back(pPitchDropdown);
		pObjects.push_back(pNormalSeperator);
		pObjects.push_back(pYawDropdown);
		pObjects.push_back(pAngelSeperator);
		pObjects.push_back(pAngelPitchDropdown);
		pObjects.push_back(pAngelYawDropdown);
		pObjects.push_back(pResolverSeperator);
		pObjects.push_back(pResolverModeDropdown);
		bAddedBox = true;
	}
}

bool Menu::InBox(int x, int y, int w, int h, bool bIsDropdown) {
	if (bIsDrawingDropdown && !bIsDropdown) return false;
	return (MouseX >= MenuX + x && MouseX <= MenuX + x + w && MouseY >= MenuY + y && MouseY <= MenuY + y + h);
}

Menu::CColorPallete* Menu::CreateColorPallete(int x, int y, int w, int h, int iSquareAmountHorizontal, int iSquareAmountVertical, bool bDrawBrightnessSlider, int iSubtractBrightnessSlider, Color* pColor, const char* pTooltip, bool bDrawOutline) {
	CColorPallete* new_ColorPallete = new CColorPallete;
	new_ColorPallete->pColor = pColor;
	new_ColorPallete->iSquareAmount[0] = iSquareAmountHorizontal;
	new_ColorPallete->iSquareAmount[1] = iSquareAmountVertical;
	new_ColorPallete->iW = w;
	new_ColorPallete->iH = h;
	new_ColorPallete->iY = y;
	new_ColorPallete->iX = x;
	new_ColorPallete->pTooltip = pTooltip;
	new_ColorPallete->DrawBrightnessSlider = bDrawBrightnessSlider;
	new_ColorPallete->iBrightnessSliderOffset = iSubtractBrightnessSlider;
	new_ColorPallete->___bDrawOutline = bDrawOutline;
	
	memcpy(&new_ColorPallete->curHSVCol, pColor, sizeof(Color));

	return new_ColorPallete;
}

Menu::CColorSelector* Menu::CreateColorSelector(int x, int y, int w, int h, int type, Color* pColor, const char* pTooltip) {
	CColorSelector* new_ColorSelector = new CColorSelector;
	new_ColorSelector->pColor = pColor;
	new_ColorSelector->iX = x;
	new_ColorSelector->iY = y;
	new_ColorSelector->iW = w;
	new_ColorSelector->iH = h;
	new_ColorSelector->clType = (CColorSelector::COLORSELECTOR_TYPE)type;
	new_ColorSelector->pTooltip = pTooltip;

	return new_ColorSelector;
}


Menu::CCheckbox* Menu::CreateCheckbox(const char* pTitle, int x, int y, int w, int h, int spacer, int* pValue, int iLabelOffset, const char* pTooltip) {
	CCheckbox* new_Checkbox = new CCheckbox();
	new_Checkbox->pName = pTitle;
	new_Checkbox->iX = x;
	new_Checkbox->iY = y;
	new_Checkbox->iW = w;
	new_Checkbox->iH = h;
	new_Checkbox->iSpacer = spacer;
	new_Checkbox->iValue = pValue;
	new_Checkbox->pTooltip = pTooltip;
	new_Checkbox->iLabelTextOffset = iLabelOffset;

	return new_Checkbox;
}

Menu::CSlider* Menu::CreateSlider(const char* pTitle, int x, int y, int w, int h, int spacer, int iSliderType, void* pValue, int iLabelOffset, bool bDrawValue, const char* pTooltip){
	CSlider* new_Slider = new CSlider((CSlider::SliderType)iSliderType);
	new_Slider->pName = pTitle;
	new_Slider->iX = x;
	new_Slider->iY = y;
	new_Slider->iW = w;
	new_Slider->iH = h;
	new_Slider->iSpacer = spacer;
	new_Slider->iValue = (int*)pValue;
	new_Slider->flValue = (float*)pValue;
	new_Slider->pTooltip = pTooltip;
	new_Slider->bDrawValue = bDrawValue;
	new_Slider->iLabelTextOffset = iLabelOffset;

	return new_Slider;
}

Menu::CSlider* Menu::CreateSliderInt(const char* pTitle, int x, int y, int w, int h, int spacer, void* pValue, int iMin, int iMax, int iLabelOffset, bool bDrawValue, const char* pTooltip) {
	CSlider* new_Slider = CreateSlider(pTitle, x, y, w, h, spacer, CSlider::SLIDER_INT, pValue, iLabelOffset, bDrawValue, pTooltip);
	new_Slider->iMin = iMin;
	new_Slider->iMax = iMax;

	return new_Slider;
}

Menu::CSlider* Menu::CreateSliderFloat(const char* pTitle, int x, int y, int w, int h, int spacer, void* pValue, float flMin, float flMax, int iLabelOffset, bool bDrawValue, const char* pTooltip) {
	CSlider* new_Slider = CreateSlider(pTitle, x, y, w, h, spacer, CSlider::SLIDER_FLOAT, pValue, iLabelOffset, bDrawValue, pTooltip);
	new_Slider->flMin = flMin;
	new_Slider->flMax = flMax;

	return new_Slider;
}

Menu::CFrame* Menu::CreateFrame(const char* pName, int x, int y, bool bActive) {
	CFrame* new_Frame = new CFrame;
	new_Frame->bActive = bActive;
	new_Frame->pTitle = pName;
	new_Frame->PosX = x;
	new_Frame->PosY = y;

	return new_Frame;
}

Menu::CArea* Menu::CreateArea(const char* pTitle, int x, int y, int w, int h) {
	CArea* new_Area = new CArea;
	new_Area->pName = pTitle;
	new_Area->iX = x;
	new_Area->iY = y;
	new_Area->iW = w;
	new_Area->iH = h;

	return new_Area;
}

Menu::CSeperator* Menu::CreateSeperator(const char* pTitle, int x, int y, int w) {
	CSeperator* new_Seperator = new CSeperator;
	new_Seperator->pName = pTitle;
	new_Seperator->iX = x;
	new_Seperator->iY = y;
	new_Seperator->iW = w;

	return new_Seperator;
}

Menu::CTab* Menu::CreateTab(const char* pTitle, int x, int y, int w, int h, bool bDrawOutline, Color pColor, const char* pTooltip) {
	CTab* new_Tab = new CTab;
	new_Tab->BackgroundColor.r = pColor.r;
	new_Tab->BackgroundColor.g = pColor.g;
	new_Tab->BackgroundColor.b = pColor.b;
	new_Tab->BackgroundColor.a = pColor.a;
	
	new_Tab->pName = pTitle;
	new_Tab->iX = x;
	new_Tab->iY = y;
	new_Tab->iW = w;
	new_Tab->iH = h;
	new_Tab->bDrawOutline = bDrawOutline;
	new_Tab->pTooltip = pTooltip;

	return new_Tab;
}

Menu::CTabSelection* Menu::CreateTabSelection(int x, int y, int w, int h) {
	CTabSelection* new_TabSelection = new CTabSelection;

	new_TabSelection->iX = x;
	new_TabSelection->iY = y;
	new_TabSelection->iW = w;
	new_TabSelection->iH = h;

	return new_TabSelection;
}

Menu::CMultiDropdown* Menu::CreateMultiDropdown(const char* pTitle, int x, int y, int w, int h, int spacer, int* pRequired, int* pValue, int iLabelOffset, const char* pTooltip) {
	CMultiDropdown* new_Dropdown = new CMultiDropdown;
	new_Dropdown->pName = pTitle;
	new_Dropdown->iX = x;
	new_Dropdown->iY = y;
	new_Dropdown->iW = w;
	new_Dropdown->iH = h;
	new_Dropdown->iSpacer = spacer;
	new_Dropdown->pValueRequired = pRequired;
	new_Dropdown->iValue = pValue;
	new_Dropdown->pTooltip = pTooltip;
	new_Dropdown->iLabelTextOffset = iLabelOffset;

	return new_Dropdown;
}

namespace Menu {
	CMultiDropdown::DropdownOptions* CreateDropdownOptionSegment(int iRequired, std::vector<const char*> vecChar);
}

Menu::CMultiDropdown::DropdownOptions* Menu::CreateDropdownOptionSegment(int iRequired, std::vector<const char*> vecChar) {
	CMultiDropdown::DropdownOptions* new_OptionSegment = new CMultiDropdown::DropdownOptions;
	new_OptionSegment->iValRequired = iRequired;
	for (auto pElem : vecChar)
		new_OptionSegment->AddOption(pElem);

	return new_OptionSegment;
}

Menu::CDropdown* Menu::CreateDropdown(const char* pTitle, int x, int y, int w, int h, int spacer, int* pValue, int iLabelOffset, const char* pTooltip) {
	CDropdown* new_Dropdown = new CDropdown;
	new_Dropdown->iValue = pValue;
	new_Dropdown->pName = pTitle;
	new_Dropdown->iX = x;
	new_Dropdown->iY = y;
	new_Dropdown->iW = w;
	new_Dropdown->iH = h;
	new_Dropdown->iSpacer = spacer;
	new_Dropdown->pTooltip = pTooltip;
	new_Dropdown->iLabelTextOffset = iLabelOffset;

	return new_Dropdown;
}

Menu::CButton* Menu::CreateButton(const char* pTitle, int x, int y, int w, int h, int spacer, void* OnClick, const char* pTooltip) {
	CButton* new_Button = new CButton;
	new_Button->pName = pTitle;
	new_Button->iX = x;
	new_Button->iY = y;
	new_Button->iW = w;
	new_Button->iH = h;
	new_Button->iSpacer = spacer;
	new_Button->OnClick = OnClick;
	new_Button->pTooltip = pTooltip;

	return new_Button;
}

Menu::CLabel* Menu::CreateLabel(const char* pTitle, int x, int y, int iAlignHorizontal, int iAlignVertical, unsigned long Font, Color pColor) {
	CLabel* new_Label = new CLabel;
	new_Label->pName = pTitle;
	new_Label->iX = x;
	new_Label->iY = y;
	new_Label->iTextAlign[0] = iAlignHorizontal;
	new_Label->iTextAlign[1] = iAlignVertical;
	new_Label->ulFont = Font;
	new_Label->TextColor.r = pColor.r;
	new_Label->TextColor.g = pColor.g;
	new_Label->TextColor.b = pColor.b;
	new_Label->TextColor.a = pColor.a;

	return new_Label;
}

Menu::CPlayerList* Menu::CreatePlayerlist() {
	return new CPlayerList;
}

void Menu::CFrame::Drag() {
	if (!_MouseDown || !InBox(PosX, PosY, 250, 20)) { bSavePos = false; return; }

	if (!bSavePos) {
		iSavedX = MouseX - PosX;
		iSavedY = MouseY - PosY;
		bSavePos = true;
	}

	PosX = MouseX;
	PosY = MouseY;
	PosX -= iSavedX;
	PosY -= iSavedY;
}

void Menu::CFrame::Draw() {
	MenuDrawing::DrawRect(PosX, PosY, 250, 350, 50, 50, 50, 255);
	MenuDrawing::DrawRect(PosX, PosY, 250, 20, MenuColor.r, MenuColor.g, MenuColor.b, 255);
	MenuDrawing::DrawOutlinedRect(PosX, PosY, 250, 350, 0, 0, 0, 255);
	MenuDrawing::DrawOutlinedRect(PosX, PosY, 250, 20, 0, 0, 0, 255);

	MenuDrawing::GetTextSize(Font[FONT_FRAMETITLE], tw, th, this->pTitle);
	MenuDrawing::DrawString(Font[FONT_FRAMETITLE], PosX + 250 / 2 - tw / 2, PosY + 10 - th / 2, 220, 220, 220, 255, this->pTitle);

	for (auto pObject : vecObjects) {
		pObject->iParentX = PosX;
		pObject->iParentY = PosY;
		if(pObject->bShouldDraw)
			pObject->Draw();
	}

	Drag();
}

void Menu::CFrame::Delete() {
	for (auto pObject : vecObjects) {
		MSG_MENU(obTranslate[pObject->obType], pObject);
		if (pObject->obType == OBJECT_TAB) ((CTab*)pObject)->Delete();
		else if (pObject->obType == OBJECT_TABSELECTION) ((CTabSelection*)pObject)->Delete();
		else if (pObject->obType == OBJECT_MULTIDROPDOWN) ((CMultiDropdown*)pObject)->Delete();
		delete pObject;
	}
}

void TestClick() {
	Msg("ONCLICK\n");
}

void Menu::Draw() {
	
	if (GetAsyncKeyState(VK_INSERT) & 0x1) {
		ShouldDraw = !ShouldDraw;
		if (!ShouldDraw)
			Interfaces->Engine->ExecuteClientCmd("cl_mouseenable 1");
	}

//	if (Stepmania::bDrawOverlay) { Interfaces->Engine->ExecuteClientCmd("cl_mouseenable 1"); ShouldDraw = false; }

	static bool FirstFrame;

	if (!ShouldDraw) { FirstFrame = true; }

	_MouseDown = GetAsyncKeyState(VK_LBUTTON);
	if (FirstFrame) _MouseDown = false;
	MouseDown = (_MouseDown && !_MouseDownLastFrame) ? true : false;

	FirstFrame = false;

	Interfaces->Surface->GetScreenSize(ScrW, ScrH);
	Interfaces->Surface->GetCursorPos(MouseX, MouseY);
	if (ShouldDraw) {
		Interfaces->Engine->ExecuteClientCmd("cl_mouseenable 0");
	}

	static float flHue = 0.f;
	flHue += Interfaces->Globals->frametime / 10.f;
	while (flHue > 1.f) flHue -= 1.f;

	static bool Init;

	if (!Init) {
		Init = true;

		/*------------------------------------------------------------------------------------------------------------------------*/
		CFrame* pRagebot = CreateFrame("Ragebot", 5, 50);
		pRagebot->AddObject(CreateSeperator("Targetting & Accuracy ", 5, 126, 240)); // +15 4 area
		CArea* pAimbotSection = CreateArea("Aimbot", 5, 30, 240, 69 + 17);
		pAimbotSection->AddObject(CreateCheckbox("Enabled", 5, 10, 12, 12, 150, &Vars.Aimbot.Enabled, 0, "Toggle Aimbot"));
		pAimbotSection->AddObject(CreateCheckbox("Autofire", 5, 24, 12, 12, 150, &Vars.Aimbot.AutoShoot, 0, "Toggle Autofire"));
		CDropdown* pAimbotSilentDropdown = CreateDropdown("Silent Aim", 5, 38 + 14 * 2, 70, 15, 150, &Vars.Aimbot.Silent, -1, "Change Silent Aim Options");
		pAimbotSilentDropdown->AddOption(std::vector<const char*>{"Disabled", "Silent", "pSilent"});
		pAimbotSection->AddObject(CreateCheckbox("Autowall", 5, 38, 12, 12, 150, &Vars.Aimbot.Autowall, 0, "Toggle Autowall"));
		CArea* pTargetSection = CreateArea("Targetting", 5, 126 + 15, 240, 100); // 110 w, 135 offset
		CDropdown* pTargetSelectionDropdown = CreateDropdown("Selection", 5, 10, 70, 15, 150, &Vars.Aimbot.TargetSelection, 0, "Change Aimbot Targetselection");
		pTargetSelectionDropdown->AddOption(std::vector<const char*>{"Nextshot", "Health", "Distance"});
		CDropdown* pMultiboxDropdown = CreateDropdown("Multibox", 5, 28, 70, 15, 150, &Vars.Aimbot.MultiBox, 0, "Change Multibox Settings");
		pMultiboxDropdown->AddOption(std::vector<const char*>{"Disabled", "Very Low", "Low", "Medium", "High", "Very High"});
		CDropdown* pMultispotDropdown = CreateDropdown("Multispot", 5, 46, 70, 15, 150, &Vars.Aimbot.MultiSpot, 0, "Change Multispot Settings");
		pMultispotDropdown->AddOption(std::vector<const char*>{"Disabled", "Very Low", "Low", "Medium", "High", "Very High"});
		CDropdown* pAimspotDropdown = CreateDropdown("Aimspot", 5, 46 + 18, 70, 15, 150, &Vars.Aimbot.Aimspot, 0, "Change Aimspot");
		pAimspotDropdown->AddOption(std::vector<const char*>{"Head", "Neck", "Chest", "Stomach", "Pelvis"});
		pTargetSection->AddObject(CreateSliderInt("Hitchance", 5, 46 + 36, 70, 15, 150, &Vars.Aimbot.hitchance, 0, 100, 0, true, "Change Pointscale"));

		pTargetSection->AddObject(CreateSliderFloat("Pointscale", 5, 46 + 36 + 18, 70, 3, 150, &Vars.Aimbot.PointScale, 0.f, 1.f, 0, true, "Change Pointscale"));
		pTargetSection->AddObject(pAimspotDropdown);
		pTargetSection->AddObject(pMultispotDropdown);
		pTargetSection->AddObject(pMultiboxDropdown);
		pTargetSection->AddObject(pTargetSelectionDropdown);
		pAimbotSection->AddObject(pAimbotSilentDropdown);
		pRagebot->AddObject(pAimbotSection);

		CArea* pAccuracySection = CreateArea("Accuracy", 5, 235 + 18, 240, 60);
		pAccuracySection->AddObject(CreateCheckbox("No Recoil", 5, 10, 12, 12, 150, &Vars.Aimbot.Accuracy.NoRecoil, 0, "Toggle No Recoil"));
		CDropdown* pLagfixDropdown = CreateDropdown("Fakelag Fix", 5, 24, 70, 15, 150, &Vars.Aimbot.Accuracy.LagFix, 1, "Change LagFix Options");
		CDropdown* pResolverDropdown = CreateDropdown("Resolver", 5, 41, 70, 15, 150, &Vars.Aimbot.Accuracy.Resolver, 0, "Choose Resolver Options");
		pLagfixDropdown->AddOption(std::vector<const char*>{"Disabled", "Normal", "Wait"});
		pResolverDropdown->AddOption(std::vector<const char*>{"Disabled", "Full Spin", "45 Step", "Jitter"});
		pAccuracySection->AddObject(pResolverDropdown);
		pAccuracySection->AddObject(pLagfixDropdown);
		pRagebot->AddObject(pAccuracySection);
		pRagebot->AddObject(pTargetSection);
		/*------------------------------------------------------------------------------------------------------------------------*/
		CFrame* pHackvsHack = CreateFrame("Hack vs Hack", 265, 50);
		pHackvsHack->AddObject(CreateSeperator("Anti Aim", 5, 30, 240));
		CTabSelection* pAntiAimTabSelection = CreateTabSelection(5, 45, 240, 15);
		/*/*/
		CTab* pNormalAntiAimTab = CreateTab("Normal", 0, 0, 0, 100, true, Color(70, 70, 70), "Change regular AntiAim Options");
		pNormalAntiAimTab->AddObject(CreateCheckbox("Enabled", 5, 10, 12, 12, 150, &Vars.AntiAim.Normal.Enabled, 0, "Toggle AntiAim"));
		CDropdown* pNormalTargetDropdown = CreateDropdown("At Target Mode", 5, 24, 80, 15, 150, &Vars.AntiAim.Normal.TargetMode, 0, "Change Target Mode");
		pNormalTargetDropdown->AddOption(std::vector<const char*>{"Static", "Following", "At Target"});
		CDropdown* pNormalPitchDropdown = CreateDropdown("Pitch", 5, 24 + 17, 80, 15, 150, &Vars.AntiAim.Normal.Pitch, 0, "Change AntiAim Pitch");
		pNormalPitchDropdown->AddOption(std::vector<const char*>{"Lag Down", "Lag Up", "Fakedown", "Normal", "Normal Inverted", "Angel Down", "Angel Up"});
		CDropdown* pNormalYawDropdown = CreateDropdown("Yaw", 5, 24 + 17 + 17, 80, 15, 150, &Vars.AntiAim.Normal.Yaw, 0, "Change AntiAim Yaw");
		pNormalYawDropdown->AddOption(std::vector<const char*>{"Sideways", "Half Sideways", "Backwards", "Jitter", "Spin", "Angel" });
		CMultiDropdown* pNormalYawModeDropdown = CreateMultiDropdown("Yaw Mode", 5, 24 + 17 * 3, 80, 15, 150, &Vars.AntiAim.Normal.Yaw, &Vars.AntiAim.Normal.YawMode, 0 , "Change AntiAim Yaw Mode");
		for (int i = 0; i < 2; i++)
			pNormalYawModeDropdown->AddOption(std::vector<const char*>{"Fake Side 1", "Fake Side 2", "Normal Side 1", "Normal Side 2"}, i);
		pNormalYawModeDropdown->AddOption(std::vector<const char*>{"Fake Side 1", "Fake Side 2", "Normal"}, 2);
		pNormalYawModeDropdown->AddOption(std::vector<const char*>{"Normal", "Synced", "M3nly", "Sp3cial", "R-Sp3cial"}, 3);
		pNormalYawModeDropdown->AddOption(std::vector<const char*>{"Fast", "Fast Fake", "Slow", "Slow Fake"}, 4);
		pNormalYawModeDropdown->AddOption(std::vector<const char*>{"Normal", "Inverted", "Fast Spin", "Fast Fake Spin", "Slow Spin", "Slow Fake Spin", "Jitter"}, 5);

		pNormalAntiAimTab->AddObject(pNormalYawModeDropdown);
		pNormalAntiAimTab->AddObject(pNormalYawDropdown);
		pNormalAntiAimTab->AddObject(pNormalPitchDropdown);
		pNormalAntiAimTab->AddObject(pNormalTargetDropdown);
		pAntiAimTabSelection->AddTab(pNormalAntiAimTab, true);
		/*/*/
		CTab* pDuckAntiAimTab = CreateTab("Duck", 0, 0, 0, 100, true, Color(70, 70, 70), "Change Duck AntiAim Options");
		pDuckAntiAimTab->AddObject(CreateCheckbox("Enabled", 5, 10, 12, 12, 150, &Vars.AntiAim.Duck.Enabled, 0, "Toggle AntiAim"));
		CDropdown* pDuckTargetDropdown = CreateDropdown("At Target Mode", 5, 24, 80, 15, 150, &Vars.AntiAim.Duck.TargetMode, 0, "Change Target Mode");
		pDuckTargetDropdown->AddOption(std::vector<const char*>{"Static", "Following", "At Target"});
		CDropdown* pDuckPitchDropdown = CreateDropdown("Pitch", 5, 24 + 17, 80, 15, 150, &Vars.AntiAim.Duck.Pitch, 0, "Change AntiAim Pitch");
		pDuckPitchDropdown->AddOption(std::vector<const char*>{"Lag Down", "Lag Up", "Fakedown", "Normal", "Normal Inverted", "Up"});
		CDropdown* pDuckYawDropdown = CreateDropdown("Yaw", 5, 24 + 17 + 17, 80, 15, 150, &Vars.AntiAim.Duck.Yaw, 0, "Change AntiAim Yaw");
		pDuckYawDropdown->AddOption(std::vector<const char*>{"Sideways", "Half Sideways", "Backwards", "Jitter"});
		CMultiDropdown* pDuckYawModeDropdown = CreateMultiDropdown("Yaw Mode", 5, 24 + 17 * 3, 80, 15, 150, &Vars.AntiAim.Duck.Yaw, &Vars.AntiAim.Duck.YawMode, 0, "Change AntiAim Yaw Mode");
		for (int i = 0; i < 2; i++)
			pDuckYawModeDropdown->AddOption(std::vector<const char*>{"Fake Side 1", "Fake Side 2", "Normal Side 1", "Normal Side 2"}, i);
		pDuckYawModeDropdown->AddOption(std::vector<const char*>{"Fake Side 1", "Fake Side 2", "Normal"}, 2);
		pDuckYawModeDropdown->AddOption(std::vector<const char*>{"Normal", "Synced", "M3nly", "Sp3cial", "R-Sp3cial"}, 3);

		pDuckAntiAimTab->AddObject(pDuckYawModeDropdown);
		pDuckAntiAimTab->AddObject(pDuckYawDropdown);
		pDuckAntiAimTab->AddObject(pDuckPitchDropdown);
		pDuckAntiAimTab->AddObject(pDuckTargetDropdown);
		pAntiAimTabSelection->AddTab(pDuckAntiAimTab);
		/*/*/
		CTab* pWallAntiAimTab = CreateTab("Wall", 0, 0, 0, 100, true, Color(70, 70, 70), "Change Wall AntiAim Options");
		pWallAntiAimTab->AddObject(CreateCheckbox("Enabled", 5, 10, 12, 12, 150, &Vars.AntiAim.Wall.Enabled, 0, "Toggle Wall Detection"));
		 
		CDropdown* pWallPitchDropdown = CreateDropdown("Pitch", 5, 24, 80, 15, 150, &Vars.AntiAim.Wall.Pitch, 0, "Change Wall Detection Pitch");
		pWallPitchDropdown->AddOption(std::vector<const char*>{"Normal", "180 Down", "Lisp"});
		CDropdown* pWallYawDropdown = CreateDropdown("Yaw", 5, 24 + 17, 80, 15, 150, &Vars.AntiAim.Wall.YawMode, 0, "Change Wall Detection Yaw");
		pWallYawDropdown->AddOption(std::vector<const char*>{"Normal", "Fake", "Fake Out", "Jitter", "Jitter Synced", "Lisp"});

		pWallAntiAimTab->AddObject(pWallYawDropdown);
		pWallAntiAimTab->AddObject(pWallPitchDropdown);

		pAntiAimTabSelection->AddTab(pWallAntiAimTab);
		/*/*/
		CArea* pFakelagArea = CreateArea("Fakelag", 5, 30 + 15 + 100 + 10 + 15, 240, 55);
		pFakelagArea->AddObject(CreateCheckbox("Enabled", 5, 10, 12, 12, 150, &Vars.Fakelag.Enabled, 0, "Toggle Fakelag"));
		pFakelagArea->AddObject(CreateSliderInt("Factor", 5, 27, 64, 3, 150, &Vars.Fakelag.Factor, 0, 14, 1, true, "Select Fakelag Factor"));
		CDropdown* pFakelagModeDropdown = CreateDropdown("Mode", 5, 27 + 8, 80, 15, 150, &Vars.Fakelag.Mode, 0, "Select Fakelag Mode");
		pFakelagModeDropdown->AddOption(std::vector<const char*>{"Normal", "Switch", "Adaptive", "Pulse"});
		pFakelagArea->AddObject(pFakelagModeDropdown);
		/*/*/
		pHackvsHack->AddObject(pFakelagArea);
		pHackvsHack->AddObject(pAntiAimTabSelection);
		/*------------------------------------------------------------------------------------------------------------------------*/
		CFrame* pVisuals = CreateFrame("Visuals", 265 + 260, 50);
		pVisuals->AddObject(CreateSeperator("Player", 5, 30, 240));
		CArea* pESPSection = CreateArea("ESP", 5, 45, 240, 98 + 15 + 3);
		pESPSection->AddObject(CreateCheckbox("Enemy Team Only", 5, 10, 12, 12, 150, &Vars.Visuals.Player.EnemyOnly, 0, "Enable ESP for Enemy only"));
		pESPSection->AddObject(CreateCheckbox("Box", 5, 24, 12, 12, 150, &Vars.Visuals.Player.Box, 0, "Toggle Box ESP"));
		pESPSection->AddObject(CreateCheckbox("Name", 5, 38, 12, 12, 150, &Vars.Visuals.Player.Name, 0, "Toggle Name ESP"));
		pESPSection->AddObject(CreateCheckbox("Weapon", 5, 52, 12, 12, 150, &Vars.Visuals.Player.Weapon, 0, "Toggle Weapon ESP"));
		pESPSection->AddObject(CreateCheckbox("Aimspot", 5, 66, 12, 12, 150, &Vars.Visuals.Player.Aimspot, 0, "Toggle Aimspot ESP"));
		CDropdown* pHealthDropdown = CreateDropdown("Health", 5, 80, 80, 15, 150, &Vars.Visuals.Player.Health, 0, "Choose Health Mode");
		pHealthDropdown->AddOption(std::vector<const char*>{"Disabled", "Bar", "Text", "Text + Bar"});
		CDropdown* pOutlineDropdown = CreateDropdown("Outline", 5, 80 + 17, 80, 15, 150, &Vars.Visuals.Player.Outline, 0, "Choose Outline Mode");
		pOutlineDropdown->AddOption(std::vector<const char*>{"Disabled", "In", "Out", "Full"});
		pESPSection->AddObject(pOutlineDropdown);
		pESPSection->AddObject(pHealthDropdown);
		CArea* pChamsSection = CreateArea("Chams", 5, 45 + 98 + 10 + 15 + 3, 240, 70 + 15 + 5);
		pChamsSection->AddObject(CreateCheckbox("Player Chams", 5, 10, 12, 12, 150, &Vars.Visuals.Player.Chams.Enabled, 0, "Toggle Player Chams"));
		CDropdown* pChamModeDropdown = CreateDropdown("Chams Mode", 5, 24, 80, 15, 150, &Vars.Visuals.Player.Chams.Rainbow, -1, "Select Chams Mode");
		pChamModeDropdown->AddOption(std::vector<const char*>{"Solid Color", "Rainbow"});
		pChamsSection->AddObject(CreateSliderFloat("Chams Alpha", 5, 44/*29*/, 55, 3, 150, &Vars.Visuals.Player.Chams.Alpha, 0.f, 1.f, 0, true, "Set Player Chams Transparency"));
		CDropdown* pViewmodelChamsDropdown = CreateDropdown("Viewmodel Chams", 5, 38 + 9 + 5, 80, 15, 150, &Vars.Visuals.Player.Chams.ViewModelChams, 0, "Choose Viewmodel Chams Mode");
		pViewmodelChamsDropdown->AddOption(std::vector<const char*>{"Disabled", "Solid", "Wireframe", "No-Draw"});
		CDropdown* pHandChamsDropdown = CreateDropdown("Hand Chams", 5, 55 + 9 + 5, 80, 15, 150, &Vars.Visuals.Player.Chams.HandChams, 0, "Choose Hand Chams Mode");
		pHandChamsDropdown->AddOption(std::vector<const char*>{"Disabled", "Solid", "Wireframe", "No-Draw"});
		pChamsSection->AddObject(pHandChamsDropdown);
		pChamsSection->AddObject(pViewmodelChamsDropdown);
		pChamsSection->AddObject(pChamModeDropdown);
		pVisuals->AddObject(CreateSeperator("World", 5, 261 + 15, 240));
		pVisuals->AddObject(CreateSliderFloat("ASUS", 5, 261 + 15 + 10 + 5, 100, 3, 110, &Vars.Visuals.World.ASUS, 0.f, 1.f, 0, true, "Change ASUS Walls Factor"));
		pVisuals->AddObject(pChamsSection);
		pVisuals->AddObject(pESPSection);
		/*------------------------------------------------------------------------------------------------------------------------*/
		CFrame* pMisc = CreateFrame("Misc", 265 + 260 + 260, 50);
		pMisc->AddObject(CreateCheckbox("Bunnyhop", 5, 25, 12, 12, 150, &Vars.Misc.Bunnyhop, 0, "Toggle Bunnyhop"));

		/*------------------------------------------------------------------------------------------------------------------------*/
		CFrame* pColors = CreateFrame("Colors", 265 + 260 + 260 + 260, 50);
		/*------------------------------------------------------------------------------------------------------------------------*/
		pColors->AddObject(CreateSeperator("Chams", 5, 30, 240));
		CTabSelection* pChamsColorSelector = CreateTabSelection(5 + 240 / 2 - 193 / 2, 40, 193, 15);
		///////////////////////////////////////////////////////
		CTab* pTerroristChamTab = CreateTab("Terrorists", 0, 0, 0, 59, true, Color(70, 70, 70), "Change Terrorist Chams Color");
		pTerroristChamTab->AddObject(CreateColorPallete(1, 1, 180, 56, 25, 7, true, 5, &pColorT));
		pChamsColorSelector->AddTab(pTerroristChamTab, true);
		///////////////////////////////////////////////////////
		CTab* pCTerroristsChamTab = CreateTab("C-Terrorists", 0, 0, 0, 59, true, Color(70, 70, 70), "Change Counter-Terrorist Chams Color");
		pCTerroristsChamTab->AddObject(CreateColorPallete(1, 1, 180, 56, 25, 7, true, 5, &pColorCT));
		pChamsColorSelector->AddTab(pCTerroristsChamTab);
		//===================================================//
		pColors->AddObject(CreateSeperator("Viewmodel & Weapons", 5, 45 + 15 + 59 + 10 - 5, 240));
		CTabSelection* pWeaponColorSelector = CreateTabSelection(5 + 240 / 2 - 193 / 2, 45 + 15 + 59 + 10 + 10 - 5, 193, 15);
		////////////////////////////////////////////////////////
		CTab* pWeaponWorldChamsTab = CreateTab("World", 0, 0, 0, 59, true, Color(70, 70, 70), "Change World Weapon Model Chams Color");
		pWeaponWorldChamsTab->AddObject(CreateColorPallete(1, 1, 180, 56, 25, 7, true, 5, &pColorWeapons));
		pWeaponColorSelector->AddTab(pWeaponWorldChamsTab, true);
		////////////////////////////////////////////////////////
		CTab* pViewmodelChamsTab = CreateTab("View", 0, 0, 0, 59, true, Color(70, 70, 70), "Change Viewmodel Chams Color");
		pViewmodelChamsTab->AddObject(CreateColorPallete(1, 1, 180, 56, 25, 7, true, 5, &pColorViewModel));
		pWeaponColorSelector->AddTab(pViewmodelChamsTab);
		////////////////////////////////////////////////////////
		CTab* pHandChamsTab = CreateTab("Hands", 0, 0, 0, 59, true, Color(70, 70, 70), "Change Hand Chams Color");
		pHandChamsTab->AddObject(CreateColorPallete(1, 1, 180, 56, 25, 7, true, 5, &pColorArms));
		pWeaponColorSelector->AddTab(pHandChamsTab);
		//===================================================//
		pColors->AddObject(CreateSeperator("ESP", 5, 45 + 15 + 59 + 10 + 59 + 10 + 15 + 15 - 5 - 5, 240));
		CTabSelection* pESPColorSelector = CreateTabSelection(5 + 240 / 2 - 193 / 2, 45 + 15 + 59 + 10 + 59 + 10 + 15 + 15 + 10 - 5 - 5, 193, 15);
		////////////////////////////////////////////////////////
		CTab* pTerroristESPTab = CreateTab("Terrorists", 0, 0, 0, 59, true, Color(70, 70, 70), "Change Terrorist ESP Color");
		pTerroristESPTab->AddObject(CreateColorPallete(1, 1, 180, 56, 25, 7, true, 5, &pColorTESP));
		pESPColorSelector->AddTab(pTerroristESPTab, true);
		////////////////////////////////////////////////////////
		CTab* pCTerroristESPTab = CreateTab("C-Terrorists", 0, 0, 0, 59, true, Color(70, 70, 70), "Change Counter-Terrorist ESP Color");
		pCTerroristESPTab->AddObject(CreateColorPallete(1, 1, 180, 56, 25, 7, true, 5, &pColorCTESP));
		pESPColorSelector->AddTab(pCTerroristESPTab);
		//===================================================//
		pColors->AddObject(CreateSeperator("Menu", 5, 45 + 15 + 59 + 10 + 59 + 10 + 15 + 15 - 5 - 5 + 59 + 10 + 15 + 10, 240));
		pColors->AddObject(CreateColorPallete(5 + 240 / 2 - 193 / 2, 322, 180, 24, 45, 6, true, 2, &MenuColor, "Change Menu Color", true));
		/*------------------------------------------------------------------------------------------------------------------------*/
		pColors->AddObject(pChamsColorSelector);
		pColors->AddObject(pWeaponColorSelector);
		pColors->AddObject(pESPColorSelector);
		/*------------------------------------------------------------------------------------------------------------------------*/
		CFrame* pPlayerlist = CreateFrame("Playerlist", 5, 50 + 350 + 5);
		pPlayerlist->AddObject(CreatePlayerlist());

		AddFrame(pRagebot);
		AddFrame(pHackvsHack);
		AddFrame(pVisuals);
		AddFrame(pMisc);
		AddFrame(pColors);
		AddFrame(pPlayerlist);

		CreateFont(FONT_BAR, "Calibri", 16, 600, FONTFLAG_DROPSHADOW);
		CreateFont(FONT_FRAMETITLE, "", 14, 500, FONTFLAG_DROPSHADOW);
		CreateFont(FONT_CHECKBOX, "", 14, 500, FONTFLAG_DROPSHADOW);
		CreateFont(FONT_AREA, "", 14, 500, FONTFLAG_DROPSHADOW);
		CreateFont(FONT_SEPERATOR, "", 14, 500, FONTFLAG_DROPSHADOW);
		CreateFont(FONT_TABSELECTION, "", 12, 500, FONTFLAG_DROPSHADOW);
		CreateFont(FONT_DROPDOWN, "", 12, 500, FONTFLAG_DROPSHADOW);
		CreateFont(FONT_LABEL, "", 14, 500, FONTFLAG_DROPSHADOW);
		CreateFont(FONT_BUTTON, "", 12, 500, FONTFLAG_DROPSHADOW);
		CreateFont(FONT_SLIDER, "", 12, 500, FONTFLAG_DROPSHADOW);
		//CreateFont(FONT_CLOCK, "SF Digital Readout", 60, 600, FONTFLAG_DROPSHADOW);
		CreateFont(FONT_TOOLTIP, "", 12, 500, FONTFLAG_DROPSHADOW);
		CreateFont(FONT_CLOCK, "Fixedsys", 56, 300, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);
	
	}

	int iOffset = 10;
	int iIndex = 0;

	bGotTooltipThisFrame = false;

	
	static float iAnimationOffset = -75.f;
	if (iAnimationOffset == -75.f && !ShouldDraw) return;
	float iAddValue = ((fabs(iAnimationOffset) + 1.f) / 75.f) * 750.f;

	iAnimationOffset += (Interfaces->Globals->frametime * iAddValue) * ((ShouldDraw) ? 1.f : -1.f);
	if (iAnimationOffset > 0)
		iAnimationOffset = 0.f;
	else if (iAnimationOffset <= -75.f)
		iAnimationOffset = -75.f;

	fAlphaToUse = (int)((fabs(iAnimationOffset) / 75.f) * 255.f);
	if (fAlphaToUse < 0) fAlphaToUse = 0;
	else if (fAlphaToUse > 255) fAlphaToUse = 255;

	Drawing::DrawRect(0, iAnimationOffset, ScrW, 30, 50, 50, 50, 255);

	static int iTexture = Interfaces->Surface->CreateNewTextureID(true);

	Vertex_t pVertex[4];
	Interfaces->Surface->DrawSetTexture(iTexture);
	pVertex[0].Init(Vector2D(ScrW / 2 - 70, iAnimationOffset + 30), Vector2D(ScrW / 2 + 70, iAnimationOffset + 30));
	pVertex[1].Init(Vector2D(ScrW / 2 + 70, iAnimationOffset + 30), Vector2D(ScrW / 2 + 67, iAnimationOffset + 45));
	pVertex[2].Init(Vector2D(ScrW / 2 + 67, iAnimationOffset + 45), Vector2D(ScrW / 2 - 67, iAnimationOffset + 45));
	pVertex[3].Init(Vector2D(ScrW / 2 - 67, iAnimationOffset + 45), Vector2D(ScrW / 2 - 70, iAnimationOffset + 30));

	Interfaces->Surface->DrawTexturedPolygon(4, pVertex, true);

	Drawing::DrawLine(ScrW / 2 - 70, iAnimationOffset + 30, ScrW / 2 - 66, iAnimationOffset + 45, 0, 0, 0);
	Drawing::DrawLine(ScrW / 2 - 66, iAnimationOffset + 45, ScrW / 2 + 65, iAnimationOffset + 45, 0, 0, 0);
	Drawing::DrawLine(ScrW / 2 + 65, iAnimationOffset + 45, ScrW / 2 + 70, iAnimationOffset + 30, 0, 0, 0);

	/*Drawing::DrawLine(ScrW / 2 - 50, 30, ScrW / 2 - 46, 45, 0, 0, 0, 255);
	Drawing::DrawLine(ScrW / 2 - 45, 45, ScrW / 2 + 45, 45, 0, 0, 0, 255);
	Drawing::DrawLine(ScrW / 2 + 44, 45, ScrW / 2 + 49, 30, 0, 0, 0, 255);*/
	//Drawing::DrawLine(ScrW / 2 - 50, 30, ScrW / 2 - 45, 45, 0, 0, 0, 255);

	//Drawing::DrawOutlinedRect(0, 0, ScrW, 30, 0, 0, 0, 255);
	Drawing::DrawLine(0, iAnimationOffset + 30, ScrW / 2 - 70, iAnimationOffset + 30, 0, 0, 0);
	Drawing::DrawLine(ScrW / 2 + 70, iAnimationOffset + 30, ScrW, iAnimationOffset + 30, 0, 0, 0);

	Color colFromHSB = UTIL::GetFromHSB(flHue, 1.f, 1.f);
	Drawing::GetTextSize(Font[FONT_CLOCK], tw, th, "ampaste");
	Drawing::DrawString(Font[FONT_CLOCK], ScrW / 2 - tw / 2, iAnimationOffset + 45 / 2 - th / 2 - 3, colFromHSB.r, colFromHSB.g, colFromHSB.b, 255, "ampaste");

	Drawing::GetTextSize(Font[FONT_BAR], tw, th, "welcome", pUser);
	Drawing::DrawString(Font[FONT_BAR], ScrW - tw - 5, iAnimationOffset + 15 - th / 2, 220, 220, 220, 255, "welcome", pUser);

	for (auto pFrame : vecFrames) {
		bool inBox = InBox(5 + iOffset, iAnimationOffset + 5, 80, 20);
		if (inBox || pFrame->bActive)
			Drawing::DrawRect(5 + iOffset, iAnimationOffset + 5, 80, 20, 110, 110, 110, 100);
		else
			Drawing::DrawRect(5 + iOffset, iAnimationOffset + 5, 80, 20, 90, 90, 90, 100);

		Drawing::GetTextSize(Font[FONT_BUTTON], tw, th, pFrame->pTitle);
		Drawing::DrawString(Font[FONT_BUTTON], 5 + iOffset + 40 - tw / 2, iAnimationOffset + 5 + 20 / 2 - th / 2, 220, 220, 220, 255, pFrame->pTitle);

		Drawing::DrawOutlinedRect(5 + iOffset, iAnimationOffset + 5, 80, 20, 0, 0, 0, 255);

		if (inBox && MouseDown)
			pFrame->bActive = !pFrame->bActive;

		iOffset += 95;

		if (pFrame->bActive) 
			pFrame->Draw(); 

		iIndex++;
	}

	_MouseDownLastFrame = _MouseDown;

	if (flTooltipTime < .8f)
		pTooltipText = 0;

	if (ShouldDraw && bGotTooltipThisFrame) {
		if (pTooltipText) {
			Drawing::GetTextSize(Font[FONT_TOOLTIP], tw, th, pTooltipText);
			Drawing::DrawRect(MouseX, MouseY - 20, tw + 10, 15, 50, 50, 50, 255);
			Drawing::DrawOutlinedRect(MouseX, MouseY - 20, tw + 10, 15, 0, 0, 0, 255);
			Drawing::DrawString(Font[FONT_TOOLTIP], MouseX + 5, MouseY - 20 + 15 / 2 - th / 2, 220, 220, 220, 255, pTooltipText);
		}
	}
	else {
		pTooltipText = 0;
		flTooltipTime = 0.f;
	}

	if(ShouldDraw) DrawMouse();
}

void Menu::DrawMouse() {
	int x, y;
	x = MouseX;
	y = MouseY;

	Drawing::DrawRect(x, y, 1, 12, 0, 0, 0);
	Drawing::DrawRect(x + 1, y, 1, 11, 0, 0, 0);
	Drawing::DrawRect(x + 1, y + 11, 1, 1, 0, 0, 0);
	Drawing::DrawRect(x + 2, y + 1, 1, 1, 0, 0, 0);
	Drawing::DrawRect(x + 2, y + 10, 1, 1, 0, 0, 0);
	Drawing::DrawRect(x + 3, y + 2, 1, 1, 0, 0, 0);
	Drawing::DrawRect(x + 3, y + 9, 1, 1, 0, 0, 0);
	Drawing::DrawRect(x + 4, y + 3, 1, 1, 0, 0, 0);
	Drawing::DrawRect(x + 5, y + 4, 1, 1, 0, 0, 0);
	Drawing::DrawRect(x + 6, y + 5, 1, 1, 0, 0, 0);
	Drawing::DrawRect(x + 7, y + 6, 1, 1, 0, 0, 0);
	Drawing::DrawRect(x + 8, y + 7, 1, 1, 0, 0, 0);
	Drawing::DrawRect(x + 4, y + 8, 4, 1, 0, 0, 0);

	Drawing::DrawRect(x + 1, y + 1, 1, 10, MenuColor.r, MenuColor.g, MenuColor.b);
	Drawing::DrawRect(x + 2, y + 2, 1, 8, MenuColor.r, MenuColor.g, MenuColor.b);
	Drawing::DrawRect(x + 3, y + 3, 1, 6, MenuColor.r, MenuColor.g, MenuColor.b);
	Drawing::DrawRect(x + 4, y + 4, 1, 4, MenuColor.r, MenuColor.g, MenuColor.b);
	Drawing::DrawRect(x + 5, y + 5, 1, 3, MenuColor.r, MenuColor.g, MenuColor.b);
	Drawing::DrawRect(x + 6, y + 6, 1, 2, MenuColor.r, MenuColor.g, MenuColor.b);
	Drawing::DrawRect(x + 7, y + 7, 1, 1, MenuColor.r, MenuColor.g, MenuColor.b);
}