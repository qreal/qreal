#include <QtCore/QDir>

#include <qrkernel/settingsManager.h>

#include "fontCache.h"

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
		int const fontId = QFontDatabase::addApplicationFont(QDir::currentPath() + "/DejaVuSansCondensed.ttf");
		if (fontId != -1) {
			mTitlesFont = QFont(QFontDatabase::applicationFontFamilies(fontId).at(0), 7);
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
