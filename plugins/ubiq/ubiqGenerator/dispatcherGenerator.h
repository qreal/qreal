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
	QString generateEventHandlers(qReal::Id const &diagram) const;
	QString generateEventHandler(QString const &handlerName) const;
	QString generateCaseBody(qReal::Id const &handlerStart) const;
	QString generateOperatorCode(qReal::Id const &currentNode) const;

	QString generateConstants(qReal::Id const &element) const;
	QString generateFields(qReal::Id const &element) const;
	QString generateMessageInputMethods(qReal::Id const &element) const;
	QString generateHelperFunctions(qReal::Id const &element) const;
	QString generateFunctionParameters(qReal::Id const &element) const;
	QString generateFunctionBody(qReal::Id const &element) const;
};

}
}
