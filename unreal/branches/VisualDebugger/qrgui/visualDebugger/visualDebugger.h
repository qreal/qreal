#pragma once

#include <QGraphicsEffect>

#include "propertyeditorproxymodel.h"

#include "../view/editorview.h"
#include "../mainwindow/errorReporter.h"

//class qReal::gui::ErrorReporter;

namespace qReal {
//	namespace gui {
//		class ErrorReporter;
//	}
	
	class VisualDebugger : QObject
	{
		Q_OBJECT
		
	public:
		VisualDebugger(EditorView *editor, model::Model *model);
		~VisualDebugger();
		void clearErrorReporter();
	public slots:
		gui::ErrorReporter debug();
		gui::ErrorReporter debugSingleStep();
	private:
		enum ErrorType {
			missingBeginNode,
			missingEndOfLinkNode,
			endWithNotEndNode,
			missingValidLink,
			noErrors
		};
	private:
		EditorView *mEditor;
		model::Model *mModel;
		UML::Element *mCurrentElem;
		Id mCurrentId;
		QGraphicsColorizeEffect *mEffect;
		gui::ErrorReporter mErrorReporter;
		
		void error(ErrorType e);
		UML::Element* findBeginNode(QString name);
		Id findValidLink();
		void pause(int time);
		bool isFinalNode(Id id);
		bool hasEndOfLinkNode(Id id);
		ErrorType doFirstStep(UML::Element* elem);
		void doStep(Id id);
		void deinitialize();
	};
}
