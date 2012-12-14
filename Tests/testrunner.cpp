/*
 * testrunner.cpp
 *
 *  Created on: 14.12.2012
 *      Author: philip
 */
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/XmlOutputter.h>
#include <iostream>

using namespace CppUnit;
using namespace std;


int main(int argc, char** argv) {

	// Create the event manager and test controller
	TestResult controller;

	// Add a listener that colllects test result
	TestResultCollector result;
	controller.addListener(&result);

	// Add a listener that print dots as test run.
	BriefTestProgressListener progress;
	controller.addListener(&progress);

	TextUi::TestRunner runner;
	TestFactoryRegistry &registry = TestFactoryRegistry::getRegistry();
	runner.addTest(registry.makeTest());

	std::cout << std::endl << " === Running Tests " << std::endl;

	runner.run(controller);

	// Print test in a compiler compatible format.
	std::cout << std::endl << " === Test Results" << std::endl;

	CompilerOutputter outputter(&result, stdCOut());
	outputter.write();

	ofstream xmlFile("Tests/data/cpptestresults.xml");
	XmlOutputter xmlOut(&result, xmlFile);
	xmlOut.write();



	return result.wasSuccessful() ? 0 : 1;
}
