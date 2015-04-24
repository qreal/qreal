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

#include <QtWidgets/QWidget>

#include "qrutils/utilsDeclSpec.h"

namespace Ui {
class AuthenticationSettingsWidget;
}


namespace qReal {
namespace versioning {
namespace ui {

/// @brief Represents credentials editor (username, address)
/// Can be used by different plugins
class QRUTILS_EXPORT AuthenticationSettingsWidget : public QWidget
{
	Q_OBJECT
	
public:
	/// Creates new instance of @see AuthenticationSettingsWidget
	/// @param parent Parent widget
	explicit AuthenticationSettingsWidget(QWidget *parent = 0);

	/// Creates new instance of @see AuthenticationSettingsWidget
	/// @param parent Parent widget
	/// @param settingsPrefix Prefix of all settings keys used buy @see SettingsManager
	AuthenticationSettingsWidget(QString const &settingsPrefix, QWidget *parent = 0);

	~AuthenticationSettingsWidget();
	
	/// Fills fields with info provided by @see SettingsManager
	void reinit();

	/// Saves fields info with @see SettingsManager
	void save();

	/// Sets prefix of all settings keys used buy @see SettingsManager
	void setSettingsPrefix(QString const &prefix);

	/// Transates text on widget using new culture
	void retranslate();

	/// Returns settings key of is authentication enabled property
	/// @param prefix Prefix of required key
	static QString enabledSettingsName(QString const &prefix);

	/// Returns settings key of username property
	/// @param prefix Prefix of required key
	static QString usernameSettingsName(QString const &prefix);

	/// Returns settings key of password property
	/// @param prefix Prefix of required key
	static QString passwordSettingsName(QString const &prefix);

	static QString emailSettingsName(QString const &prefix);

private slots:
	void onEnableChecked();

private:
	QString enabledSettingsName() const;
	QString usernameSettingsName() const;
	QString passwordSettingsName() const;
	QString emailSettingsName() const;

	Ui::AuthenticationSettingsWidget *mUi;
	QString mSettingsPrefix;
};

}
}
}
