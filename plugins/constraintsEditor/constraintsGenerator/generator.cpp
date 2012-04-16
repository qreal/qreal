#include "generator.h"
#include <QtCore/QDir>
#include "concreateGenerator.h"

using namespace constraints::generator;

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

void Generator::generate()
{
	qDebug() << "count = " << mLogicalModel->logicalRepoApi().elementsCount();
	qReal::IdList ch = mLogicalModel->logicalRepoApi().children(qReal::Id::rootId());
	foreach (qReal::Id id, ch) {
		qDebug() << id;
	}

	foreach (qReal::Id const &metamodel, mLogicalModel->logicalRepoApi().elementsByType("MetamodelConstraints")) {
		if (!mLogicalModel->logicalRepoApi().isLogicalElement(metamodel)) {
			continue;
		}
		qDebug() << "generate : " << mLogicalModel->propertyByRoleName(metamodel, "outputDirPath").toString();
		ConcreateGenerator generator(templateDir, mLogicalModel->propertyByRoleName(metamodel, "outputDirPath").toString()
									, *mLogicalModel, *mErrorReporter, mLogicalModel->propertyByRoleName(metamodel, "metamodelName").toString());
		generator.generate();
	}
}
