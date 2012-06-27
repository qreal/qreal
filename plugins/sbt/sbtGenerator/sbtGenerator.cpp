#include "sbtGenerator.h"
#include "dataObjectGenerator.h"

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
	DataObjectGenerator dataObjectGenerator("./templates", "./output", *mLogicalModel, *mErrorReporter);
	dataObjectGenerator.generate();

//	CustomClassSbtGenerator customClassSbtGenerator("./templates/", "./output/", *mLogicalModel, *mErrorReporter);
//	customClassSbtGenerator.generate();

//	DispatcherSbtGenerator dispatcherSbtGenerator("./templates", "./output", *mLogicalModel, *mErrorReporter);
//	dispatcherSbtGenerator.generate();
}
