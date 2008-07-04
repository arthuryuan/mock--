
#ifndef __MOCKCPP_PROC_STUB_H
#define __MOCKCPP_PROC_STUB_H

#include <Stub.h>
#include <Asserter.h>
#include <RefAny.h>
#include <Any.h>
#include <Invocation.h>
#include <RepeatMacros.h>

MOCKCPP_NS_START

template <typename F>
struct ProcStub;

class ProcStubBaseImpl;

struct ProcStubBase : public Stub
{
public:

    ProcStubBase(const std::string& name, void* addr);

    ~ProcStubBase();

    std::string toString() const;
    
private:

    ProcStubBaseImpl* This;
};

////////////////////////////////////////////////////
std::string getParameterMismatchString(int n
                , std::string p
                , const Invocation& inv);

////////////////////////////////////////////////////
#define MOCKCPP_CHECK_AND_ASSIGN_PARAMETER(N) \
        MOCKCPP_ASSERT_TRUE_MESSAGE( \
                    getParameterMismatchString(N, TypeString<P##N>::value(), inv) \
                        , any_castable<P##N>(inv.getParameter(N))); \
        P##N p##N = any_cast<P##N>(inv.getParameter(N));
    
#define PROC_STUB_CONS()  \
    ProcStub(Func f, std::string name) \
        : ProcStubBase(name, (void*)f), func(f) \
    {} 

///////////////////////////////////////////////////////////
#define __ARG(n) P ## n
#define END_ARGS(n) __ARG(n)
#define ARGS(n) __ARG(n) ,

#define __PARAM(n) p ## n
#define END_PARAMS(n) __PARAM(n)
#define PARAMS(n) __PARAM(n) ,

#define TEMPLATE_ARG(n) typename __ARG(n)
#define TEMPLATE_ARGS(n) , TEMPLATE_ARG(n)
#define VOID_TEMPLATE_ARGS(n) typename __ARG(n),

#define DECL_TEMPLATE_ARGS(n) SIMPLE_REPEAT(n, TEMPLATE_ARGS)
#define DECL_VOID_TEMPLATE_ARGS(n) REPEAT(n, VOID_TEMPLATE_ARGS, TEMPLATE_ARG)
#define DECL_ARGS(n) REPEAT(n, ARGS, END_ARGS)
#define DECL_PARAMS(n) REPEAT(n, PARAMS, END_PARAMS)

///////////////////////////////////////////////////////////
#define PROC_STUB_DEF(n) \
template <typename R DECL_TEMPLATE_ARGS(n)> \
struct ProcStub<R(DECL_ARGS(n))> : public ProcStubBase \
{ \
    typedef R (*Func)(DECL_ARGS(n)); \
 \
    PROC_STUB_CONS() \
 \
    Any& invoke(const Invocation& inv) \
    { \
        SIMPLE_REPEAT(n, MOCKCPP_CHECK_AND_ASSIGN_PARAMETER); \
 \
        result = func(DECL_PARAMS(n)); \
        return result; \
    } \
 \
private: \
    Func func; \
    Any result; \
}; \
template <typename R DECL_TEMPLATE_ARGS(n)> \
Stub* invoke(R(*f)(DECL_ARGS(n)), const char* name = 0) \
{ \
    return new ProcStub<R(DECL_ARGS(n))>(f, name?name:""); \
}

#define VOID_PROC_STUB_DEF(n) \
template <DECL_VOID_TEMPLATE_ARGS(n)> \
struct ProcStub<void(DECL_ARGS(n))> : public ProcStubBase \
{ \
    typedef void (*Func)(DECL_ARGS(n)); \
 \
    PROC_STUB_CONS() \
 \
    Any& invoke(const Invocation& inv) \
    { \
        SIMPLE_REPEAT(n, MOCKCPP_CHECK_AND_ASSIGN_PARAMETER); \
 \
        func(DECL_PARAMS(n)); \
        return getVoidAny(); \
    } \
 \
private: \
    Func func; \
}; 

/////////////////////////////////////////////////////
VOID_PROC_STUB_DEF(0)
VOID_PROC_STUB_DEF(1)
VOID_PROC_STUB_DEF(2)
VOID_PROC_STUB_DEF(3)
VOID_PROC_STUB_DEF(4)
VOID_PROC_STUB_DEF(5)
VOID_PROC_STUB_DEF(6)
VOID_PROC_STUB_DEF(7)
VOID_PROC_STUB_DEF(8)
VOID_PROC_STUB_DEF(9)
VOID_PROC_STUB_DEF(10)
VOID_PROC_STUB_DEF(11)
VOID_PROC_STUB_DEF(12)

PROC_STUB_DEF(0)
PROC_STUB_DEF(1)
PROC_STUB_DEF(2)
PROC_STUB_DEF(3)
PROC_STUB_DEF(4)
PROC_STUB_DEF(5)
PROC_STUB_DEF(6)
PROC_STUB_DEF(7)
PROC_STUB_DEF(8)
PROC_STUB_DEF(9)
PROC_STUB_DEF(10)
PROC_STUB_DEF(11)
PROC_STUB_DEF(12)

/////////////////////////////////////////////////////

#define PROC(function) function, #function

/////////////////////////////////////////////////////

MOCKCPP_NS_END

#endif
