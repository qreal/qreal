#include <QtGui/QApplication>

#include "generator.h"

int main(int argc, char *argv[])
{
	QApplication app(argc,argv);

	QStringList l;
	l
	<< "kernel_metamodel.xml" 
	<< "basicbehaviors_metamodel.xml" 
	<< "trace.xml"
	<< "requirements_metamodel.xml" 
	<< "class_metamodel.xml"
	<< "usecase_metamodel.xml"
	<< "sequence_metamodel.xml"
	<< "communication_metamodel.xml"
	<< "statemachines_metamodel.xml"
	<< "deployment.xml"
	<< "component_metamodel.xml"
	<< "timing_metamodel.xml"
	<< "activity_metamodel.xml"
	<< "package.xml"
	// << "orthodox.xml"
	<< "bpel.xml"
	<< "qrealparallel_metamodel.xml";
	new Generator(l);
	return 0;
}
