enum MotionBlurMode_t
{
	MOTION_BLUR_DISABLE = 1,
	MOTION_BLUR_GAME = 2,			// Game uses real-time inter-frame data
	MOTION_BLUR_SFM = 3				// Use SFM data passed in CViewSetup structure
};

class CViewSetup{
public:
	int			x;
	int			x_old;
	int			y;
	int			y_old;
	int			width;
	int			width_old;
	int			height;
	int			height_old;
	bool		m_bOrtho;
	float		m_OrthoLeft;
	float		m_OrthoTop;
	float		m_OrthoRight;
	float		m_OrthoBottom;
	bool		m_bCustomViewMatrix; // 0x34
	matrix3x4	m_matCustomViewMatrix; // 0x38
	char		pad_0x68[0x48]; // 0x68
	float		fov; //	0xB0
	float		fovViewmodel;
	Vector		origin;
	Vector		angles;
	float		zNear;
	float		zFar;
	float		zNearViewmodel;
	float		zFarViewmodel;
	float		m_flAspectRatio;
	float		m_flNearBlurDepth;
	float		m_flNearFocusDepth;
	float		m_flFarFocusDepth;
	float		m_flFarBlurDepth;
	float		m_flNearBlurRadius;
	float		m_flFarBlurRadius;
	int			m_nDoFQuality;
	MotionBlurMode_t	m_nMotionBlurMode;
	float		m_flShutterTime;
	Vector		m_vShutterOpenPosition;
	Vector		m_shutterOpenAngles;
	Vector		m_vShutterClosePosition;
	Vector		m_shutterCloseAngles;
	float		m_flOffCenterTop;
	float		m_flOffCenterBottom;
	float		m_flOffCenterLeft;
	float		m_flOffCenterRight;
	int			m_EdgeBlur;

}; // size = 0x14C = 332

void __fastcall hkOverrideView(void* pThis, void*,	CViewSetup* view) {
	float& fov = view->fov;
	fov = fov >= 70.f ? 120.f : fov;

	ClientModeVmt->getOrig<void(__thiscall*)(void*, void*)>(18)(pThis, view);
}