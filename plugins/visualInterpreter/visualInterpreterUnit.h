#pragma once

#include "../../../qrgui/mainwindow/errorReporter.h"
#include "../../../qrgui/mainwindow/mainWindowInterpretersInterface.h"
#include "../../../qrutils/graphTransformation/baseGraphTransformationUnit.h"
#include "ruleParser.h"

namespace qReal {

/// Visual interpreter unit performs different transformation of graphic model
/// according to contained rules which can contain creation, removal, replacement
/// (etc) of elements.
class VisualInterpreterUnit : public BaseGraphTransformationUnit
{
	Q_OBJECT

public:
	VisualInterpreterUnit(LogicalModelAssistInterface const &logicalModelApi
			, GraphicalModelAssistInterface const &graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface);
	
	/// Load semantics model from current open diagram
	void loadSemantics();
	
	/// Make one step according to semantics (find match, delete,
	/// create and replace elements)
	void interpret();
	
	/// True if match was found
	bool findMatch();
	
protected:
	/// For debug uses only
	void highlightMatch();
	
	/// Fields initialization before loading semantics
	void initBeforeSemanticsLoading();
	
	/// Fields initialization before interpretation
	void initBeforeInterpretation();
	
	/// Checks current diagram for being semantics model
	bool isSemanticsEditor();
	
	/// Perform all transformations
	bool makeStep();
	
	/// Fill rules information with this
	void putIdIntoMap(QHash<QString ,IdList*> *map, QString const &ruleName,
			Id const &id);
	
	/// Obtain an element id with the corresponding control flow mark
	Id nodeIdWithControlMark(Id const &controlMarkId) const;
	
	/// Get all rules from semantics model
	IdList allRules() const;
	
	/// Get first node from rule to start the algo
	Id startElement() const;
	
	/// Functions for test elements for equality
	bool compareElements(Id const &first, Id const &second) const;
	bool compareElementTypesAndProperties(Id const &first, Id const &second) const;
	
	/// Logical repo api methods for more quick access
	IdList linksInRule(Id const &id) const;
	
	/// Reports if semantics loading failed
	void semanticsLoadingError(QString const &message);
	
	/// Is semantics loaded successfully
	bool isSemanticsLoaded;
	
	/// Metamodel name which loaded semantics is for
	QString mMetamodelName;
	
	/// All rules in map with key - rule name and value - rule id on diagram
	QHash<QString, Id> *mRules;
	
	/// All maps below has the format: key - rule name, value - list of ids
	QHash<QString, IdList*> *mDeletedElements;
	QHash<QString, IdList*> *mReplacedElements;
	QHash<QString, IdList*> *mCreatedElements;
	QHash<QString, IdList*> *mNodesWithNewControlMark;
	QHash<QString, IdList*> *mNodesWithDeletedControlMark;
	QHash<QString, IdList*> *mNodesWithControlMark;
	
	QString mMatchedRuleName;
	QString mCurrentRuleName;
	
	/// Nodes of model which have control mark
	IdList mCurrentNodesWithControlMark;
	
	/// Rule parser and interpreter to deal with textual part of rules
	RuleParser *mRuleParser;
};

}
