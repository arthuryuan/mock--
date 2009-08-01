
#ifndef __MOCKCPP_METHOD_TYPE_TRAITS_H
#define __MOCKCPP_METHOD_TYPE_TRAITS_H

#include <mockcpp.h>

#include <ArgumentsList.h>
#include <ArgumentsMacroHelpers.h>

MOCKCPP_NS_START

template <class C, typename Method>
struct MethodTypeTraits;

#define MOCKCPP_DELEGATED_METHOD_DEF(n) \
template <int VPTRIndex, int VTBLIndex, typename R DECL_TEMPLATE_ARGS(n)> \
struct DelegatedMethod<VPTRIndex, VTBLIndex, ArgumentsList<R DECL_REST_ARGS(n)> > \
{ \
   R operator()(DECL_PARAM_LIST(n)) \
   { \
      return R(); \
   } \
}

#define MOCKCPP_METHOD_TYPE_TRAITS_DEF(n) \
template <class C, class C1, typename R DECL_TEMPLATE_ARGS(n)> \
struct MethodTypeTraits<C, R (C1::*) (DECL_ARGS(n))> \
{ \
   typedef C1 OrignalClass; \
   typedef R (C::*MethodType)(DECL_ARGS(n)); \
 \
   typedef ArgumentsList<R, DECL_REST_ARGS(n)> ArgumentsList; \
}

#define MOCKCPP_CONST_METHOD_TYPE_TRAITS_DEF(n) \
template <class C, class C1, typename R DECL_TEMPLATE_ARGS(n)> \
struct MethodTypeTraits<C, R (C1::*) (DECL_ARGS(n)) const> \
{ \
   typedef C1 OrignalClass; \
   typedef R (C::*MethodType)(DECL_ARGS(n)) const; \
 \
   typedef ArgumentsListConst<R, DECL_REST_ARGS(n)> ArgumentsList; \
}

MOCKCPP_METHOD_TYPE_TRAITS_DEF(0);
MOCKCPP_CONST_METHOD_TYPE_TRAITS_DEF(0);

MOCKCPP_METHOD_TYPE_TRAITS_DEF(1);
MOCKCPP_CONST_METHOD_TYPE_TRAITS_DEF(1);

MOCKCPP_METHOD_TYPE_TRAITS_DEF(2);
MOCKCPP_CONST_METHOD_TYPE_TRAITS_DEF(2);

MOCKCPP_METHOD_TYPE_TRAITS_DEF(3);
MOCKCPP_CONST_METHOD_TYPE_TRAITS_DEF(3);

MOCKCPP_METHOD_TYPE_TRAITS_DEF(4);
MOCKCPP_CONST_METHOD_TYPE_TRAITS_DEF(4);

MOCKCPP_METHOD_TYPE_TRAITS_DEF(5);
MOCKCPP_CONST_METHOD_TYPE_TRAITS_DEF(5);

MOCKCPP_METHOD_TYPE_TRAITS_DEF(6);
MOCKCPP_CONST_METHOD_TYPE_TRAITS_DEF(6);

#if MOCKCPP_MAX_PARAMETERS > 6
MOCKCPP_METHOD_TYPE_TRAITS_DEF(7);
MOCKCPP_CONST_METHOD_TYPE_TRAITS_DEF(7);
#endif

#if MOCKCPP_MAX_PARAMETERS > 7
MOCKCPP_METHOD_TYPE_TRAITS_DEF(8);
MOCKCPP_CONST_METHOD_TYPE_TRAITS_DEF(8);
#endif

#if MOCKCPP_MAX_PARAMETERS > 8
MOCKCPP_METHOD_TYPE_TRAITS_DEF(9);
MOCKCPP_CONST_METHOD_TYPE_TRAITS_DEF(9);
#endif

#if MOCKCPP_MAX_PARAMETERS > 9
MOCKCPP_METHOD_TYPE_TRAITS_DEF(10);
MOCKCPP_CONST_METHOD_TYPE_TRAITS_DEF(10);
#endif

#if MOCKCPP_MAX_PARAMETERS > 10
MOCKCPP_METHOD_TYPE_TRAITS_DEF(11);
MOCKCPP_CONST_METHOD_TYPE_TRAITS_DEF(11);
#endif

#if MOCKCPP_MAX_PARAMETERS > 11
MOCKCPP_METHOD_TYPE_TRAITS_DEF(12);
MOCKCPP_CONST_METHOD_TYPE_TRAITS_DEF(12);
#endif

#if MOCKCPP_MAX_PARAMETERS > 12
MOCKCPP_METHOD_TYPE_TRAITS_DEF(13);
MOCKCPP_CONST_METHOD_TYPE_TRAITS_DEF(13);
#endif

#if MOCKCPP_MAX_PARAMETERS > 13
MOCKCPP_METHOD_TYPE_TRAITS_DEF(14);
MOCKCPP_CONST_METHOD_TYPE_TRAITS_DEF(14);
#endif

#if MOCKCPP_MAX_PARAMETERS > 14
MOCKCPP_METHOD_TYPE_TRAITS_DEF(15);
MOCKCPP_CONST_METHOD_TYPE_TRAITS_DEF(15);
#endif

MOCKCPP_NS_END

#endif