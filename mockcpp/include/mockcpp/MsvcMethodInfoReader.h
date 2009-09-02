/***
    mockcpp is a generic C/C++ mock framework.
    Copyright (C) <2009>  <Darwin Yuan: darwin.yuan@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***/
#ifndef __MOCKCPP_MSVC_METHOD_INFO_READER_H
#define __MOCKCPP_MSVC_METHOD_INFO_READER_H


#include <mockcpp/mockcpp.h>

MOCKCPP_NS_START
 

#ifdef MSVC_VMG_ENABLED

struct MsvcVmgMFP
{
   void* m_func_address; 
   int m_delta;
   int m_vtordisp;
   int m_vtable_index; 
};

template <typename Method>
union MsvcMethodUnion
{
   MsvcVmgMFP mfp;
   Method method;
};

///////////////////////////////////////////////
template <typename Method>
void* msvcGetAddrOfMethodOnly(Method input)
{
	MsvcMethodUnion<Method> m;
	m.method = input;
	return m.mfp.m_func_address;
}

///////////////////////////////////////////////////////////
unsigned int msvcGetIndexOfVirtualMethod(void* p);

void msvcVerifyNonVirtualMethod(void* p);

///////////////////////////////////////////////////////////
template <typename Method>
void* getAddrOfMethod(Method input)
{
   void * p = msvcGetAddrOfMethodOnly(input);
   msvcVerifyNonVirtualMethod(p);
   return p;
}

///////////////////////////////////////////////////////////
template <class C, typename Method>
unsigned int getIndexOfMethod(Method method)
{
	return msvcGetIndexOfVirtualMethod(msvcGetAddrOfMethodOnly(method));
}

///////////////////////////////////////////////////////////
template <class C, typename Method>
MsvcVmgMFP getMsvnVmgMFPOfVirtualMethod(Method input)
{
   typedef typename MethodTypeTraits<C, Method>::MethodType ExpectedMethodType; 
   MsvcMethodUnion<ExpectedMethodType> m;
   m.method = input;

   return m.mfp;
}
///////////////////////////////////////////////////////////
template <class C, typename Method>
unsigned int getDeltaOfMethod(Method method)
{
	return getMsvnVmgMFPOfVirtualMethod<C, Method>(method).m_delta/sizeof(void*);
}

#else

// TODO
template <typename Method>
void* getAddrOfMethod(Method input)
{
	return 0;
}

///////////////////////////////////////////////////////////
// TODO
template <class C, typename Method>
unsigned int getIndexOfMethod(Method method)
{
	return 0;
}

///////////////////////////////////////////////////////////
// TODO
template <class C, typename Method>
unsigned int getDeltaOfMethod(Method method)
{
	return 0;
}
#endif

MOCKCPP_NS_END

#endif