#pragma once

#include "../utilsDeclSpec.h"

#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QCheckBox>

namespace versioning
{
namespace ui
{

class QRUTILS_EXPORT NetworkSettingsWidget : public QWidget
{
	Q_OBJECT

public:
	explicit NetworkSettingsWidget(QWidget *parent = 0);

	void init();
	void save();

	void setSettingsPrefix(QString const &prefix);

//TODO: add retranslation

	QString enabledSettingsName() const;
	QString addressSettingsName() const;
	QString portSettingsName() const;
	QString usernameSettingsName() const;
	QString passwordSettingsName() const;
	QString timeoutSettingsName() const;

private:
	QString mSettingsPrefix;

	QCheckBox *mEnabledCheckBox;
	QLineEdit *mServerAddressEditor;
	QSpinBox *mServerPortEditor;
	QLineEdit *mUserEditor;
	QLineEdit *mPasswordEditor;
	QSpinBox *mTimeoutEditor;


private slots:
	void onEnableChecked();
};

}
}
