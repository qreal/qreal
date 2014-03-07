#pragma once

#include <QtGui/QFontDatabase>

#include <qrkernel/settingsManager.h>

namespace qReal {

/// A class for managing all used in system fonts
class Fonts
{
public:
	Fonts()
	{
		initTitlesFont();
	}

	virtual ~Fonts()
	{
	}

	/// Returns font for inline labels on scene
	QFont sceneLabelsFont() const
	{
		return mTitlesFont;
	}

	/// Returns a path to a font for inline labels on scene if other was not selected by user
	virtual QString defaultSceneLabelsFont() const
	{
		return ":/fonts/Pfennig.ttf";
	}

	/// Returns a path to a font for a text on styled command buttons
	virtual QString commandButtonsFont() const
	{
		return ":/fonts/Pfennig.ttf";
	}

	/// Returns a path to a font for a styled text, non-regular one
	virtual QString styledTextFont() const
	{
		return ":/fonts/Pfennig.ttf";
	}

private:
	void initTitlesFont()
	{
		if (SettingsManager::value("CustomFont").toBool()) {
			mTitlesFont.fromString(SettingsManager::value("CurrentFont").toString());
		} else {
			int const fontId = QFontDatabase::addApplicationFont(defaultSceneLabelsFont());
			if (fontId != -1) {
				mTitlesFont.fromString(QFontDatabase::applicationFontFamilies(fontId).at(0) + ",11,-1,5,50,0,0,0,0,0");
			}
		}
	}

	QFont mTitlesFont;

};

}

// Implemented in .h file for correct linkage
