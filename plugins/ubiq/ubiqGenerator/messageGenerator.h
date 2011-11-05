#pragma once

#include "../../../qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"

namespace ubiq {
namespace generator {

class MessageGenerator
{
public:
	MessageGenerator(QString const &templateFilePath, qReal::LogicalModelAssistInterface const &logicalModel);
	virtual ~MessageGenerator();

	void generate();

private:
	qReal::LogicalModelAssistInterface const &mModel;
	QString const mTemplateFilePath;
};

}
}
