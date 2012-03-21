#pragma once

#include <QtGui/QLineEdit>

#include "../../../qrgui/dialogs/preferencesPages/preferencesPage.h"

namespace Ui {
	class VisualDebuggerPreferencesPage;
}

namespace qReal {

/// Preference page for visual debugger of block diagrams (also with gdb debug)
class VisualDebuggerPreferencesPage : public PreferencesPage
{
	Q_OBJECT
	
public:
	explicit VisualDebuggerPreferencesPage(QWidget *parent = 0);
	~VisualDebuggerPreferencesPage();
	
	void save();
	
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
