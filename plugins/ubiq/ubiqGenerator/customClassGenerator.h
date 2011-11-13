#pragma once

#include "abstractGenerator.h"

namespace ubiq {
namespace generator {

QString const customClassLabel = "CustomClass";

/// Generator for custom class that can be used in Message, handlers or helper functions.
class CustomClassGenerator : public AbstractGenerator
{
public:
	/** Constructor.
	  @param templateFilePath Path to a directory with generation template.
	  @param outputFilePath Path to a directory where <custom class name>.cs shall be generated.
	  @param logicalModel Logical model reference.
	  @param errorReporter Object to return errors to.
	  */
	CustomClassGenerator(QString const &templateDirPath
			, QString const &outputDirPath
			, qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			);

	virtual ~CustomClassGenerator();

	/// Starts generation.
	void generate();

private:
	/// Generates default and "full" constructor for a class.
	QString generateConstructors(qReal::Id const &element);
};

}
}
