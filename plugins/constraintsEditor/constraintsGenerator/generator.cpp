#include "generator.h"
#include <QtCore/QDir>
#include "concreateGenerator.h"

using namespace constraints::generator;

QString templateDir = "./templates";

const QString keywordForAllLanguages = "AllLanguages";//asd_copypast

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
	QString metamodelName = mLogicalModel->propertyByRoleName(metamodel, "metamodelName").toString();
	if ((metamodelName.compare("all", Qt::CaseInsensitive) == 0) || (metamodelName.compare(keywordForAllLanguages, Qt::CaseInsensitive) == 0)) {
		metamodelName = keywordForAllLanguages;
	}
	ConcreateGenerator generator(templateDir, mLogicalModel->propertyByRoleName(metamodel, "outputDirPath").toString()
								 , *mLogicalModel, *mErrorReporter, metamodelName, mLogicalModel->propertyByRoleName(metamodel, "name").toString());
	generator.generate();
	mConstraintModelFullName = generator.constraintModelFullName();
}

QString Generator::constraintModelFullName()
{
	return mConstraintModelFullName;
}
