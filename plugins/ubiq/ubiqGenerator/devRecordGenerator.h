#pragma once

#include "abstractGenerator.h"

namespace ubiq {
namespace generator {

class DevRecordGenerator : public AbstractGenerator
{
public:
	DevRecordGenerator(QString const &templateDirPath
			, QString const &outputDirPath
			, qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);

	virtual ~DevRecordGenerator();

	void generate();

private:
	QString mFileTemplate;
};

}
}
