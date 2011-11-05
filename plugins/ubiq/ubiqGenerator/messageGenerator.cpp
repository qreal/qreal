#include "messageGenerator.h"

#include <QtCore/QDebug>

using namespace ubiq::generator;

MessageGenerator::MessageGenerator(QString const &templateFilePath, qReal::LogicalModelAssistInterface const &logicalModel)
		: mApi(logicalModel.logicalRepoApi())
		, mTemplateFilePath(templateFilePath)
{
}

MessageGenerator::~MessageGenerator()
{
}

void MessageGenerator::generate()
{
	qDebug() << "ffffuuuuu~";
}
