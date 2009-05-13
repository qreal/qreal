#include <QCoreApplication>

#include "generator.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc,argv);
	Generator g;
	bool ret = true;

	g.setSrcDir("editors");
	// FIXME: get  via args
	ret = ret && g.loadFile("kernel_metamodel.xml");
	ret = ret && g.loadFile("basicbehaviors_metamodel.xml");
	ret = ret && g.loadFile("trace.xml");
	ret = ret && g.loadFile("requirements_metamodel.xml");
	ret = ret && g.loadFile("class_metamodel.xml");
	ret = ret && g.loadFile("usecase_metamodel.xml");
	ret = ret && g.loadFile("sequence_metamodel.xml");
	ret = ret && g.loadFile("communication_metamodel.xml");
	ret = ret && g.loadFile("statemachines_metamodel.xml");
	ret = ret && g.loadFile("deployment.xml");
	ret = ret && g.loadFile("component_metamodel.xml");
	ret = ret && g.loadFile("timing_metamodel.xml");
	ret = ret && g.loadFile("activity_metamodel.xml");
	ret = ret && g.loadFile("package.xml");
	ret = ret && g.loadFile("bpel.xml");
	ret = ret && g.loadFile("qrealparallel_metamodel.xml");
//	ret = ret && g.loadFile("orthodox.xml");
	ret = ret && g.loadFile("meta_editor.xml");
//	ret = ret && g.loadFile("NEW_Meta_editor.xml");

	if (!ret)
		return 1; // Failure

	if (!g.generate())
		return 1; // Failure
	return 0; // Success
}
