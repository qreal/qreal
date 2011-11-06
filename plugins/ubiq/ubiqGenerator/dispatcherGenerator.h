#pragma once

#include "abstractGenerator.h"

namespace ubiq {
namespace generator {

class DispatcherGenerator : public AbstractGenerator
{
public:
	DispatcherGenerator(QString const &templateDirPath
			, QString const &outputDirPath
			, qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);

	virtual ~DispatcherGenerator();

	void generate();

private:
	QString generateConstructors(qReal::Id const &element);
};

}
}
