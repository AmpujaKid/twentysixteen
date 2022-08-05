struct DrawModelState_t {
	studiohdr_t*			m_pStudioHdr;
	void*					m_pStudioHWData;
	IClientRenderable*		m_pRenderable;
	const matrix3x4			*m_pModelToWorld;
	unsigned short			m_decals;
	int						m_drawFlags;
	int						m_lod;
};