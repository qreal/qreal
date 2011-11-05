#include "generator.h"

#include <QtCore/QDebug>

#include "messageGenerator.h"
#include "devRecordGenerator.h"

using namespace ubiq::generator;

Generator::Generator()
{
}

Generator::~Generator()
{
}

void Generator::init(qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
{
	mLogicalModel = &logicalModel;
	mErrorReporter = &errorReporter;
}

void Generator::generate()
{
	MessageGenerator generator("./templates/Message.cs", "./output/Message.cs", *mLogicalModel, *mErrorReporter);
	generator.generate();

	CustomClassGenerator customClassGenerator("./templates/", "./output/", *mLogicalModel, *mErrorReporter);
	customClassGenerator.generate();
}
