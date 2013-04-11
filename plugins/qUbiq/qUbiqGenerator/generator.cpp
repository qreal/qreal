#include "generator.h"
#include "../../../qrutils/nameNormalizer.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace qUbiq::qUbiqGenerator;
using namespace qReal;
using namespace utils;

// QString templateDir = "./templates"; // ?

/// Generation target file

Generator::Generator(QString const &templateDirPath
		, QString const &outputDirPath
		, QString const &pathToQReal
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		, QString const &diagramName
		)
	: AbstractGenerator(templateDirPath, outputDirPath, logicalModel, errorReporter)
	, mPathToQReal(pathToQReal), mDiagramName(diagramName)
{
	mPathToQReal.replace("\\", "/");
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
	// qwerty_generate
}

