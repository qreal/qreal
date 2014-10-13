#pragma once

#include <QtCore/QTranslator>
#include <QtWidgets/QLineEdit>

#include "../../../qrgui/dialogs/preferencesPages/preferencesPage.h"

namespace Ui {
	class ViewInteractionPreferencesPage;
}

namespace qReal {
namespace databasesSupport {

class DatabasesPreferencesPage : public PreferencesPage
{
public:
	explicit DatabasesPreferencesPage(QWidget *parent = 0);
	~DatabasesPreferencesPage();

	void save();
	virtual void restoreSettings();
};

}
}
