#pragma once

#include <QtGui/QFontDatabase>

namespace qReal {

/// A class for managing all used fonts
class FontCache
{
public:
	/// Returns single instance of font cache
	static FontCache *instance();

	/// Returns font for inline titles on scene
	QFont titlesFont() const;

private:
	FontCache();
	void initTitlesFont();

	QFont mTitlesFont;
};

}
