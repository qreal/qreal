#pragma once

#include <QWidget>

#include "../utilsDeclSpec.h"

namespace Ui {
class NetworkSettingsWidget;
}

namespace versioning
{
namespace ui
{

/// @brief Represents network settings editor
/// Includes proxy settings (address, port, username, password, timeout)
class QRUTILS_EXPORT NetworkSettingsWidget : public QWidget
{
	Q_OBJECT

public:
	/// Creates new instance of @see NetworkSettingsWidget
	/// @param parent Parent widget
	explicit NetworkSettingsWidget(QWidget *parent = 0);
	/// Creates new instance of @see NetworkSettingsWidget
	/// @param parent Parent widget
	/// @param settingsPrefix Prefix of all settings keys used by @see SettingsManager
	NetworkSettingsWidget(QString const &settingsPrefix, QWidget *parent = 0);
	~NetworkSettingsWidget();

	/// Fills fields with info provided by @see SettingsManager
	void reinit();
	/// Saves fields info with @see SettingsManager
	void save();
	/// Sets prefix of all settings keys used buy @see SettingsManager
	void setSettingsPrefix(QString const &prefix);
	/// Transates text on widget using new culture
	void retranslate();

	/// Returns settings key of use proxy or not property
	/// @param prefix Prefix of required key
	static QString enabledSettingsName(QString const &prefix);
	/// Returns settings key of proxy server address property
	/// @param prefix Prefix of required key
	static QString addressSettingsName(QString const &prefix);
	/// Returns settings key of proxy server port property
	/// @param prefix Prefix of required keys
	static QString portSettingsName(QString const &prefix);
	/// Returns settings key of proxy username property
	/// @param prefix Prefix of required key
	static QString usernameSettingsName(QString const &prefix);
	/// Returns settings key of proxy password property
	/// @param prefix Prefix of required key
	static QString passwordSettingsName(QString const &prefix);
	/// Returns settings key of proxy timeout property
	/// @param prefix Prefix of required key
	static QString timeoutSettingsName(QString const &prefix);

private slots:
	void onEnableChecked();

private:
	QString enabledSettingsName() const;
	QString addressSettingsName() const;
	QString portSettingsName() const;
	QString usernameSettingsName() const;
	QString passwordSettingsName() const;
	QString timeoutSettingsName() const;

	Ui::NetworkSettingsWidget *mUi;
	QString mSettingsPrefix;
};

}
}
