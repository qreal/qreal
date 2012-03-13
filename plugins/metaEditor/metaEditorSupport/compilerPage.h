#pragma once

#include "../../../qrgui/dialogs/preferencesPages/preferencesPage.h"

namespace Ui {
	class PreferencesCompilerPage;
}

namespace metaEditor {

/// Page with plugin compiler settings to be shown in preferences dialog
class PreferencesCompilerPage : public PreferencesPage
{
	Q_OBJECT

public:
	/// Constructor
	/// @param parent Parent widget
	explicit PreferencesCompilerPage(QWidget *parent = 0);

	~PreferencesCompilerPage();

	void save();

protected:
	void changeEvent(QEvent *e);

private slots:
	/// Slot for switching between OS presets
	void changeSystem();

private:
	/// Helper function to init form fields after preset selection
	void initCompilersSettings(QString const &pathToQmake,
			QString const &pathToMake, QString const &pluginExtension, QString const &prefix);

	/// Ui object
	Ui::PreferencesCompilerPage *mUi;  // Has ownership
};

}
