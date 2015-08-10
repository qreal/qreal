#pragma once

#include <QtCore/QTranslator>
#include <QtWidgets/QLineEdit>
#include <qrgui/preferencesDialog/preferencesPage.h>
#include "ui_databasesPreferencesPage.h"

namespace Ui {
	class DatabasesPreferencesPage;
}

namespace qReal {
namespace databasesSupport {

/// Class for plugin preferences
class DatabasesPreferencesPage : public gui::PreferencesPage
{
	Q_OBJECT
public:
	explicit DatabasesPreferencesPage(QWidget *parent = 0);
	~DatabasesPreferencesPage();

	void save();
	virtual void restoreSettings();
public slots:
	void dbmsChanging(QString const &dbmsName);
signals:
	void dbmsChanged(QString const &dbmsName);
private:
	Ui::DatabasesPreferencesPage *mUi;
};

}
}
