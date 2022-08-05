template< typename Fn >
Fn call_vfunc(void* vTable, unsigned int index) {
	return (*(Fn**)vTable)[index];
}

#define TOSTR(a) #a

#define VFUNC0(funcname, index)\
__forceinline void funcname(){\
typedef void(__thiscall* F)(void*);\
return call_vfunc<F>(this, index)(this);\
}

#define VFUNC1(funcname, index, arg1)\
__forceinline void funcname(arg1 a1){\
typedef void(__thiscall* F)(void*, arg1);\
return call_vfunc<F>(this, index)(this, a1);\
}

#define VFUNC2(funcname, index, arg1, arg2)\
__forceinline void funcname(arg1 a1, arg2 a2){\
typedef void(__thiscall* F)(void*, arg1, arg2);\
return call_vfunc<F>(this, index)(this, a1, a2);\
}

#define VFUNC3(funcname, index, arg1, arg2, arg3)\
__forceinline void funcname(arg1 a1, arg2 a2, arg3 a3){\
typedef void(__thiscall* F)(void*, arg1, arg2, arg3);\
return call_vfunc<F>(this, index)(this, a1, a2, a3);\
}

#define VFUNC4(funcname, index, arg1, arg2, arg3, arg4)\
__forceinline void funcname(arg1 a1, arg2 a2, arg3 a3, arg4 a4){\
typedef void(__thiscall* F)(void*, arg1, arg2, arg3, arg4);\
return call_vfunc<F>(this, index)(this, a1, a2, a3, a4);\
}

#define VFUNC5(funcname, index, arg1, arg2, arg3, arg4, arg5)\
__forceinline void funcname(arg1 a1, arg2 a2, arg3 a3, arg4 a4, arg5 a5){\
typedef void(__thiscall* F)(void*, arg1, arg2, arg3, arg4, arg5);\
return call_vfunc<F>(this, index)(this, a1, a2, a3, a4, a5);\
}

#define VFUNC6(funcname, index, arg1, arg2, arg3, arg4, arg5, arg6)\
__forceinline void funcname(arg1 a1, arg2 a2, arg3 a3, arg4 a4, arg5 a5, arg6 a6){\
typedef void(__thiscall* F)(void*, arg1, arg2, arg3, arg4, arg5, arg6);\
return call_vfunc<F>(this, index)(this, a1, a2, a3, a4, a5, a6);\
}

#define VFUNC7(funcname, index, arg1, arg2, arg3, arg4, arg5, arg6, arg7)\
__forceinline void funcname(arg1 a1, arg2 a2, arg3 a3, arg4 a4, arg5 a5, arg6 a6, arg7 a7){\
typedef void(__thiscall* F)(void*, arg1, arg2, arg3, arg4, arg5, arg6, arg7);\
return call_vfunc<F>(this, index)(this, a1, a2, a3, a4, a5, a6, a7);\
}

#define VFUNC8(funcname, index, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)\
__forceinline void funcname(arg1 a1, arg2 a2, arg3 a3, arg4 a4, arg5 a5, arg6 a6, arg7 a7, arg8 a8){\
typedef void(__thiscall* F)(void*, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);\
return call_vfunc<F>(this, index)(this, a1, a2, a3, a4, a5, a6, a7, a8);\
}

#define VFUNC9(funcname, index, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)\
__forceinline void funcname(arg1 a1, arg2 a2, arg3 a3, arg4 a4, arg5 a5, arg6 a6, arg7 a7, arg8 a8, arg9 a9){\
typedef void(__thiscall* F)(void*, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);\
return call_vfunc<F>(this, index)(this, a1, a2, a3, a4, a5, a6, a7, a8, a9);\
}

#define VFUNCR0(funcname, index, rettype)\
__forceinline rettype funcname(){\
typedef rettype(__thiscall* F)(void*);\
return call_vfunc<F>(this, index)(this);\
}

#define VFUNCR1(funcname, index, rettype, arg1)\
__forceinline rettype funcname(arg1 a1){\
typedef rettype(__thiscall* F)(void*, arg1);\
return call_vfunc<F>(this, index)(this, a1);\
}

#define VFUNCR2(funcname, index, rettype, arg1, arg2)\
__forceinline rettype funcname(arg1 a1, arg2 a2){\
typedef rettype(__thiscall* F)(void*, arg1, arg2);\
return call_vfunc<F>(this, index)(this, a1, a2);\
}

#define VFUNCR3(funcname, index, rettype, arg1, arg2, arg3)\
__forceinline rettype funcname(arg1 a1, arg2 a2, arg3 a3){\
typedef rettype(__thiscall* F)(void*, arg1, arg2, arg3);\
return call_vfunc<F>(this, index)(this, a1, a2, a3);\
}

#define VFUNCR4(funcname, index, rettype, arg1, arg2, arg3, arg4)\
__forceinline rettype funcname(arg1 a1, arg2 a2, arg3 a3, arg4 a4){\
typedef rettype(__thiscall* F)(void*, arg1, arg2, arg3, arg4);\
return call_vfunc<F>(this, index)(this, a1, a2, a3, a4);\
}

#define VFUNCR5(funcname, index, rettype, arg1, arg2, arg3, arg4, arg5)\
__forceinline rettype funcname(arg1 a1, arg2 a2, arg3 a3, arg4 a4, arg5 a5){\
typedef rettype(__thiscall* F)(void*, arg1, arg2, arg3, arg4, arg5);\
return call_vfunc<F>(this, index)(this, a1, a2, a3, a4, a5);\
}

#define VFUNCR6(funcname, index, rettype, arg1, arg2, arg3, arg4, arg5, arg6)\
__forceinline rettype funcname(arg1 a1, arg2 a2, arg3 a3, arg4 a4, arg5 a5, arg6 a6){\
typedef rettype(__thiscall* F)(void*, arg1, arg2, arg3, arg4, arg5, arg6);\
return call_vfunc<F>(this, index)(this, a1, a2, a3, a4, a5, a6);\
}

#define VFUNCR7(funcname, index, rettype, arg1, arg2, arg3, arg4, arg5, arg6, arg7)\
__forceinline rettype funcname(arg1 a1, arg2 a2, arg3 a3, arg4 a4, arg5 a5, arg6 a6, arg7 a7){\
typedef rettype(__thiscall* F)(void*, arg1, arg2, arg3, arg4, arg5, arg6, arg7);\
return call_vfunc<F>(this, index)(this, a1, a2, a3, a4, a5, a6, a7);\
}

#define VFUNCR8(funcname, index, rettype, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)\
__forceinline rettype funcname(arg1 a1, arg2 a2, arg3 a3, arg4 a4, arg5 a5, arg6 a6, arg7 a7, arg8 a8){\
typedef rettype(__thiscall* F)(void*, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);\
return call_vfunc<F>(this, index)(this, a1, a2, a3, a4, a5, a6, a7, a8);\
}

#define VFUNCR9(funcname, index, rettype, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)\
__forceinline rettype funcname(arg1 a1, arg2 a2, arg3 a3, arg4 a4, arg5 a5, arg6 a6, arg7 a7, arg8 a8, arg9 a9){\
typedef rettype(__thiscall* F)(void*, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);\
return call_vfunc<F>(this, index)(this, a1, a2, a3, a4, a5, a6, a7, a8, a9);\
}