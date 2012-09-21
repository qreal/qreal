#pragma once
#include "QtCore/QString"
#include "QtCore/QList"

#include "../../../qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"
#include "../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"
#include "../../../qrutils/generator/abstractGenerator.h"

namespace sbt {

class DataIntegratorGenerator : public utils::AbstractGenerator
{
public:
	DataIntegratorGenerator(QString const &templateDirPath
					, QString const &outputDirPath
					, qReal::LogicalModelAssistInterface const &logicalModel
					, qReal::ErrorReporterInterface &errorReporter
					);

	/// Returns list of generated files for all ORMDataIntegrator elements.
	QList<QString> getFiles();

	/// Starts generation process.
	void generate();

private:
	QList<QString> mFiles;
};

}

