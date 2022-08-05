namespace Menu {
	bool ShouldDraw;
}
/*
namespace Stepmania {
	namespace Announcer {
		const char* pAnnouncerBad[] = {
			"C:\\CSGODDR\\announcer\\bad\\1.wav",
			"C:\\CSGODDR\\announcer\\bad\\2.wav",
			"C:\\CSGODDR\\announcer\\bad\\3.wav",
			"C:\\CSGODDR\\announcer\\bad\\4.wav",
			"C:\\CSGODDR\\announcer\\bad\\5.wav",
			"C:\\CSGODDR\\announcer\\bad\\6.wav",
			"C:\\CSGODDR\\announcer\\bad\\7.wav",
		};

		const char* pAnnouncerGood[] = {
			"C:\\CSGODDR\\announcer\\good\\1.wav",
			"C:\\CSGODDR\\announcer\\good\\2.wav",
			"C:\\CSGODDR\\announcer\\good\\3.wav",
			"C:\\CSGODDR\\announcer\\good\\4.wav",
			"C:\\CSGODDR\\announcer\\good\\5.wav",
		};

		const char* pAnnouncerPerfect[] = {
			"C:\\CSGODDR\\announcer\\perfect\\1.wav",
			"C:\\CSGODDR\\announcer\\perfect\\2.wav",
			"C:\\CSGODDR\\announcer\\perfect\\3.wav",
			"C:\\CSGODDR\\announcer\\perfect\\4.wav",
			"C:\\CSGODDR\\announcer\\perfect\\5.wav",
			"C:\\CSGODDR\\announcer\\perfect\\6.wav",
			"C:\\CSGODDR\\announcer\\perfect\\7.wav",
			"C:\\CSGODDR\\announcer\\perfect\\8.wav",
			"C:\\CSGODDR\\announcer\\perfect\\9.wav",
		};

		const char* pStageEnd[] = {
			"C:\\CSGODDR\\announcer\\stage_end\\1.wav",
			"C:\\CSGODDR\\announcer\\stage_end\\2.wav",
			"C:\\CSGODDR\\announcer\\stage_end\\3.wav",
		};

		const char* pStageFailed[] = {
			"C:\\CSGODDR\\announcer\\stage_failed\\1.wav"
			"C:\\CSGODDR\\announcer\\stage_failed\\2.wav"
			"C:\\CSGODDR\\announcer\\stage_failed\\3.wav"
			"C:\\CSGODDR\\announcer\\stage_failed\\4.wav"
			"C:\\CSGODDR\\announcer\\stage_failed\\5.wav"
			"C:\\CSGODDR\\announcer\\stage_failed\\6.wav"
			"C:\\CSGODDR\\announcer\\stage_failed\\7.wav"
			"C:\\CSGODDR\\announcer\\stage_failed\\8.wav"
			"C:\\CSGODDR\\announcer\\stage_failed\\9.wav"
		};

		const char* pStageStart[] = {
			"C:\\CSGODDR\\announcer\\stage_start\\1.wav",
			"C:\\CSGODDR\\announcer\\stage_start\\2.wav",
			"C:\\CSGODDR\\announcer\\stage_start\\3.wav",
		};
	}
	namespace Songs {
		const char* pSongNames[] = {
			"Supervideo",
			"Wuki Wuki",
		};

		float flActualSongDurations[] = {
			101.25f,
			86.f,
		};

		float flPlaySongDurations[] = {
			90.5f,
			72.5f,
		};

		float flStartPoint[] = {
			10.f,
		};

		const char* pSongCommands[] = {
			"play good_stepmania/songs/supervideo.mp3",
			"play good_stepmania/songs/wukiwuki.mp3",
		};

		const char* pPreviewCommands[] = {
			"play good_stepmania/preview/supervideo.mp3",
			"play good_stepmania/preview/wukiwuki.mp3",
		};
	}

	namespace SongPlayer {
		enum ARROW_TYPES {
			ARROW_LEFT,
			ARROW_DOWN,
			ARROW_UP,
			ARROW_RIGHT,
		};
		struct ArrowStruct {
			float flTimeToHit;
			ARROW_TYPES eArrowType;
			bool bHold;
			float fHoldTime;
			bool bDone;
		};

		struct LyricsStruct {
			float StartTime;
			float EndTime;
			const char* pText;
			bool bDone;
		};

		struct SongStruct {
			std::vector<ArrowStruct> vecArrows;
			std::vector<LyricsStruct> vecLyrics;
		};

		SongStruct pSongData[2];
	}

	namespace Scoring {
		enum Hit_Types {
			HIT_RANDOM,
			HIT_BAD,
			HIT_EARLY,
			HIT_GOOD,
			HIT_PERFECT,
			HIT_FLAWLESS,
			HIT_LATE,
			HIT_MAX = HIT_LATE,
		};

		void RegisterHit(float);

		int iHits[HIT_MAX + 1];
	}
}

namespace Stepmania {
	void ConfirmSelection();
	void DrawOverlay();
	void PlayPreview();
	int SongIndex;
	bool bHasPrecached;
	void Precache();
	int ScrW, ScrH;
	int iCurAlpha;
	bool bDrawOverlay;
	float flFullDuration;
	float flPlayDuration;
	float flCurrentTime;
	bool bStartedPlayingSong;
	bool bPlayedEndSound;
	void DrawLyrics();
	void ResetLyrics();
	unsigned long LyricsFont;
	int tw, th;
	bool bHasPlayedStartingSound;
	bool bHasPlayedPreStartSound;
	bool bShouldDrawCountdown;

	void Init();
	void DrawCountdown();
}

void Stepmania::ResetLyrics() {
	for (SongPlayer::LyricsStruct& pLyrics : SongPlayer::pSongData[SongIndex].vecLyrics)
		pLyrics.bDone = false;
}

void Stepmania::DrawLyrics() {
	for (SongPlayer::LyricsStruct& pLyrics : SongPlayer::pSongData[SongIndex].vecLyrics) {
		if (pLyrics.bDone) continue;
		if (flCurrentTime < pLyrics.StartTime) continue;
		if (flCurrentTime >= pLyrics.EndTime) {
			pLyrics.bDone = true;
			continue;
		}

		Drawing::GetTextSize(LyricsFont, tw, th, pLyrics.pText);
		Drawing::DrawString(LyricsFont, ScrW / 2 - tw / 2, ScrH - 100, 220, 220, 220, 255, pLyrics.pText);

		int iStrLen = strlen(pLyrics.pText);
		int iCurrentChar = (int)ceil((((flCurrentTime - pLyrics.StartTime) / (pLyrics.EndTime - pLyrics.StartTime)) * (float)iStrLen));

		char* pBuf = (char*)malloc(iCurrentChar + 1);
		strncpy(pBuf, pLyrics.pText, iCurrentChar);
		pBuf[iCurrentChar] = '\0';
		Drawing::DrawString(LyricsFont, ScrW / 2 - tw / 2, ScrH - 100, 152, 251, 152, 255, pBuf);
		free(pBuf);

		break;
	}
}

void Stepmania::ConfirmSelection() {
	bDrawOverlay = true;
	iCurAlpha = 0;
	flFullDuration = Songs::flActualSongDurations[SongIndex];
	flPlayDuration = Songs::flPlaySongDurations[SongIndex];
	bPlayedEndSound = false;
	bStartedPlayingSong = false;
	flCurrentTime = 0.f;
	bHasPlayedStartingSound = false;
	bHasPlayedPreStartSound = false;
	bShouldDrawCountdown = false;

	for (int i = 0; i < (Scoring::HIT_MAX + 1); i++)
		Scoring::iHits[i] = 0;
}

void Stepmania::DrawCountdown() {
	float flCurCountdown = Songs::flStartPoint[SongIndex] - flCurrentTime;

	static unsigned long Font;
	if (!Font) {
		Font = Interfaces->Surface->CreateFont();
		Interfaces->Surface->SetFontGlyphSet(Font, "Fixedsys", 80, 500, 0, 0, FONTFLAG_DROPSHADOW, 0, 0);
	}

	int iAlpha = (flCurCountdown > -.4f) ? 255 : (255 - (int)((fabsf(flCurCountdown) - .4f) * 2550.f));
	if (iAlpha > 255) iAlpha = 255; else if (iAlpha < 0 || flCurCountdown <= -.5f) iAlpha = 0;

	Drawing::GetTextSize(Font, tw, th, (flCurCountdown > 2.f ? "3" : (flCurCountdown > 1.f ? "2" : (flCurCountdown > 0.f ? "1" : "GO!"))));
	Drawing::DrawString(Font, ScrW / 2 - tw / 2, ScrH / 2 - th / 2, 255, 0, 0, iAlpha, (flCurCountdown > 2.f ? "3" : (flCurCountdown > 1.f ? "2" : (flCurCountdown > 0.f ? "1" : "GO!"))));

	if (flCurCountdown <= -.5f)
		bShouldDrawCountdown = false;
}

void Stepmania::DrawOverlay() {
	if (!bStartedPlayingSong) {
		bStartedPlayingSong = true;
		Interfaces->Engine->ExecuteClientCmd(Songs::pSongCommands[SongIndex]);
		PlaySoundA(Announcer::pStageStart[0], 0, SND_ASYNC);
		//return;
	}
	
	Interfaces->Surface->GetScreenSize(ScrW, ScrH);

	if (!bPlayedEndSound && flPlayDuration < -2.f) {
		bPlayedEndSound = true;
		PlaySoundA(Announcer::pStageEnd[rand() % 3], 0, SND_ASYNC);
	}

	if (flFullDuration <= 0.f) {
		iCurAlpha -= (int)(Interfaces->Globals->frametime * 220.f * 2.f);
		if (iCurAlpha <= 0) {
			bDrawOverlay = false;
			ResetLyrics();
			return;
		}
	}
	else {
		iCurAlpha += (int)(Interfaces->Globals->frametime * 220.f * 2.f);
	}

	flFullDuration -= Interfaces->Globals->frametime;
	flPlayDuration -= Interfaces->Globals->frametime;
	flCurrentTime += Interfaces->Globals->frametime;

	if (!bHasPlayedStartingSound && (flCurrentTime + .5f) >= Songs::flStartPoint[SongIndex]) {
		bHasPlayedStartingSound = true;
		PlaySoundA(Announcer::pStageStart[1], 0, SND_ASYNC);
	}


	if (!bHasPlayedPreStartSound && (flCurrentTime + 3.f) >= Songs::flStartPoint[SongIndex]) {
		bHasPlayedPreStartSound = true;
		PlaySoundA(Announcer::pStageStart[2], 0, SND_ASYNC);
		bShouldDrawCountdown = true;
	}

	if (iCurAlpha < 0) iCurAlpha = 0; else if (iCurAlpha > 220) iCurAlpha = 220;
	Drawing::DrawRect(0, 0, ScrW, ScrH, 0, 0, 0, iCurAlpha);
	int x, y;
	/*left arrow*/ /*
	x = ScrW / 2 - (45 * 4 + 25 * 3 + 25) / 2;
	y = 80;
	Drawing::DrawLine(x, y, x + 20, y + 20, 255, 255 ,255);
	Drawing::DrawLine(x, y, x + 20, y - 20, 255, 255, 255);
	Drawing::DrawLine(x + 20, y - 20, x + 23, y - 15, 255, 255, 255);
	Drawing::DrawLine(x + 20, y + 20, x + 23, y + 15, 255, 255, 255);
	Drawing::DrawLine(x + 23, y + 15, x + 18, y + 10, 255, 255, 255);
	Drawing::DrawLine(x + 23, y - 15, x + 18, y - 10, 255, 255, 255);
	Drawing::DrawLine(x + 18, y - 10, x + 40, y - 10, 255, 255, 255);
	Drawing::DrawLine(x + 18, y + 10, x + 40, y + 10, 255, 255, 255);
	Drawing::DrawLine(x + 40, y + 10, x + 40, y - 10, 255, 255, 255);

	/*down arrow*/ /*
	x += 25 + 45;
	y = 80;

	Drawing::DrawLine(x, y + 20, x - 20, y, 255, 255, 255);
	Drawing::DrawLine(x, y + 20, x + 20, y, 255, 255, 255);
	Drawing::DrawLine(x + 20, y, x + 17, y - 5, 255, 255, 255);
	Drawing::DrawLine(x - 20, y, x - 17, y - 5, 255, 255, 255);
	Drawing::DrawLine(x + 17, y - 5, x + 12, y, 255, 255, 255);
	Drawing::DrawLine(x - 17, y - 5, x - 12, y, 255, 255, 255);
	Drawing::DrawLine(x - 12, y, x - 12, y - 20, 255, 255, 255);
	Drawing::DrawLine(x + 12, y, x + 12, y - 20, 255, 255, 255);
	Drawing::DrawLine(x + 12, y - 20, x - 12, y - 20, 255, 255, 255);
	/*up arrow*/ /*
	x += 25 + 45;
	y = 80;

	Drawing::DrawLine(x, y - 20, x + 20, y, 255, 255, 255);
	Drawing::DrawLine(x, y - 20, x - 20, y, 255, 255, 255);
	Drawing::DrawLine(x - 20, y, x - 17, y + 5, 255, 255, 255);
	Drawing::DrawLine(x + 20, y, x + 17, y + 5, 255, 255, 255);
	Drawing::DrawLine(x - 17, y + 5, x - 12, y, 255, 255, 255);
	Drawing::DrawLine(x + 17, y + 5, x + 12, y, 255, 255, 255);
	Drawing::DrawLine(x + 12, y, x + 12, y + 20, 255, 255, 255);
	Drawing::DrawLine(x - 12, y, x - 12, y + 20, 255, 255, 255);
	Drawing::DrawLine(x - 12, y + 20, x + 12, y + 20, 255, 255, 255);

	/*right arrow*/ /*

	x += 25 + 45;
	y = 80;

	Drawing::DrawLine(x, y, x - 20, y - 20, 255, 255, 255);
	Drawing::DrawLine(x, y, x - 20, y + 20, 255, 255, 255);
	Drawing::DrawLine(x - 20, y + 20, x - 23, y + 15, 255, 255, 255);
	Drawing::DrawLine(x - 20, y - 20, x - 23, y - 15, 255, 255, 255);
	Drawing::DrawLine(x - 23, y - 15, x - 18, y - 10, 255, 255, 255);
	Drawing::DrawLine(x - 23, y + 15, x - 18, y + 10, 255, 255, 255);
	Drawing::DrawLine(x - 18, y + 10, x - 40, y + 10, 255, 255, 255);
	Drawing::DrawLine(x - 18, y - 10, x - 40, y - 10, 255, 255, 255);
	Drawing::DrawLine(x - 40, y - 10, x - 40, y + 10, 255, 255, 255);

	if(bShouldDrawCountdown) DrawCountdown();
	DrawLyrics();
}

void Stepmania::PlayPreview() {
	Interfaces->Engine->ExecuteClientCmd(Songs::pPreviewCommands[SongIndex]);
}

void Stepmania::Precache() {
	for (auto pSong : Songs::pSongCommands)
		Interfaces->Engine->ExecuteClientCmd(pSong);

	for (auto pPreview : Songs::pPreviewCommands)
		Interfaces->Engine->ExecuteClientCmd(pPreview);

	bHasPrecached = true;

	Interfaces->Engine->ExecuteClientCmd("stopsound");
}

void Stepmania::Init() {
	/*
	LyricsStruct
		float StartTime;
		float EndTime;
		const char* pText;
		bool bDone;
	*/

	/*

	LyricsFont = Interfaces->Surface->CreateFont();
	Interfaces->Surface->SetFontGlyphSet(LyricsFont, "", 32, 500, 0, 0, FONTFLAG_DROPSHADOW, 0, 0);

	/*SUPERVIDEO*/
	/*
	SongPlayer::SongStruct& pSupervideo = SongPlayer::pSongData[0];
	
	/*Lyrics*/

	/*
	pSupervideo.vecLyrics.push_back({ 0.f, 1.7f, "Na-na-na-na-na-na" });
	pSupervideo.vecLyrics.push_back({ 2.69f, 4.2f, "Let's go supervideo" });
	pSupervideo.vecLyrics.push_back({ 4.3375f, 7.f, "Na-na-na-na-na-na" });
	pSupervideo.vecLyrics.push_back({ 7.2f, 9.55f, "Go, go, go, go, go" });
	pSupervideo.vecLyrics.push_back({ 10.1f, 14.15f, "Video, video, video, video, video, video" });
	pSupervideo.vecLyrics.push_back({ 14.38f, 15.575f, "Oh, yes we loves it" });
	pSupervideo.vecLyrics.push_back({ 15.8f, 21.f, "Video, video, video, video, video, video" });
	pSupervideo.vecLyrics.push_back({ 32.8f, 33.6f, "So pretty girl" });
	pSupervideo.vecLyrics.push_back({ 33.8f, 35.35f, "Hey my supervideo" });
	pSupervideo.vecLyrics.push_back({ 35.5f, 38.f, "To be the queen all over the stars" });
	pSupervideo.vecLyrics.push_back({ 38.5f, 39.2f, "It's not a dream" });
	pSupervideo.vecLyrics.push_back({ 39.575f, 41.025f, "If you live in Tokyo" });
	pSupervideo.vecLyrics.push_back({ 41.335f, 43.485f, "Because the city's highly bizarre" });
	pSupervideo.vecLyrics.push_back({ 55.22f, 57.95f, "Go, go, go, what a pretty lover" });
	pSupervideo.vecLyrics.push_back({ 58.1f, 60.22f, "Only love the way you are (are)" });
	pSupervideo.vecLyrics.push_back({ 60.22f, 63.57f, "Oh baby go go go, like a happy soldier" });
	pSupervideo.vecLyrics.push_back({ 63.57f, 67.585f, "Sitting in a Sushi bar" });
	pSupervideo.vecLyrics.push_back({ 67.8f, 70.456f, "Hey wanna game to play? (To play)" });
	pSupervideo.vecLyrics.push_back({ 70.6315f, 73.3f, "A candy lover night and day" });
	pSupervideo.vecLyrics.push_back({ 73.445f, 76.1210f, "Hey wanna game to play? (To play)" });
	pSupervideo.vecLyrics.push_back({ 76.275f, 78.412f, "That all the people want to start" });
	pSupervideo.vecLyrics.push_back({ 79.0875f, 79.0875f + 2.656f, "Hey wanna game to play? (To play)" });
	pSupervideo.vecLyrics.push_back({ 81.918f, 84.75f, "A shady love can take you away (take you away)" });
	pSupervideo.vecLyrics.push_back({ 84.75f, 84.75f + 2.656f, "Hey wanna game to play? (To play)" });
	pSupervideo.vecLyrics.push_back({ 84.75f + 2.656f + .154f, 84.75f + 2.656f + .154f + 2.137f, "That all the people want to start" });
	pSupervideo.vecLyrics.push_back({ 84.75f + 2.656f + .154f + 2.137f, 90.59f , "Let's go together" });
	*/
	/*Arrows*/

	/*WUKI WUKI*//*
}*/