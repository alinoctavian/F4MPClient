#ifndef F4MPREVERSE_UTILITIES_H
#define F4MPREVERSE_UTILITIES_H

#include <Windows.h>
#include <iostream>
#include "Types.h"
#include "GamePtr.h"

namespace Utilities {

    #pragma warning (push)
    #pragma warning (disable : 4200)
    struct RTTIType
    {
        void	* typeInfo;
        Types::UInt64	data;
        char	name[0];
    };

    struct RTTILocator
    {
        Types::UInt32		sig, offset, cdOffset;
        Types::UInt32		typeDesc;
        Types::UInt32		classDesc;
    };
    #pragma warning (pop)

    const char * GetObjClassName(void * objBase);
    void DumpClass(void * theClassPtr, Types::UInt64 nIntsToDump);

    template <typename T>
    uintptr_t GetFnAddr(T src)
    {
        union
        {
            uintptr_t	u;
            T			t;
        } data;

        data.t = src;

        return data.u;
    }

    class IAT {
    public:
        IAT(void * module,const char * DllName,const char * ImportName) : module(module), DllName(DllName), ImportName(ImportName) {
            base = (Types::UInt8 *)module;
            dosHeader = (IMAGE_DOS_HEADER *)base;
            ntHeader = (IMAGE_NT_HEADERS *)(base + dosHeader->e_lfanew);
            importTable = (IMAGE_IMPORT_DESCRIPTOR *)(base + ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
        };

        void * Get();
    private:
        void * module;
        const char * DllName;
        const char * ImportName;

        Types::UInt8 * base;
        IMAGE_DOS_HEADER * dosHeader;
        IMAGE_NT_HEADERS * ntHeader;
        IMAGE_IMPORT_DESCRIPTOR	* importTable;
    };

    //+++THANKS TO F4SE TEAM
#define MEMBER_FN_PREFIX(className)	\
	typedef className _MEMBER_FN_BASE_TYPE

#define DEFINE_MEMBER_FN_LONG(className, functionName, retnType, address, ...)		\
	typedef retnType (className::* _##functionName##_type)(__VA_ARGS__);			\
																					\
	inline _##functionName##_type * _##functionName##_GetPtr(void)					\
	{																				\
		static uintptr_t _address;													\
		_address = address + Memory::GamePtr_Manager::s_baseAddress;				\
		return (_##functionName##_type *)&_address;									\
	}

#define DEFINE_MEMBER_FN(functionName, retnType, address, ...)	\
	DEFINE_MEMBER_FN_LONG(_MEMBER_FN_BASE_TYPE, functionName, retnType, address, __VA_ARGS__)

#define DEFINE_STATIC_HEAP(staticAllocate, staticFree)						\
	static void * operator new(std::size_t size)							\
	{																		\
		return staticAllocate(size);										\
	}																		\
	static void * operator new(std::size_t size, const std::nothrow_t &)	\
	{																		\
		return staticAllocate(size);										\
	}																		\
	static void * operator new(std::size_t size, void * ptr)				\
	{																		\
		return ptr;															\
	}																		\
	static void operator delete(void * ptr)									\
	{																		\
		staticFree(ptr);													\
	}																		\
	static void operator delete(void * ptr, const std::nothrow_t &)			\
	{																		\
		staticFree(ptr);													\
	}																		\
	static void operator delete(void *, void *)								\
	{																		\
	}

#define CALL_MEMBER_FN(obj, fn)	\
	((*(obj)).*(*((obj)->_##fn##_GetPtr())))


// Using the original implementation does very broken things in a Release build
// For classes like BSGFxShaderFXTarget and GameMenuBase
#define FORCE_INLINE  __forceinline
#define DEFINE_MEMBER_FN_0(fnName, retnType, addr)						\
	FORCE_INLINE retnType fnName() {										\
	struct empty_struct {};													\
	typedef retnType(empty_struct::*_##fnName##_type)();					\
	const static uintptr_t address = addr + Memory::GamePtr_Manager::s_baseAddress;	\
	_##fnName##_type fn = *(_##fnName##_type*)&address;						\
	return (reinterpret_cast<empty_struct*>(this)->*fn)();					\
	}
#define DEFINE_MEMBER_FN_1(fnName, retnType, addr, ...)					\
	template<typename T1>													\
	FORCE_INLINE retnType fnName(T1 && t1) {								\
	struct empty_struct {};													\
	typedef retnType(empty_struct::*_##fnName##_type)(__VA_ARGS__);			\
	const static uintptr_t address = addr + Memory::GamePtr_Manager::s_baseAddress;	\
	_##fnName##_type fn = *(_##fnName##_type*)&address;						\
	return (reinterpret_cast<empty_struct*>(this)->*fn)(t1);				\
	}
#define DEFINE_MEMBER_FN_2(fnName, retnType, addr, ...)					\
	template<typename T1, typename T2>										\
	FORCE_INLINE retnType fnName(T1 && t1, T2 && t2) {						\
	struct empty_struct {};													\
	typedef retnType(empty_struct::*_##fnName##_type)(__VA_ARGS__);			\
	const static uintptr_t address = addr + Memory::GamePtr_Manager::s_baseAddress;	\
	_##fnName##_type fn = *(_##fnName##_type*)&address;						\
	return (reinterpret_cast<empty_struct*>(this)->*fn)(t1, t2);			\
	}
#define DEFINE_MEMBER_FN_3(fnName, retnType, addr, ...)					\
	template<typename T1, typename T2, typename T3>							\
	FORCE_INLINE retnType fnName(T1 && t1, T2 && t2, T3 && t3) {			\
	struct empty_struct {};													\
	typedef retnType(empty_struct::*_##fnName##_type)(__VA_ARGS__);			\
	const static uintptr_t address = addr + Memory::GamePtr_Manager::s_baseAddress;	\
	_##fnName##_type fn = *(_##fnName##_type*)&address;						\
	return (reinterpret_cast<empty_struct*>(this)->*fn)(t1, t2, t3);		\
	}
#define DEFINE_MEMBER_FN_4(fnName, retnType, addr, ...)					\
	template<typename T1, typename T2, typename T3, typename T4>			\
	FORCE_INLINE retnType fnName(T1 && t1, T2 && t2, T3 && t3, T4 && t4) {	\
	struct empty_struct {};													\
	typedef retnType(empty_struct::*_##fnName##_type)(__VA_ARGS__);			\
	const static uintptr_t address = addr + Memory::GamePtr_Manager::s_baseAddress;	\
	_##fnName##_type fn = *(_##fnName##_type*)&address;						\
	return (reinterpret_cast<empty_struct*>(this)->*fn)(t1, t2, t3, t4);	\
	}
#define DEFINE_MEMBER_FN_5(fnName, retnType, addr, ...)								\
	template<typename T1, typename T2, typename T3, typename T4, typename T5>			\
	FORCE_INLINE retnType fnName(T1 && t1, T2 && t2, T3 && t3, T4 && t4, T5 && t5) {	\
	struct empty_struct {};																\
	typedef retnType(empty_struct::*_##fnName##_type)(__VA_ARGS__);						\
	const static uintptr_t address = addr + Memory::GamePtr_Manager::s_baseAddress;				\
	_##fnName##_type fn = *(_##fnName##_type*)&address;									\
	return (reinterpret_cast<empty_struct*>(this)->*fn)(t1, t2, t3, t4, t5);			\
	}
#define DEFINE_MEMBER_FN_6(fnName, retnType, addr, ...)										\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>		\
	FORCE_INLINE retnType fnName(T1 && t1, T2 && t2, T3 && t3, T4 && t4, T5 && t5, T6 && t6) {	\
	struct empty_struct {};																		\
	typedef retnType(empty_struct::*_##fnName##_type)(__VA_ARGS__);								\
	const static uintptr_t address = addr + Memory::GamePtr_Manager::s_baseAddress;						\
	_##fnName##_type fn = *(_##fnName##_type*)&address;											\
	return (reinterpret_cast<empty_struct*>(this)->*fn)(t1, t2, t3, t4, t5, t6);				\
	}
#define DEFINE_MEMBER_FN_7(fnName, retnType, addr, ...)													\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>		\
	FORCE_INLINE retnType fnName(T1 && t1, T2 && t2, T3 && t3, T4 && t4, T5 && t5, T6 && t6, T7 && t7) {	\
	struct empty_struct {};																					\
	typedef retnType(empty_struct::*_##fnName##_type)(__VA_ARGS__);											\
	const static uintptr_t address = addr + Memory::GamePtr_Manager::s_baseAddress;									\
	_##fnName##_type fn = *(_##fnName##_type*)&address;														\
	return (reinterpret_cast<empty_struct*>(this)->*fn)(t1, t2, t3, t4, t5, t6, t7);						\
	}
#define DEFINE_MEMBER_FN_8(fnName, retnType, addr, ...)																\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>	\
	FORCE_INLINE retnType fnName(T1 && t1, T2 && t2, T3 && t3, T4 && t4, T5 && t5, T6 && t6, T7 && t7, T8 && t8) {		\
	struct empty_struct {};																								\
	typedef retnType(empty_struct::*_##fnName##_type)(__VA_ARGS__);														\
	const static uintptr_t address = addr + Memory::GamePtr_Manager::s_baseAddress;												\
	_##fnName##_type fn = *(_##fnName##_type*)&address;																	\
	return (reinterpret_cast<empty_struct*>(this)->*fn)(t1, t2, t3, t4, t5, t6, t7, t8);								\
	}
#define DEFINE_MEMBER_FN_9(fnName, retnType, addr, ...)																			\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>	\
	FORCE_INLINE retnType fnName(T1 && t1, T2 && t2, T3 && t3, T4 && t4, T5 && t5, T6 && t6, T7 && t7, T8 && t8, T9 && t9) {		\
	struct empty_struct {};																											\
	typedef retnType(empty_struct::*_##fnName##_type)(__VA_ARGS__);																	\
	const static uintptr_t address = addr + Memory::GamePtr_Manager::s_baseAddress;															\
	_##fnName##_type fn = *(_##fnName##_type*)&address;																				\
	return (reinterpret_cast<empty_struct*>(this)->*fn)(t1, t2, t3, t4, t5, t6, t7, t8, t9);										\
	}
#define DEFINE_MEMBER_FN_10(fnName, retnType, addr, ...)																						\
	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>	\
	FORCE_INLINE retnType fnName(T1 && t1, T2 && t2, T3 && t3, T4 && t4, T5 && t5, T6 && t6, T7 && t7, T8 && t8, T9 && t9, T10 && t10) {		\
	struct empty_struct {};																														\
	typedef retnType(empty_struct::*_##fnName##_type)(__VA_ARGS__);																				\
	const static uintptr_t address = addr + Memory::GamePtr_Manager::s_baseAddress;																		\
	_##fnName##_type fn = *(_##fnName##_type*)&address;																							\
	return (reinterpret_cast<empty_struct*>(this)->*fn)(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);												\
	}
}

#endif