#include "devRecordGenerator.h"

#include <QtCore/QFile>

#include <QtCore/QDebug>

using namespace ubiq::generator;
using namespace qReal;

DevRecordGenerator::DevRecordGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
		: AbstractGenerator(templateDirPath , outputDirPath, logicalModel, errorReporter)
{
}

DevRecordGenerator::~DevRecordGenerator()
{
}

void DevRecordGenerator::generate()
{
	loadTemplateFromFile(fileName, mFileTemplate);
	loadTemplateUtils();
	qDebug() << mFileTemplate;

	foreach (Id const diagram, mApi.elementsByType("DataStructuresDiagram")){ // for each diagram
		if (!mApi.isLogicalElement(diagram))
			continue;

		// find DevRecord

		foreach (Id const element, mApi.children(diagram)){
			if (!mApi.isLogicalElement(element) || (element.element() != "CustomClass"))
				continue;


			qDebug() << element.toString();
			QString propertyTemplate = mTemplateUtils["@@Property@@"];
			propertyTemplate.replace("@Name@", mApi.name(element));



			// save into file

			QDir dir;

			if (!dir.exists(mOutputDirPath))
				dir.mkdir(mOutputDirPath);
			dir.cd(mOutputDirPath);

			QString const outputFileName = dir.absoluteFilePath(fileName);
			QFile file(outputFileName);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
				qDebug() << "cannot open \"" << outputFileName << "\"";
				return;
			}

			QTextStream out(&file);
			QString projectTemplate = mFileTemplate;
			out << projectTemplate.replace("@@Properties@@", "ololo");
			file.close();


		}
	}




}
