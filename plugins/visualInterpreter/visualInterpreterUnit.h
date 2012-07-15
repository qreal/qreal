#pragma once

#include "../../qrgui/mainwindow/errorReporter.h"
#include "../../qrgui/mainwindow/mainWindowInterpretersInterface.h"
#include "../../qrutils/graphTransformation/baseGraphTransformationUnit.h"
#include "ruleParser.h"

namespace qReal {

/// Visual interpreter unit performs different transformation of graphic model
/// according to contained rules which can contain creation, removal, replacement
/// (etc) of elements.
class VisualInterpreterUnit : public BaseGraphTransformationUnit
{
	Q_OBJECT

public:
	VisualInterpreterUnit(LogicalModelAssistInterface &logicalModelApi
			, GraphicalModelAssistInterface &graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface);

	/// Load semantics model from current open diagram
	void loadSemantics();

	/// Make one step according to semantics (find match, delete,
	/// create and replace elements)
	void interpret();

	/// Stops interpretation
	void stopInterpretation();
	
	/// True if match was found
	bool findMatch();
	
	/// Get rule parser for watch list
	utils::ExpressionsParser* ruleParser();

protected:
	/// For debug uses only
	void highlightMatch();

	/// Fields initialization before loading semantics
	void initBeforeSemanticsLoading();

	/// Fields initialization before interpretation
	void initBeforeInterpretation();
	
	/// Clear memory from current semantics
	void deinit();

	/// Checks current diagram for being semantics model
	bool isSemanticsEditor() const;

	/// Perform all transformations
	bool makeStep();
	
	/// Delete elements according to rule. True if at least one element was deleted
	bool deleteElements();
	
	/// Create elements according to rule. True if at least one element was created
	bool createElements();
	
	/// Create elements to replace with according to rule. True if at least
	/// one element was created
	bool createElementsToReplace();
	
	/// Replace elements according to rule
	void replaceElements();
	
	/// Performs control flow changes
	void moveControlFlow();
	
	/// Interpret rule reaction
	bool interpretReaction();
	
	/// Arranges connections between newly created elements
	void arrangeConnections();
	
	/// Copy properties from element in rule to element in model
	void copyProperties(Id const &elemInModel, Id const &elemInRule);
	
	/// Generates position for new element
	QPointF position();
	
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
	bool mIsSemanticsLoaded;
	
	bool mNeedToStopInterpretation;
	
	/// Metamodel name which loaded semantics is for
	QString mMetamodelName;

	/// All rules in map with key - rule name and value - rule id on diagram
	QHash<QString, Id> *mRules;

	/// All maps below has the format: key - rule name, value - list of ids
	QHash<QString, IdList*> *mDeletedElements;
	QHash<QString, QHash<Id, Id>* > *mReplacedElements;
	QHash<QString, IdList*> *mCreatedElements;
	QHash<QString, IdList*> *mNodesWithNewControlMark;
	QHash<QString, IdList*> *mNodesWithDeletedControlMark;
	QHash<QString, IdList*> *mNodesWithControlMark;
	QHash<Id, Id> *mReplacedElementsPairs;
	QHash<Id, Id> *mCreatedElementsPairs;
	
	QString mMatchedRuleName;
	QString mCurrentRuleName;

	/// Nodes of model which have control mark
	IdList mCurrentNodesWithControlMark;

	/// Rule parser and interpreter to deal with textual part of rules
	RuleParser *mRuleParser;
};

}
