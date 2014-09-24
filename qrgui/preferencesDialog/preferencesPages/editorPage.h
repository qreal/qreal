#pragma once

#include <QtWidgets/QFontDialog>

#include "preferencesPage.h"

namespace Ui {
	class PreferencesEditorPage;
}

class PreferencesEditorPage : public qReal::gui::PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesEditorPage(QWidget *parent = 0);
	~PreferencesEditorPage();

	/// Sets value to palette combo box and spin box respectively/
	void changePaletteParameters();
	void save();
	virtual void restoreSettings();

protected:
	void changeEvent(QEvent *e);

private slots:
	void widthGridSliderMoved(int value);
	void indexGridSliderMoved(int value);
	void dragAreaSliderMoved(int value);
	void manualFontCheckBoxChecked(bool);
	void fontSelectionButtonClicked();
	void paletteComboBoxClicked(int index);

private:
	Ui::PreferencesEditorPage *mUi;
	int mWidthGrid;
	int mIndexGrid;
	int mDragArea;
	QString mFont;

	bool mFontButtonWasPressed;
	bool mWasChecked;
	QString mOldFont;
};
