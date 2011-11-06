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
	QString generateEnumElements(qReal::Id const &element) const;
	QString generateDefaultFieldsInitialization(qReal::Id const &element) const;
	QString generateFieldsInitialization(qReal::Id const &element) const;
	QString generateConstructorArguments(qReal::Id const &element) const;
	QString generateConstructorActualArguments(qReal::Id const &element) const;
	QString generatePackFields(qReal::Id const &element) const;
	QString generateUnpackFields(qReal::Id const &element) const;
	QString generateSerializationRelatedCode(qReal::Id const &element, QString const &method) const;
};

}
}
