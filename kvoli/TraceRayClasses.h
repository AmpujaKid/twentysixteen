class VectorAligned : public Vector {
public:
	VectorAligned() { x = y = z = 0.0f; }
	VectorAligned(const Vector& v) { x = v.x; y = v.y; z = v.z; }
	float w;
};

FORCEINLINE void VectorAdd(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

FORCEINLINE void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

struct Ray_t
{
	VectorAligned  m_Start;	// starting point, centered within the extents
	VectorAligned  m_Delta;	// direction + length of the ray
	VectorAligned  m_StartOffset;	// Add this to m_Start to get the actual ray start
	VectorAligned  m_Extents;	// Describes an axis aligned box extruded along a ray
	const matrix3x4 *m_pWorldAxisTransform;
	bool	m_IsRay;	// are the extents zero?
	bool	m_IsSwept;	// is delta != 0?

	Ray_t() : m_pWorldAxisTransform(NULL) {}

	void Init(Vector const& start, Vector const& end)
	{
		VectorSubtract(end, start, m_Delta);

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		VectorClear(m_Extents);
		m_pWorldAxisTransform = NULL;
		m_IsRay = true;

		// Offset m_Start to be in the center of the box...
		VectorClear(m_StartOffset);
		VectorCopy(start, m_Start);
	}

	void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs)
	{
		VectorSubtract(end, start, m_Delta);

		m_pWorldAxisTransform = NULL;
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		VectorSubtract(maxs, mins, m_Extents);
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		VectorAdd(mins, maxs, m_StartOffset);
		m_StartOffset *= 0.5f;
		VectorAdd(start, m_StartOffset, m_Start);
		m_StartOffset *= -1.0f;
	}
	// compute inverse delta
	Vector InvDelta() const
	{
		Vector vecInvDelta;
		for (int iAxis = 0; iAxis < 3; ++iAxis)
		{
			if (m_Delta[iAxis] != 0.0f)
			{
				vecInvDelta[iAxis] = 1.0f / m_Delta[iAxis];
			}
			else
			{
				vecInvDelta[iAxis] = FLT_MAX;
			}
		}
		return vecInvDelta;
	}
private:
};

struct csurface_t {
	const char	*name;
	short		surfaceProps;
	unsigned short	flags;
};

struct cplane_t {
	Vector	normal;
	float	dist;
	byte	type;
	byte	signbits;
	byte	pad[2];
};

class CBaseTrace
{
public:

	// these members are aligned!!
	Vector			startpos;				// start position
	Vector			endpos;					// final position
	cplane_t		plane;					// surface normal at impact

	float			fraction;				// time completed, 1.0 = didn't hit anything

	int				contents;				// contents on other side of surface hit
	unsigned short	dispFlags;				// displacement flags for marking surfaces with data

	bool			allsolid;				// if true, plane is not valid
	bool			startsolid;				// if true, the initial point was in a solid area

};

class CGameTrace : public CBaseTrace
{
public:
	bool DidHitWorld() const;
	bool DidHitNonWorldEntity() const;
	int GetEntityIndex() const;
	bool DidHit() const;
	bool IsVisible() const;
public:
	float			fractionleftsolid;	// time we left a solid, only valid if we started in solid
	csurface_t		surface;			// surface hit (impact surface)
	int				hitgroup;			// 0 == generic, non-zero is specific body part
	short			physicsbone;		// physics bone hit by trace in studio
	unsigned short	worldSurfaceIndex;	// Index of the msurface2_t, if applicable
	CBaseEntity *m_pEnt;
	// NOTE: this member is overloaded.
	// If hEnt points at the world entity, then this is the static prop index.
	// Otherwise, this is the hitbox index.
	int			hitbox;					// box hit by trace in studio
};

typedef CGameTrace trace_t;

enum TraceType_t {
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

class ITraceFilter {
public:
	virtual bool ShouldHitEntity(void *pEntity, int contentsMask) = 0;
	virtual TraceType_t	GetTraceType() const = 0;
};

class CTraceFilterSimple : public ITraceFilter {
public:
	CTraceFilterSimple(void* pPass1) {
		passentity1 = pPass1;
	}

	virtual bool ShouldHitEntity(void *pEntityHandle, int contentsMask) {
		return !(pEntityHandle == passentity1);
	}

	virtual TraceType_t    GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	void* passentity1;
};

class CTraceFilterSkipTwoEntities : public ITraceFilter {
public:
	CTraceFilterSkipTwoEntities(void *pPassEnt1, void *pPassEnt2) {
		passentity1 = pPassEnt1;
		passentity2 = pPassEnt2;
	}

	virtual bool ShouldHitEntity(void *pEntityHandle, int contentsMask) {
		return !(pEntityHandle == passentity1 || pEntityHandle == passentity2);
	}

	virtual TraceType_t    GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	void *passentity1;
	void *passentity2;
};