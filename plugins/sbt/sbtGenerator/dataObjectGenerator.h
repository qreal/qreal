#pragma once
#include "QtCore/QString"
#include "QtCore/QList"

#include "../../../qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"
#include "../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"
#include "../../../qrutils/generator/abstractGenerator.h"

namespace sbt {

class DataObjectGenerator : public utils::AbstractGenerator
{
public:
	DataObjectGenerator(QString const &templateDirPath
					, QString const &outputDirPath
					, qReal::LogicalModelAssistInterface const &logicalModel
					, qReal::ErrorReporterInterface &errorReporter
					);

	/// Starts generation process.
	void generate();

	/// Returns list of generated files.
	QList<QString> getFiles();

private:
	QList<QString> mFiles;
};

}
