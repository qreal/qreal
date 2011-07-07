#pragma once

#include "page.h"

namespace Ui {
	class PreferencesEditorPage;
}

class PreferencesEditorPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesEditorPage(QAction * const showGridAction, QAction * const showAlignmentAction
		,QAction * const activateGridAction, QAction * const activateAlignmentAction, QWidget *parent = 0);
	~PreferencesEditorPage();

	void save();

protected:
	void changeEvent(QEvent *e);

signals:
	void gridChanged();

private slots:
	void widthGridSliderMoved(int value);
	void indexGridSliderMoved(int value);

private:
	Ui::PreferencesEditorPage *mUi;

	int mWidthGrid;
	int mIndexGrid;

	QAction * const mShowGridAction;
	QAction * const mShowAlignmentAction;
	QAction * const mActivateGridAction;
	QAction * const mActivateAlignmentAction;

};
