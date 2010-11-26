#pragma once

#include <QGraphicsEffect>

#include "propertyeditorproxymodel.h"

#include "../view/editorview.h"
#include "../mainwindow/errorReporter.h"

#include "blockParser.h"

namespace qReal {
	class VisualDebugger : QObject
	{
		Q_OBJECT
		
	public:
		enum DebugType {
			noDebug,
			singleStepDebug,
			fullDebug
		};
	public:
		VisualDebugger(model::Model *model);
		~VisualDebugger();
		void clearErrorReporter();
		void setEditor(EditorView *editor);
		bool canDebug(VisualDebugger::DebugType type);
	public slots:
		gui::ErrorReporter& debug();
		gui::ErrorReporter& debugSingleStep();
	private:
		enum ErrorType {
			missingBeginNode,
			missingEndOfLinkNode,
			endWithNotEndNode,
			missingValidLink,
			wrongEditor,
			someDiagramIsRunning,
			noErrors
		};
	private:
		EditorView *mEditor;
		model::Model *mModel;
		UML::Element *mCurrentElem;
		VisualDebugger::ErrorType mError;
		Id mCurrentId;
		QGraphicsColorizeEffect *mEffect;
		gui::ErrorReporter *mErrorReporter;
		BlockParser *mBlockParser;
		int mTimeout;
		DebugType mDebugType;
		QColor mDebugColor;
		
		void error(ErrorType e);
		ErrorType checkEditor();
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
		void setDebugColor(QString color);
	};
}
