#include <QtCore/QDir>

#include <qrkernel/settingsManager.h>

#include "fontCache.h"

#include <QtCore/QDebug>

using namespace qReal;

FontCache::FontCache()
{
	initTitlesFont();
}

void FontCache::initTitlesFont()
{
	if (SettingsManager::value("CustomFont").toBool()) {
		mTitlesFont.fromString(SettingsManager::value("CurrentFont").toString());
	} else {
		int const fontId = QFontDatabase::addApplicationFont(QDir::currentPath() + "/Pfennig.ttf");
		if (fontId != -1) {
			mTitlesFont.fromString(QFontDatabase::applicationFontFamilies(fontId).at(0) + ",11,-1,5,50,0,0,0,0,0");
		}
	}
}

FontCache *FontCache::instance()
{
	static FontCache cache;
	return &cache;
}

QFont FontCache::titlesFont() const
{
	return mTitlesFont;
}
