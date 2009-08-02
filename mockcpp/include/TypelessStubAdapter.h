
#ifndef __MOCKCPP_TYPELESS_STUB_ADAPTER_H
#define __MOCKCPP_TYPELESS_STUB_ADAPTER_H

#include <mockcpp.h>
#include <Stub.h>

MOCKCPP_NS_START

class TypelessStub;
class Invocation;

struct TypelessStubAdapter : public Stub
{
    TypelessStubAdapter(TypelessStub* ts);

    ~TypelessStubAdapter();

    bool isCompleted() const;

    Any& invoke(const Invocation& inv);

    std::string toString(void) const;

private:

    TypelessStub* stub;
};

MOCKCPP_NS_END

#endif

