#include "generator.h"
#include <QtCore/QDir>
#include "concreateGenerator.h"

using namespace constraints::generator;

QString templateDir = "./templates";

const QString keywordForAllLanguages = "AllLanguages";//qwerty_asd_copypast

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

void Generator::generate(qReal::Id const &metamodel)
{
	qDebug() << "generate : " << mLogicalModel->propertyByRoleName(metamodel, "outputDirPath").toString();
	QString constraintMetamodelName = mLogicalModel->logicalRepoApi().name(metamodel);
	if (constraintMetamodelName.isEmpty()) {
		mErrorReporter->addCritical("Name of ConstraintModel not found!\n", metamodel);
		return;
	}
	QString metamodelName = mLogicalModel->propertyByRoleName(metamodel, "metamodelName").toString();
	if ((metamodelName.compare("all", Qt::CaseInsensitive) == 0) || (metamodelName.compare(keywordForAllLanguages, Qt::CaseInsensitive) == 0)) {
		metamodelName = keywordForAllLanguages;
	}
	ConcreateGenerator generator(templateDir, mLogicalModel->propertyByRoleName(metamodel, "outputDirPath").toString()
								, mLogicalModel->propertyByRoleName(metamodel, "pathToQReal").toString(), *mLogicalModel, *mErrorReporter
								, metamodelName, constraintMetamodelName);
	generator.generate();
	mConstraintModelFullName = generator.constraintModelFullName();
	mConstraintModelName = generator.constraintModelName();
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

QString Generator::constraintModelId()
{
	return mConstraintModelId;
}
