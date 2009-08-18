
#ifndef __TESTCPP_LT_TEST_LISTENER_LOADER_H
#define __TESTCPP_LT_TEST_LISTENER_LOADER_H

#include <testcpp/testcpp.h>

#include <testcpp/runner/TestListenerLoader.h>

TESTCPP_NS_START

struct TestListener;
struct TestRunnerContext;
struct LTTestListenerLoaderImpl;

struct LTTestListenerLoader : public TestListenerLoader
{
   LTTestListenerLoader(const std::string& name);
   ~LTTestListenerLoader();

	void load(TestRunnerContext* context);

private:
   LTTestListenerLoaderImpl* This;
};

TESTCPP_NS_END

#endif

