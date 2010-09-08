
#ifndef __MOCKCPP_C_API_HOOK_H__
#define __MOCKCPP_C_API_HOOK_H__

#include <mockcpp/mockcpp.h>
#include <mockcpp/ApiHook.h>

MOCKCPP_NS_START

struct Arch32ApiHook;

struct CApiHook
{
	CApiHook(ApiHook::Address pfnOld, ApiHook::Address pfnNew);
	~CApiHook();

private:
	Arch32ApiHook* hooker;
};

MOCKCPP_NS_END

#endif
