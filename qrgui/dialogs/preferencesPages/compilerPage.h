#pragma once

#include "preferencesPage.h"

namespace Ui {
	class PreferencesCompilerPage;
}

class PreferencesCompilerPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesCompilerPage(QWidget *parent = 0);
	~PreferencesCompilerPage();

	void save();
	QIcon getIcon() const;

protected:
	void changeEvent(QEvent *e);

private slots:
	void changeSystem();

private:
	void initCompilersSettings(QString const &pathToQmake,
			QString const &pathToMake, QString const &pluginExtension, QString const &prefix);

	Ui::PreferencesCompilerPage *mUi;

	QIcon mIcon;
};
