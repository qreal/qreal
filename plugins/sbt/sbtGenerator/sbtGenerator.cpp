#include "sbtGenerator.h"
#include "dataObjectGenerator.h"
#include "Qdir"

#include <QtCore/QDebug>

SbtGenerator::SbtGenerator()
{
}

SbtGenerator::~SbtGenerator()
{
}

void SbtGenerator::init(qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
{
	mLogicalModel = &logicalModel;
	mErrorReporter = &errorReporter;
}

void SbtGenerator::generate()
{
	QString path = "plugins/sbt/sbtGenerator";
	if (QDir::currentPath().endsWith("bin")) {
	  path = "../" + path;
	}
	DataObjectGenerator dataObjectGenerator(path + "/template/",
			path + "/output/", *mLogicalModel, *mErrorReporter);
	dataObjectGenerator.generate();

//	CustomClassSbtGenerator customClassSbtGenerator("./templates/", "./output/", *mLogicalModel, *mErrorReporter);
//	customClassSbtGenerator.generate();

//	DispatcherSbtGenerator dispatcherSbtGenerator("./templates", "./output", *mLogicalModel, *mErrorReporter);
//	dispatcherSbtGenerator.generate();
}
