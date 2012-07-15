#include "sbtGenerator.h"
#include "dataObjectGenerator.h"
#include "Qdir"
#include "dataIntegratorGenerator.h"
#include "projectGenerator.h"

#include <QtCore/QDebug>

SbtGenerator::SbtGenerator()
{
}

SbtGenerator::~SbtGenerator()
{
}

void SbtGenerator::init(qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
{
	mLogicalModel = &logicalModel;
	mErrorReporter = &errorReporter;
}

void SbtGenerator::generate()
{
	QString path = ".";
	QList<QString> files;
	DataObjectGenerator dataObjectGenerator(path + "/template/",
			path + "/output/", *mLogicalModel, *mErrorReporter);
	dataObjectGenerator.generate();
	files.append(dataObjectGenerator.getFiles());
	DataIntegratorGenerator dataIntegratorGenerator(path + "/template/",
			path + "/output/", *mLogicalModel, *mErrorReporter);
	dataIntegratorGenerator.generate();
	files.append(dataIntegratorGenerator.getFiles());
	ProjectGenerator prGen(path + "/template/",
			path + "/output/", *mLogicalModel, *mErrorReporter);
	prGen.setFilesName(files);
	prGen.generate();
}
