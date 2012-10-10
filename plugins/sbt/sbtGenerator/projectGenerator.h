#pragma once
#include "QtCore/QString"
#include "QtCore/QList"

#include "../../../qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"
#include "../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"
#include "../../../qrutils/generator/abstractGenerator.h"

namespace sbt {

class ProjectGenerator : public utils::AbstractGenerator
{
public:
	ProjectGenerator(QString const &templateDirPath
			, QString const &outputDirPath
			, qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);

	/// Starts generation process.
	void generate();

	/// Adds files to project.
	void setFilesName(QList<QString> const & files);

private:
	QString mFiles;
};

}

