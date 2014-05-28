#pragma once

#include "D:/qrealGen/qrutils/generator/abstractGenerator.h"

namespace generators {
namespace qUbiqPresentationGenerator {

class Generator : public utils::AbstractGenerator
{
public:
	/** Constructor.
	  @param templateDirPath Path to a directory with generation template.
	  @param outputDirPath Path to a directory.
	  @param logicalModel Logical model reference.
	  @param errorReporter Object to return errors to.
	  */
	Generator(QString const &outputDirPath
			, QString const &programName
			, qReal::LogicalModelAssistInterface const &logicalModel
			, qReal::ErrorReporterInterface &errorReporter
			, QMap<QString, QString> marksProperty
			);

	virtual ~Generator();

	/// Starts generation
	void generate();
private:
	QString const &mProgramName; // имя диаграммы (т.е. программы)
	
	bool isCorrectedName(QString const &name);
};

}
}

