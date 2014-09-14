#pragma once

#include <QtGui/QIcon>
#include <QtWidgets/QWidget>

#include "preferencesDialogDeclSpec.h"

namespace qReal {
namespace gui {

/// Abstract class for preferences page
class QRGUI_PREFERENCES_DIALOG_EXPORT PreferencesPage : public QWidget
{
	Q_OBJECT

	/// @todo friend to be able to call protected changeEvent() method. It seems to be bad idea.
	friend class PreferencesDialog;

public:
	explicit PreferencesPage(QWidget *parent = 0);
	virtual ~PreferencesPage();

	/// This method will be called on pressing "Apply" or "Ok"
	virtual void save() = 0;

	/// This method will be called before page is shown and when user pressed "Cancel"
	virtual void restoreSettings() = 0;

protected:
	/// Indicates the system to prompt system restart after settings applying.
	void setRestartFlag();

private:
	/// If this flag is set to true then system restart will be prompted after settings applying.
	bool mShouldRestartSystemToApply;
};

}
}
