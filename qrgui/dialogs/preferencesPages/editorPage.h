#pragma once

#include <QtWidgets/QFontDialog>

#include "dialogs/preferencesPages/preferencesPage.h"

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

	/// Sets value to palette combo box and spin box respectively/
	void changePaletteParameters();
	void save();
	virtual void restoreSettings();

protected:
	void changeEvent(QEvent *e);

signals:
	void gridChanged();
	void fontChanged();
	void paletteRepresentationChanged();

private slots:
	void widthGridSliderMoved(int value);
	void indexGridSliderMoved(int value);
	void dragAreaSliderMoved(int value);
	void manualFontCheckBoxChecked(bool);
	void fontSelectionButtonClicked();
	void paletteComboBoxClicked(int index);

	void showGrid(bool);
	void showAlignment(bool);
	void activateGrid(bool);
	void activateAlignment(bool);

private:
	Ui::PreferencesEditorPage *mUi;
	int mWidthGrid;
	int mIndexGrid;
	int mDragArea;
	QString mFont;

	bool mFontButtonWasPressed;
	bool mWasChecked;
	QString mOldFont;

	QAction * const mShowGridAction;
	QAction * const mShowAlignmentAction;
	QAction * const mActivateGridAction;
	QAction * const mActivateAlignmentAction;
};
