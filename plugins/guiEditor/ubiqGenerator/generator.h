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

	struct NeededStringsForOneSlideDescription {
		QString oneFormDescription;
		QString onButtonDescription;
	};

	struct NeededStringsForOneElementDeclaration {
		QString elemetCreation;
		QString elementName;
	};

	bool isCorrectedName(QString const &name);
	void initGeneratingFiles();
	void generateVariables();
	void generatePresentationDiagrams();
	void generateLogicDiagrams();
	void saveGeneratedFiles();
	void generateAndSaveCSProject();
	void saveAppConfig();

	NeededStringsForPresentationDiagram generateMainForms(qReal::Id const &diagram);
	QString countStartFormName(qReal::Id const &diagram);
	NeededStringsForOneSlideDescription countFormWithButtonDescription(qReal::Id const &form);
	QString countOnButtonDescription(qReal::Id const &button);
	QString countMainGridFilling(qReal::Id const &form);
	QString countButtonDeclaration(qReal::Id const &button);
	QString countButtonDeclarationWithHandler(qReal::Id const &button);
	QString countTextDeclaration(qReal::Id const &element);
	QString countImageDeclaration(qReal::Id const &element);
	QString countListDeclaration(qReal::Id const &element);
	QString countGridDeclaration(qReal::Id const &element);
	NeededStringsForOneElementDeclaration countOneElementDeclarationForLists(qReal::Id const &element, int tail);
	QString generateHandlers(qReal::Id const &diagram);

	QString mProgramName;
	QVector<QString> mCompileIncludeFiles;

	QString mResultForms;
	QString mResultVariables;
};

}
}

