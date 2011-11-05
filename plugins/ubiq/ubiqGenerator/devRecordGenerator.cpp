#include "devRecordGenerator.h"

#include <QtCore/QFile>

#include <QtCore/QDebug>

using namespace ubiq::generator;

DevRecordGenerator::DevRecordGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
		: AbstractGenerator(templateDirPath , outputDirPath, logicalModel, errorReporter)
{
}

DevRecordGenerator::~DevRecordGenerator()
{
}

void DevRecordGenerator::generate()
{
	loadTemplateFromFile("DevRecord.cs", mFileTemplate);
	qDebug() << mFileTemplate;

}
