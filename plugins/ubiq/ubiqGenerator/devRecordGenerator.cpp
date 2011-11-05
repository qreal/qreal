#include <QtCore/QFile>
#include <QtCore/QDebug>

#include "devRecordGenerator.h"
#include "nameNormalizer.h"

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
	QString properties;

	foreach (Id const diagram, mApi.elementsByType("DataStructuresDiagram")){ // for each diagram
		if (!mApi.isLogicalElement(diagram))
			continue;

		// find DevRecord

		foreach (Id const element, mApi.children(diagram)){
			if (!mApi.isLogicalElement(element) || (element.element() != "CustomClass"))
				continue;

			// find properties

			foreach (Id const property, mApi.children(element)){

				// generate property code

				qDebug() << property.toString();
				QString propertyTemplate = mTemplateUtils["@@Property@@"];
				QString name = mApi.name(property);
				propertyTemplate.replace("@Name@", NameNormalizer::normalize(name, false))
						.replace("@NameCaps@", NameNormalizer::normalize(name))
						.replace("@Type@", mApi.stringProperty(property, "type"));


				properties += propertyTemplate;
			}

		}
	}


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
	QString projectTemplate = mFileTemplate.replace("@@Properties@@", properties).replace("\\n", "\n");
	out << projectTemplate.replace("@@Properties@@", "ololo");
	file.close();

}
