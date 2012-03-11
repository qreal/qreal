#include <QFileDialog>

#include "visualDebuggerPreferencesPage.h"
#include "ui_visualDebuggerPreferencesPage.h"

#include "../../../qrkernel/settingsManager.h"

using namespace qReal;

VisualDebuggerPreferencesPage::VisualDebuggerPreferencesPage(QWidget *parent) :
	PreferencesPage(parent),
	mUi(new Ui::VisualDebuggerPreferencesPage)
{
	mIcon = QIcon(":/icons/preferences/bug.png");
	mUi->setupUi(this);
	
	mUi->builderPathLineEdit->setText(SettingsManager::value("builderPath", "gcc").toString());
	mUi->buildedFileNameLineEdit->setText(SettingsManager::value("buildedFileName", "builded").toString());
	mUi->codeFileNameLineEdit->setText(SettingsManager::value("codeFileName", "code.c").toString());
	mUi->visDebWorkDirLineEdit->setText(SettingsManager::value("debugWorkingDirectory", "").toString());
	mUi->debuggerPathLineEdit->setText(SettingsManager::value("debuggerPath", "gdb").toString());
	
	connect(mUi->builderPathPushButton, SIGNAL(clicked()), this, SLOT(setBuilderPath()));
	connect(mUi->debuggerPathPushButton, SIGNAL(clicked()), this, SLOT(setDebuggerPath()));
	connect(mUi->visDebWorkDirPushButton, SIGNAL(clicked()), this, SLOT(setWorkDir()));
}

VisualDebuggerPreferencesPage::~VisualDebuggerPreferencesPage()
{
	delete mUi;
}

QString VisualDebuggerPreferencesPage::choosePath(bool isFolder)
{
	if (isFolder) {
		return QFileDialog::getExistingDirectory(this, tr("Specify directory:"));
	} else {
		return QFileDialog::getOpenFileName(this, tr("Specify file:"));
	}
}

void VisualDebuggerPreferencesPage::putTextInLineEdit(QLineEdit *lineEdit, QString text)
{
	if (text != "") {
		lineEdit->setText(text);
	}
}

void VisualDebuggerPreferencesPage::setBuilderPath()
{
	putTextInLineEdit(mUi->builderPathLineEdit, choosePath(false));
}

void VisualDebuggerPreferencesPage::setDebuggerPath()
{
	putTextInLineEdit(mUi->debuggerPathLineEdit, choosePath(false));
}

void VisualDebuggerPreferencesPage::setWorkDir()
{
	putTextInLineEdit(mUi->visDebWorkDirLineEdit, choosePath(true));
}

void VisualDebuggerPreferencesPage::save()
{
	SettingsManager::setValue("debugWorkingDirectory", mUi->visDebWorkDirLineEdit->text());
	SettingsManager::setValue("codeFileName", mUi->codeFileNameLineEdit->text());
	SettingsManager::setValue("buildedFileName", mUi->buildedFileNameLineEdit->text());
	SettingsManager::setValue("builderPath", mUi->builderPathLineEdit->text());
	SettingsManager::setValue("debuggerPath", mUi->debuggerPathLineEdit->text());
}
