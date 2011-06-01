#pragma once

#include <QList>
#include <QtGui/QGraphicsEffect>

#include "../view/editorview.h"
#include "../mainwindow/errorReporter.h"
#include "../mainwindow/mainWindowInterpretersInterface.h"

#include "blockParser.h"

namespace qReal {
	class VisualDebugger : QObject
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
		VisualDebugger(models::LogicalModelAssistApi const &modelApi
						, models::GraphicalModelAssistApi const &mGraphicalModelApi
						, qReal::gui::MainWindowInterpretersInterface &interpretersInterface);
		~VisualDebugger();
		void setEditor(EditorView *editor);
		bool canDebug(VisualDebugger::DebugType type);
		bool canBuild();
		bool canComputeBreakpoints();
		
		void createIdByLineCorrelation();
		QList<int>* computeBreakpoints();
		Id getIdByLine(int line);
		void highlight(Id id);
		void dehighlight();
		void setDebugType(VisualDebugger::DebugType type);
	public slots:
		void generateCode();
		void debug();
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
			noErrors
		};
	private:
		EditorView *mEditor;
		qReal::gui::MainWindowInterpretersInterface &mInterpretersInterface;
		models::LogicalModelAssistApi const &mLogicalModelApi;
		models::GraphicalModelAssistApi const &mGraphicalModelApi;
		UML::Element *mCurrentElem;
		VisualDebugger::ErrorType mError;
		Id mCurrentId;
		BlockParser *mBlockParser;
		int mTimeout;
		DebugType mDebugType;
		QMap<int, Id> mIdByLineCorrelation;
		bool mHasCodeGenerationError;
		bool mHasNotEndWithFinalNode;
		QString mCodeFileName;
		QString mWorkDir;

		void error(ErrorType e);
		UML::Element* findBeginNode(QString name);
		Id findValidLink();
		void pause(int time);
		bool isFinalNode(Id id);
		bool hasEndOfLinkNode(Id id);
		ErrorType doFirstStep(UML::Element* elem);
		void doStep(Id id);
		void deinitialize();
		void processAction();
		void setTimeout(int timeout);
		void generateCode(UML::Element* elem, QFile &codeFile);
		QVariant getProperty(Id id, QString propertyName);
		void createIdByLineCorrelation(UML::Element *elem, int& line);
		void setCodeFileName(QString name);
		void setWorkDir(QString path);
	};
}
