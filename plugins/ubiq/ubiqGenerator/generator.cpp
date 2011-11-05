#include "generator.h"

#include <QtCore/QDebug>

#include "messageGenerator.h"

using namespace ubiq::generator;

Generator::Generator()
{
}

Generator::~Generator()
{
}

void Generator::generate()
{
	MessageGenerator generator("./templates/Message.cs", *mLogicalModel);
	generator.generate();
}

void Generator::init(qReal::LogicalModelAssistInterface const &logicalModel)
{
	mLogicalModel = &logicalModel;
}
