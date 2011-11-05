#include "messageGenerator.h"

#include <QtCore/QFile>

#include <QtCore/QDebug>

using namespace ubiq::generator;

MessageGenerator::MessageGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
		: AbstractGenerator(templateDirPath, outputDirPath, logicalModel, errorReporter)
{
}

MessageGenerator::~MessageGenerator()
{
}

void MessageGenerator::generate()
{
	QFile templateFile(mTemplateDirPath);
	if (!templateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + templateFile.fileName() + "\"");
		return;
	}

	QTextStream templateStream(&templateFile);
	QString resultString = templateStream.readAll();
	templateFile.close();

	resultString.replace("@@TEST@@", "FFFFFFFUUuuuUUuuuUUuu~");

	QFile resultFile(mOutputDirPath);
	if (!resultFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		mErrorReporter.addError("cannot open \"" + resultFile.fileName() + "\"");
		return;
	}

	QTextStream out(&resultFile);
	out << resultString;
	out.flush();
	resultFile.close();
}
