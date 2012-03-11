#pragma once

#include <QtGui/QLineEdit>

#include "../../../qrgui/dialogs/preferencesPages/preferencesPage.h"

namespace Ui {
	class VisualDebuggerPreferencesPage;
}

namespace qReal {

class VisualDebuggerPreferencesPage : public PreferencesPage
{
	Q_OBJECT
	
public:
	explicit VisualDebuggerPreferencesPage(QWidget *parent = 0);
	~VisualDebuggerPreferencesPage();
	
	void save();
	
private slots:
	void setBuilderPath();
	void setDebuggerPath();
	void setWorkDir();
	
private:
	QString choosePath(bool isFolder);
	void putTextInLineEdit(QLineEdit *lineEdit, QString text);

	Ui::VisualDebuggerPreferencesPage *mUi;
};

}
