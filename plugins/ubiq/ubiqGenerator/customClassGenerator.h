#pragma once

#include "abstractGenerator.h"

namespace ubiq {
namespace generator {

class CustomClassGenerator : public AbstractGenerator
{
public:
	CustomClassGenerator(QString const &templateDirPath
			, QString const &outputDirPath
			, qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);

	virtual ~CustomClassGenerator();

	void generate();

private:
	QString mFileTemplate;
};

}
}
