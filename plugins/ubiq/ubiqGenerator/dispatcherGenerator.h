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
	QString generateEventHandlers(qReal::Id const &diagram);
	QString generateEventHandler(QString const &handlerName);
	QString generateCaseBody(qReal::Id const &handlerStart);
	QString generateOperatorCode(qReal::Id const &currentNode);

	QString generateConstants(qReal::Id const &element) const;
	QString generateFields(qReal::Id const &element) const;
};

}
}
