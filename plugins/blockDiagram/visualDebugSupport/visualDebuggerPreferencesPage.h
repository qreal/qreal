#pragma once

#include <QtWidgets/QLineEdit>

#include <qrgui/preferencesDialog/preferencesPage.h>

namespace Ui {
	class VisualDebuggerPreferencesPage;
}

namespace qReal {

/// Preference page for visual debugger of block diagrams (also with gdb debug)
class VisualDebuggerPreferencesPage : public gui::PreferencesPage
{
	Q_OBJECT

public:
	explicit VisualDebuggerPreferencesPage(QWidget *parent = 0);
	~VisualDebuggerPreferencesPage();

	void save();
	virtual void restoreSettings();
private slots:

	/// Set path to builder (like gcc)
	void setBuilderPath();

	/// Set path to debugger (like gdb)
	void setDebuggerPath();

	/// Set working directory path in which the source code will be generated
	/// and executable will be builded
	void setWorkDir();

private:

	/// Show choose path dialog
	QString choosePath(bool isFolder);
	void putTextInLineEdit(QLineEdit *lineEdit, QString const &text);

	Ui::VisualDebuggerPreferencesPage *mUi;
};

}
