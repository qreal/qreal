#include "preferencesPage.h"

using namespace qReal::gui;

PreferencesPage::PreferencesPage(QWidget *parent)
	: QWidget(parent)
	, mShouldRestartSystemToApply(false)
{
}

PreferencesPage::~PreferencesPage()
{
}

void PreferencesPage::setRestartFlag()
{
	mShouldRestartSystemToApply = true;
}
