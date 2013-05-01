#pragma once

#include "../../../qrutils/generator/abstractGenerator.h"

namespace qUbiq {
namespace qUbiqGenerator {

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
			);

	virtual ~Generator();

	/// Starts generation.
	void generate();

private:
	struct NeededStringsForPresentationDiagram {
		QString startFormName;
		QString formsDescription;
		QString onButtonClickedDescriptions;

		NeededStringsForPresentationDiagram(QString curStartFormName
											, QString curFormsDescription
											, QString curOnButtonClickedDescriptions)
				: startFormName(curStartFormName)
				, formsDescription(curFormsDescription)
				, onButtonClickedDescriptions(curOnButtonClickedDescriptions){}
	};

	bool isCorrectedName(QString const &name);
	void initGeneratingFiles();
	void saveGeneratedFiles();
	void generateVariables();
	void generatePresentationDiagrams();
	void generateLogicDiagrams();
	void generateAndSaveCSProject();
	NeededStringsForPresentationDiagram generateMainForms(qReal::Id const &diagram);
	void generateHandlers(qReal::Id const &diagram);

	QString mProgramName;
	QVector<QString> mCompileIncludeFiles;

	QString mResultForms;
	QString mResultVariables;
};

}
}

