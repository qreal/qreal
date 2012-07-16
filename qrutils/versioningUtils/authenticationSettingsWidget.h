#pragma once

#include "../utilsDeclSpec.h"

#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>

namespace versioning
{
namespace ui
{

class QRUTILS_EXPORT AuthenticationSettingsWidget : public QWidget
{
	Q_OBJECT

public:
	explicit AuthenticationSettingsWidget(QWidget *parent = 0);

	void init();
	void save();

	void setSettingsPrefix(QString const &prefix);

//TODO: add retranslation

	QString enabledSettingsName() const;
	QString usernameSettingsName() const;
	QString passwordSettingsName() const;

private:
	QString mSettingsPrefix;

	QCheckBox *mEnabledCheckBox;
	QLineEdit *mUserEditor;
	QLineEdit *mPasswordEditor;

private slots:
	void onEnableChecked();
};

}
}
