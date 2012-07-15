#pragma once

#include <QtGui/QDialog>

#include "../view/editorView.h"
#include "../umllib/element.h"
#include "../umllib/layout/abstractLayoutHelper.h"

namespace Ui {
class AutoLayoutDialog;
}

/// Window that allows to work with different auto layout algorithms
class AutoLayoutDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AutoLayoutDialog(QWidget *parent = 0);
	~AutoLayoutDialog();

	/// Adds another auto layout algorithm
	void addLayoutHelper(qReal::AbstractLayoutHelper *abstractLayoutHelper);
	
	/// Sets to current EditorViewMViface for getting elements
	void setMvIface(EditorViewMViface *mvIface);

private slots:
	/// Shows the proper configuration page
	void methodSelected(QString const &methodName);

	/// Gets elements from scene and runs selected method
	void prepareAndRunMethod();
	
private:
	void runMethod();

	Ui::AutoLayoutDialog *mUi;
	QList<Element *> mElements;
	QList<qReal::AbstractLayoutHelper *> mLayoutHelpers;
	EditorViewMViface *mMvIface;
};
