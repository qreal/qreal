/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

signals:
	/// Emitted when user pressed "OK" or "Apply" button in settings dialog.
	void saved();

	/// Emitted when user closed or opened settings dialog.
	void restored();

protected:
	/// Indicates the system to prompt system restart after settings applying.
	void setRestartFlag();

private:
	/// If this flag is set to true then system restart will be prompted after settings applying.
	bool mShouldRestartSystemToApply;
};

}
}
