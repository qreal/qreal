#pragma once

#include <QWidget>

#include "../utilsDeclSpec.h"

namespace Ui {
class AuthenticationSettingsWidget;
}

namespace versioning
{
namespace ui
{

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

private slots:
	void onEnableChecked();

private:
	QString enabledSettingsName() const;
	QString usernameSettingsName() const;
	QString passwordSettingsName() const;

	Ui::AuthenticationSettingsWidget *mUi;
	QString mSettingsPrefix;
};

}
}
