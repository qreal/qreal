#include <QtCore/QFile>
#include <QtCore/QDebug>

#include "dispatcherGenerator.h"
#include "nameNormalizer.h"

using namespace ubiq::generator;
using namespace qReal;

DispatcherGenerator::DispatcherGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
		: AbstractGenerator(templateDirPath , outputDirPath, logicalModel, errorReporter)
{
}

DispatcherGenerator::~DispatcherGenerator()
{
}


void DispatcherGenerator::generate()
{

}

QString DispatcherGenerator::generateConstants(qReal::Id const &element) const
{
	QString result;
	foreach (Id const id, mApi.children(element)) {
		if (!mApi.isLogicalElement(id) || id.element() != "MasterDiagramConstant")
			continue;

		QString constantTemplate = mTemplateUtils["@@MasterDiagramConstant@@"];
		QString const name = mApi.name(id);
		QString const type = mApi.stringProperty(id, "type");
		QString const value = mApi.stringProperty(id, "value");
		constantTemplate.replace("@@Name@@", NameNormalizer::normalize(name))
				.replace("@@Type@@", type);
				.replace("@@Value@@", value);

		result += constantTemplate;
	}
	return result;
}

QString DispatcherGenerator::generateFields(qReal::Id const &element) const
{
	QString result;
	foreach (Id const id, mApi.children(element)) {
		if (!mApi.isLogicalElement(id) || id.element() != "MasterDiagramField")
			continue;

		QString constantTemplate = mTemplateUtils["@@MasterDiagramField@@"];
		QString const name = mApi.name(id);
		QString const type = mApi.stringProperty(id, "type");
		QString const value = mApi.stringProperty(id, "value");
		constantTemplate.replace("@@Name@@", NameNormalizer::normalize(name))
				.replace("@@Type@@", type);
				.replace("@@Value@@", value);

		result += constantTemplate;
	}
	return result;
}
