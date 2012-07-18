#include "generator.h"
#include <QtCore/QDir>
#include "concreateGenerator.h"

using namespace constraints::generator;

QString templateDir = "./templates";

const QString keywordForAllMetamodels = "AllMetamodels";//qwerty_asd_copypast

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
	ConcreateGenerator generator(templateDir, outputDirPath
								 , pathToQReal, *mLogicalModel, *mErrorReporter
								 , metamodelName, constraintMetamodelName);
	generator.generate();
	mConstraintModelFullName = generator.constraintModelFullName();
	mConstraintModelName = generator.constraintModelName();
	mConstraintNormalizerModelName = generator.constraintNormalizerModelName();
	mConstraintModelId = generator.constraintModelId();
}

QString Generator::constraintModelFullName()
{
	return mConstraintModelFullName;
}

QString Generator::constraintModelName()
{
	return mConstraintModelName;
}

QString Generator::constraintNormalizerModelName()
{
	return mConstraintNormalizerModelName;
}

QString Generator::constraintModelId()
{
	return mConstraintModelId;
}
