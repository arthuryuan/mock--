
#ifndef __MOCKCPP_CHAINABLE_MOCK_METHOD_NAME_KEY_H
#define __MOCKCPP_CHAINABLE_MOCK_METHOD_NAME_KEY_H

#include <mockcpp.h>

#include <ChainableMockMethodKey.h>
#include <MethodNameGetter.h>

MOCKCPP_NS_START

struct ChainableMockMethodNameKey
    : public ChainableMockMethodKey
    , public MethodNameGetter
{
   ChainableMockMethodNameKey(const std::string& name);

   bool equals(const ChainableMockMethodKey * const rhs) const;

   std::string& getMethodName() const;

private:
   std::string methodName;
};

MOCKCPP_NS_END

#endif

