#pragma once
#include <QFontDialog>
#include "preferencesPage.h"

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
	QIcon getIcon() const;

protected:
	void changeEvent(QEvent *e);

signals:
	void gridChanged();
	void fontChanged();

private slots:
	void widthGridSliderMoved(int value);
	void indexGridSliderMoved(int value);
	void manualFontCheckBoxChecked(bool);
	void fontSelectionButtonClicked();


private:
	Ui::PreferencesEditorPage *mUi;
	bool mFontWasChanged;
	bool mFontButtonWasPressed;
	int mWidthGrid;
	int mIndexGrid;
	QString mFont;

	QAction * const mShowGridAction;
	QAction * const mShowAlignmentAction;
	QAction * const mActivateGridAction;
	QAction * const mActivateAlignmentAction;

	QIcon mIcon;

};
