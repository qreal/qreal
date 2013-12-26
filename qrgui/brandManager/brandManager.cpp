#include "brandManager.h"

using namespace qReal;

BrandManager::BrandManager()
	: mFonts(new Fonts)
	, mStyles(new Styles(*mFonts))
{
}

BrandManager::~BrandManager()
{
	delete mFonts;
	delete mStyles;
}

BrandManager &BrandManager::instance()
{
	static BrandManager instance;
	return instance;
}

void BrandManager::configure(ToolPluginManager const *toolPluginManager)
{
	instance().mCustomizer = toolPluginManager->customizer();
}

Fonts const *BrandManager::fonts()
{
	return instance().mFonts;
}

Styles const *BrandManager::styles()
{
	return instance().mStyles;
}

QString BrandManager::applicationName()
{
	return instance().mCustomizer->windowTitle();
}

const QImage BrandManager::applicationLogo()
{
	return instance().mCustomizer->applicationLogo();
}

const QIcon BrandManager::applicationIcon()
{
	return instance().mCustomizer->applicationIcon();
}
