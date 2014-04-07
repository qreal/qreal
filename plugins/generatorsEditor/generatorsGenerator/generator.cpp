#include "generator.h"
#include <QtCore/QDir>
#include "behavioralGenerator.h"
#include "structuralGenerator.h"

using namespace generators::generator;

QString templateDir = "./templates";

Generator::Generator()
{
}

Generator::~Generator()
{
}

void Generator::init(qReal::LogicalModelAssistInterface const &logicalModel
					, qReal::ErrorReporterInterface &errorReporter)
{
	mLogicalModel = &logicalModel;
	mErrorReporter = &errorReporter;
}

bool Generator::isCorrectedName(QString const &name)
{
	QRegExp patten;
	patten.setPattern("[A-Za-z]+([A-Za-z_0-9]*)");
	if (patten.exactMatch(name)) {
		return true;
	} else {
		return false;
	}
}

void Generator::generate(qReal::Id const &metamodel)
{
	mErrorReporter->clear();
	mErrorReporter->clearErrors();
	QString outputDirPath = mLogicalModel->propertyByRoleName(metamodel, "outputDirPath").toString();
	if (outputDirPath.isEmpty()) {
		mErrorReporter->addCritical(tr("OutputDirPath of generator model not found"), metamodel);
		return;
	}
	qDebug() << "generate : " << outputDirPath;

	QString generatorMetamodelName = mLogicalModel->logicalRepoApi().name(metamodel);
	if (generatorMetamodelName.isEmpty() || (generatorMetamodelName.compare("(Generator Root)") == 0) ) {
		mErrorReporter->addCritical(tr("Name of generators model not found.\n"), metamodel);
		return;
	} else if (!isCorrectedName(generatorMetamodelName)) {
		mErrorReporter->addCritical(tr("Name of generator model is not correct.\n"), metamodel);
		return;
	}

	QString metamodelName = mLogicalModel->propertyByRoleName(metamodel, "metamodelName").toString();
	if (metamodelName.isEmpty()) {
		mErrorReporter->addCritical(tr("MetamodelName of generator model not found"), metamodel);
		return;
	}

	QString pathToQReal = mLogicalModel->propertyByRoleName(metamodel, "pathToQReal").toString();
	if (pathToQReal.isEmpty()) {
		mErrorReporter->addCritical(tr("PathToQReal of generator model not found"), metamodel);
		return;
	}

	outputDirPath.replace("\\", "/");

	QString languageType = mLogicalModel->propertyByRoleName(metamodel, "languageType").toString();

	MyAbstractGenerator *generator;

	if (languageType.compare("behavioral") == 0) {
		generator = new BehavioralGenerator(templateDir, outputDirPath
										   , pathToQReal, *mLogicalModel, *mErrorReporter
										   , metamodelName, generatorMetamodelName);
	} else if (languageType.compare("structural") == 0) {
		generator = new StructuralGenerator(templateDir, outputDirPath
										   , pathToQReal, *mLogicalModel, *mErrorReporter
										   , metamodelName, generatorMetamodelName);
	} else {
		mErrorReporter->addCritical(tr("I don't know this type of languages yet\n"), metamodel);
		return;
	}

	generator->generate();
	mGeneratorModelFullName = generator->generatorModelFullName();
	mGeneratorModelName = generator->generatorGeneratorModelName();
	mGeneratorNormalizerGeneratorModelName = generator->generatorNormalizerGeneratorModelName();
	mGeneratorModelId = generator->generatorModelId();
}

QString Generator::generatorModelFullName()
{
	return mGeneratorModelFullName;
}

QString Generator::generatorGeneratorModelName()
{
	return mGeneratorModelName;
}

QString Generator::generatorNormalizerGeneratorModelName()
{
	return mGeneratorNormalizerGeneratorModelName;
}

QString Generator::generatorModelId()
{
	return mGeneratorModelId;
}
