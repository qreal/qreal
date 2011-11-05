#pragma once

#include "../../../qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"
#include "../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

namespace ubiq {
namespace generator {

class MessageGenerator
{
public:
	MessageGenerator(QString const &templateFilePath
			, QString const &outputFilePath
			, qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);

	virtual ~MessageGenerator();

	void generate();

private:
	QString const mTemplateFilePath;
	QString const mOutputFilePath;
	qrRepo::LogicalRepoApi const &mApi;
	qReal::ErrorReporterInterface &mErrorReporter;
};

}
}
