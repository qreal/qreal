#pragma once

#include "abstractGenerator.h"

namespace ubiq {
namespace generator {

class MessageGenerator : public AbstractGenerator
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
	QString generateEnumElements(qReal::Id const &element);
};

}
}
