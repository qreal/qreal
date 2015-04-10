/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <qrgui/mainWindow/errorReporter.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrutils/graphUtils/baseGraphTransformationUnit.h>
#include "textualPart/ruleParser.h"
#include "textualPart/pythonGenerator.h"
#include "textualPart/pythonInterpreter.h"
#include "textualPart/qtScriptGenerator.h"
#include "textualPart/qtScriptInterpreter.h"

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
	~VisualInterpreterUnit();

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

private slots:
	void processTextCodeInterpreterStdOutput(QHash<QPair<QString, QString>, QString> const &output
			, TextCodeInterpreter::CodeLanguage const language);
	void processTextCodeInterpreterErrOutput(QString const &output);

protected:
	/// For debug uses only
	void highlightMatch();

	/// Fields initialization before loading semantics
	void initBeforeSemanticsLoading();

	/// Orders rules by value of priority property
	void orderRulesByPriority();

	/// Read initialization code at load semantics step
	void readInitialization();

	/// Fields initialization before interpretation
	void initBeforeInterpretation();

	/// Checks current diagram for being semantics model
	bool isSemanticsEditor() const;
	
	bool checkRuleMatching();

	/// Checks rule application conditions on the found matches
	bool checkApplicationCondition(QString const &ruleName);

	/// Checks rule application conditions on concrete match
	bool checkApplicationCondition(QHash<Id, Id> const &match, QString const &ruleName) const;

	/// Checks rule application conditions on concrete match based on QtScript
	bool checkApplicationConditionQtScript(QHash<Id, Id> const &match, QString const &ruleName) const;

	/// Checks rule application conditions on concrete match based on C-like language
	bool checkApplicationConditionCStyle(QHash<Id, Id> const &match, QString const &appCond) const;

	/// Checks rule application conditions on concrete match based on Python
	bool checkApplicationConditionPython(QHash<Id, Id> const &match, QString const &ruleName) const;

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

	/// Interpret initialization code before interpretation will be started
	void interpretInitializationCode();

	/// Interpret rule reaction
	bool interpretReaction();

	/// Interpret rule reaction written on python
	bool interpretPythonReaction();

	/// Interpret rule reaction written on QtScript
	bool interpretQtScriptReaction();

	/// Arranges connections between newly created elements
	void arrangeConnections();

	/// Copy properties from element in rule to element in model
	void copyProperties(Id const &elemInModel, Id const &elemInRule);

	/// Generates position for new element
	QPointF position();

	/// Fill rules information with this
	void putIdIntoMap(QHash<QString ,IdList*> *map, QString const &ruleName, Id const &id);

	/// Obtain an element id with the corresponding control flow mark
	Id nodeIdWithControlMark(Id const &controlMarkId) const;

	/// Get all rules from semantics model
	IdList allRules() const;

	/// Get first node from rule to start the algo
	Id startElement() const;

	/// Reports message to the main system
	void report(QString const &message, bool isError) const;

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

	/// True if interpretational semantics, false if generational semantics
	bool mIsInterpretationalSemantics;

	/// Metamodel name which loaded semantics is for
	QString mMetamodelName;

	/// All rules in map with key - rule name and value - rule id on diagram
	QHash<QString, Id> mRules;

	QStringList mOrderedRules;

	/// Pair with initialization code and its type
	QPair<QString, QString> mInitializationCode;

	/// All maps below has the format: key - rule name, value - list of ids
	QHash<QString, IdList*> mDeletedElements;
	QHash<QString, QHash<Id, Id>* > mReplacedElements;
	QHash<QString, IdList*> mCreatedElements;
	QHash<QString, IdList*> mNodesWithNewControlMark;
	QHash<QString, IdList*> mNodesWithDeletedControlMark;
	QHash<QString, IdList*> mNodesWithControlMark;
	QHash<Id, Id> mReplacedElementsPairs;
	QHash<Id, Id> mCreatedElementsPairs;

	QString mMatchedRuleName;
	QString mCurrentRuleName;

	/// Nodes of model which have control mark
	IdList mCurrentNodesWithControlMark;

	/// Rule parser and interpreter to deal with textual part of rules
	RuleParser *mRuleParser;

	PythonGenerator *mPythonGenerator;
	PythonInterpreter *mPythonInterpreter;

	QtScriptGenerator *mQtScriptGenerator;
	QtScriptInterpreter *mQtScriptInterpreter;
};

}
