#pragma once

#include "abstractGenerator.h"

namespace ubiq {
namespace generator {

QString const customClassLabel = "CustomClass";

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
	QString generateConstructors(qReal::Id const &element);
};

}
}
