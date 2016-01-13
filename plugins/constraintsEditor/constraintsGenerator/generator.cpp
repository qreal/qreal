#include "generator.h"
#include <QtCore/QDir>
#include "concreteGenerator.h"

using namespace constraints::generator;

QString templateDir = "../templates"; // TODO_CONSTRAINTS: check all paths

const QString keywordForAllMetamodels = "AllMetamodels"; //qwerty_asd_copypast

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
	QRegExp pattern;
	pattern.setPattern("[A-Za-z]+([A-Za-z_0-9]*)");
	return (pattern.exactMatch(name));
}

void Generator::generate(qReal::Id const &metamodel)
{
	mErrorReporter->clear();
	mErrorReporter->clearErrors();

	mBuildConfiguration = mLogicalModel->propertyByRoleName(metamodel, "buildConfiguration").toString();

	QString outputDirPath = mLogicalModel->propertyByRoleName(metamodel, "outputDirPath").toString();
	if (outputDirPath.isEmpty()) {
		mErrorReporter->addCritical(tr("OutputDirPath of constraints model not found"), metamodel);
		return;
	}

	qDebug() << "generate : " << outputDirPath;

	QString constraintMetamodelName = mLogicalModel->logicalRepoApi().name(metamodel);
	if (constraintMetamodelName.isEmpty() || (constraintMetamodelName.compare("(Metamodel Constraints)") == 0) ) {
		mErrorReporter->addCritical(tr("Name of constraints model not found.\n"), metamodel);
		return;
	} else if (!isCorrectedName(constraintMetamodelName)) {
		mErrorReporter->addCritical(tr("Name of constraints model is not correct.\n"), metamodel);
		return;
	}

	QString metamodelName = mLogicalModel->propertyByRoleName(metamodel, "metamodelName").toString();
	if (metamodelName.isEmpty()) {
		mErrorReporter->addCritical(tr("MetamodelName of constraints model not found"), metamodel);
		return;
	}
	if ((metamodelName.compare("all", Qt::CaseInsensitive) == 0) || (metamodelName.compare(keywordForAllMetamodels, Qt::CaseInsensitive) == 0)) {
		metamodelName = keywordForAllMetamodels;
	}

	QString pathToQReal = mLogicalModel->propertyByRoleName(metamodel, "pathToQReal").toString();
	if (pathToQReal.isEmpty()) {
		mErrorReporter->addCritical(tr("PathToQReal of constraints model not found"), metamodel);
		return;
	}

	outputDirPath.replace("\\", "/");
	ConcreteGenerator generator(templateDir, outputDirPath
			, pathToQReal, *mLogicalModel, *mErrorReporter
			, metamodelName, constraintMetamodelName);
	generator.generate();
	mConstraintModelFullName = generator.constraintModelFullName();
	mConstraintConstraintsModelName = generator.constraintConstraintsModelName();
	mConstraintNormalizerConstraintsModelName = generator.normalizedConstraintConstraintsModelName();
	mConstraintModelId = generator.constraintModelId();
}

QString Generator::constraintModelFullName()
{
	return mConstraintModelFullName;
}

QString Generator::constraintConstraintsModelName()
{
	return mConstraintConstraintsModelName;
}

QString Generator::constraintNormalizerConstraintsModelName()
{
	return mConstraintNormalizerConstraintsModelName;
}

QString Generator::constraintModelId()
{
	return mConstraintModelId;
}

QString Generator::buildConfiguration()
{
	return mBuildConfiguration;
}
