#include "generator.h"
#include "@@pathToQReal@@/qrutils/nameNormalizer.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace generators::@@generatorName@@;
using namespace qReal;
using namespace utils;

/// Generation target file
QString const templateDir = "./templates/@@generatorName@@";
@@templateConstStringNames@@

Generator::Generator(QString const &outputDirPath
		, QString const &programName
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		, QMap<QString, QString> marksProperty
		)
	: AbstractGenerator(templateDir, QString(outputDirPath + "/" + programName).replace("\\", "/"), logicalModel, errorReporter)
	, mProgramName(programName)
{
	foreach (QString key, marksProperty.keys()) {
		mTemplateUtils[key] = marksProperty[key];
	}
}

Generator::~Generator()
{
}

bool Generator::isCorrectedName(QString const &name)
{
	QRegExp patten;
	patten.setPattern("[A-Za-z]+([A-Za-z_0-9]*)");
	return patten.exactMatch(name);
}

void Generator::generate()
{
	mErrorReporter.clear();
	mErrorReporter.clearErrors();

	if (!isCorrectedName(mProgramName)) {
		mErrorReporter.addError("Program name is not correct");
		return;
	}
	mTemplateUtils["@@@@programNameProperty@@@@"] = mProgramName;
	QString result_Links = "";
@@generateBodyForLinks@@
	QString result = "";
@@generateBody@@
	mTemplateUtils["@@elements_ALL@@"] = result + "\n" + result_Links;
@@generateBody_replaceTemplates@@
}
