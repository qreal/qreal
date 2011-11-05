#include "messageGenerator.h"

#include <QtCore/QFile>

#include <QtCore/QDebug>

using namespace ubiq::generator;

MessageGenerator::MessageGenerator(QString const &templateFilePath
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
		: mTemplateFilePath(templateFilePath)
		, mApi(logicalModel.logicalRepoApi())
		, mErrorReporter(errorReporter)
{
}

MessageGenerator::~MessageGenerator()
{
}

void MessageGenerator::generate()
{
	QFile templateFile(mTemplateFilePath);
	if (!templateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + templateFile.fileName() + "\"");
		return;
	}

	QTextStream templateStream(&templateFile);
	QString resultString = templateStream.readAll();
	templateFile.close();

	qDebug() << "ffffuuuuu~";
}
