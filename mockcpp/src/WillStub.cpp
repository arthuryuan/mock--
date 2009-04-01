
#include <OutputStringStream.h>
#include <WillStub.h>

MOCKCPP_NS_START

//////////////////////////////////////////////
WillStub::WillStub(Stub* stub)
	: expectedStub(stub)
{
}

//////////////////////////////////////////////
WillStub::~WillStub()
{
    delete expectedStub;
}

//////////////////////////////////////////////
Any& WillStub::invoke(const Invocation& inv)
{
    return expectedStub->invoke(inv);
}
    
//////////////////////////////////////////////
std::string WillStub::toString() const
{
    oss_t oss;

    oss << ".will(" << expectedStub->toString() << ")";

    return oss.str();
}

MOCKCPP_NS_END


