namespace UTIL {
	bool WorldToScreen(const Vector&, Vector&);
}

bool UTIL::WorldToScreen(const Vector& screen, Vector& onscreen) {
	return (Interfaces->DebugOverlay->ScreenPosition(screen, onscreen) != 1);
}