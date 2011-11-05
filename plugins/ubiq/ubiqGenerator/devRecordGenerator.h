#pragma once

#include "../../../qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"

namespace ubiq {
namespace generator {

class DevRecordGenerator
{
public:
	DevRecordGenerator(QString const &templateFilePath, qReal::LogicalModelAssistInterface const &logicalModel);
	virtual ~DevRecordGenerator();

	void generate();

private:
	qrRepo::LogicalRepoApi const &mApi;
	QString const mTemplateFilePath;
};

}
}
