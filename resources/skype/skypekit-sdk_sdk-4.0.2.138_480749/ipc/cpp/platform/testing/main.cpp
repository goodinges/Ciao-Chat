 
#include <stdlib.h>

#include <cppunit/Test.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>


struct OptionsParser {
	OptionsParser() :
		m_Help(false),
		m_RunSingle(false),
		m_List(false) {
	}
	void Usage(const char *executable) {
		printf("usage:\n");
		printf("	%s [options]\n", executable);
		printf("	options:\n");
		printf("		-h - print help\n");
		printf("		-l - list tests\n");
		printf("		-t name - run test with [name]\n");
	}
	int ParseOptions(int argc, const char **argv) {
		for (int i = 1; i < argc; i++) {
			const char c = argv[i][0];
			if (c == '-' || c == '/') {
				switch (argv[i][1]) {
				case 'h': {
					m_Help = true;
					break;
				}
				case 'l': {
					m_List = true;
					break;
				}
				case 't': {
					if (i + 1 >= argc)
						return -1;
					m_RunSingle = true;
					m_TestName = argv[++i];
					break;
				}
				default: {
					return -1;
					break;
				}
				}
			} else {
				return -1;
			}
		}

		return 0;
	}
	bool m_RunSingle;
	const char *m_TestName;
	bool m_Help;
	bool m_List;
};


int main(int argc, const char * argv[])
{
	OptionsParser parser;
	if (parser.ParseOptions(argc, argv) < 0 || parser.m_Help) {
		parser.Usage(argv[0]);
		return -1;
	}

	if (parser.m_List) {
		CPPUNIT_NS::Test *test = CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest();
		for (int i = 0; i < test->getChildTestCount(); i++) {
		  printf("test %s\n", test->getChildTestAt(i)->getName().c_str());
			for (int j = 0; j < test->getChildTestAt(i)->getChildTestCount(); j++) {
			  printf("test %s\n", test->getChildTestAt(i)->getChildTestAt(j)->getName().c_str());
			}
		}
		return 0;
	}

	CPPUNIT_NS::TestResult controller;

	// Add a listener that colllects test result
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener( &result );

	// Add a listener that print dots as test run.
	CPPUNIT_NS::BriefTestProgressListener progress;
	controller.addListener( &progress );

	CPPUNIT_NS::TestRunner runner;
	// Add the single test to the test runner
	if (parser.m_RunSingle) {
		runner.addTest(
				CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest()->findTest(parser.m_TestName));
	// Add the top suite to the test runner
	} else {
		runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
	}

	runner.run( controller );

	// Print test in a compiler compatible format.
	CPPUNIT_NS::CompilerOutputter outputter( &result, CPPUNIT_NS::stdCOut() );
	outputter.write();

	return result.wasSuccessful() ? 0 : 1;
}







