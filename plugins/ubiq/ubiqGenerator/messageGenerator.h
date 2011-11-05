#pragma once

#include "../../../qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"
#include "../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

namespace ubiq {
namespace generator {

class MessageGenerator
{
public:
	MessageGenerator(QString const &templateFilePath
			, qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);
	virtual ~MessageGenerator();

	void generate();

private:
	QString const mTemplateFilePath;
	qReal::LogicalModelAssistInterface const &mModel;
	qReal::ErrorReporterInterface &mErrorReporter;
};

}
}
