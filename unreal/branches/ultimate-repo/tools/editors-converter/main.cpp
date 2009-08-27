#include <QtCore/QCoreApplication>
#include <QtCore>
#include "converter.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	QStringList editors;
	editors
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
	<< "orthodox.xml"
	<< "bpel.xml"
	<< "qrealparallel_metamodel.xml";
	new converter::Converter(editors);

	return 0;
}
