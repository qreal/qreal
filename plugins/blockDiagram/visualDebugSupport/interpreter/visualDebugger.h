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

#include <QtCore/QList>

#include <qrgui/mainWindow/errorReporter.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>

#include "blockParser.h"

class QFile;

namespace qReal {

/// Visual debugger provides an opportunity to interpret and check correctness
/// of block diagram
class VisualDebugger : public QObject
{
	Q_OBJECT

public:
	enum DebugType {
		noDebug,
		singleStepDebug,
		fullDebug,
		debugWithDebugger
	};

public:
	VisualDebugger(LogicalModelAssistInterface const &logicalModelApi
			, GraphicalModelAssistInterface const &graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface
			, BlockParser *blockParser);
	~VisualDebugger();

	bool canDebug(VisualDebugger::DebugType type);
	bool canBuild();
	bool canComputeBreakpoints();

	/// For every line in generated code file it contains Id of element on the diagram
	/// from it was generated
	void createIdByLineCorrelation();

	/// Compute line number in generated code file for each element on diagram
	QList<int>* computeBreakpoints();

	Id getIdByLine(int line);

	void highlight(Id const &id);
	void dehighlight();

	void setDebugType(VisualDebugger::DebugType type);
	void setCurrentDiagram();

public slots:

	/// Generate source code from block diagram
	void generateCode();

	/// Start interpretation of block diagram in automatic mode
	void debug();

	/// Make one step of interpretation
	void debugSingleStep();

private:
	enum ErrorType {
		missingBeginNode,
		missingEndNode,
		missingEndOfLinkNode,
		endWithNotEndNode,
		missingValidLink,
		someDiagramIsRunning,
		codeGenerationError,
		incorrectUseOfLink,
		noErrors
	};

private:
	void error(ErrorType e);
	Id const findBeginNode(QString const &name);

	/// Find valid link after condition node
	Id const findValidLink();

	/// Find in links false and true edge
	void getConditionLinks(IdList const &outLinks, Id &falseEdge, Id &trueEdge);

	/// Pause before doing next step
	void pause(int time);

	bool isFinalNode(Id const &id);
	bool hasEndOfLinkNode(Id const &id);
	ErrorType doFirstStep(Id const &id);
	void doStep(Id const &id);
	void deinitialize();

	/// Interpret action in one block
	void processAction();

	bool checkForIncorrectUseOfLink(Id const &link, QString const &type);

	void setTimeout(int timeout);
	void generateCode(Id const &id, QFile &codeFile);
	QVariant getProperty(Id const &id, QString const &propertyName);

	/// Create id by line correlation from current element on diagram
	void createIdByLineCorrelation(Id const &id, int& line);

	void setCodeFileName(QString const &name);
	void setWorkDir(QString const &path);
	void setDebugColor(QString const &color);

	Id mCurrentDiagram;
	qReal::gui::MainWindowInterpretersInterface &mInterpretersInterface;
	qReal::LogicalModelAssistInterface const &mLogicalModelApi;
	qReal::GraphicalModelAssistInterface const &mGraphicalModelApi;
	VisualDebugger::ErrorType mError;
	Id mCurrentId;
	BlockParser *mBlockParser;

	/// Timeout time before doing next step
	int mTimeout;

	DebugType mDebugType;
	QColor mDebugColor;
	QMap<int, Id> mIdByLineCorrelation;
	bool mHasCodeGenerationError;
	bool mHasNotEndWithFinalNode;
	QString mCodeFileName;
	QString mWorkDir;
};

}
