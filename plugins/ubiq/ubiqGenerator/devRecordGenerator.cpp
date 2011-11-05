#include "devRecordGenerator.h"

#include <QtCore/QDebug>

using namespace ubiq::generator;

DevRecordGenerator::DevRecordGenerator(QString const &templateFilePath, qReal::LogicalModelAssistInterface const &logicalModel)
		: mApi(logicalModel.logicalRepoApi())
		, mTemplateFilePath(templateFilePath)
{
}

DevRecordGenerator::~DevRecordGenerator()
{
}

void DevRecordGenerator::generate()
{

}
