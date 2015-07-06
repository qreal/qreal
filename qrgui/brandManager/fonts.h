/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
		return mLabelsFont;
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
			mLabelsFont.fromString(SettingsManager::value("CurrentFont").toString());
		} else {
			const int fontId = QFontDatabase::addApplicationFont(defaultSceneLabelsFont());
			if (fontId != -1) {
				mLabelsFont.fromString(QFontDatabase::applicationFontFamilies(fontId).at(0) + ",11,-1,5,50,0,0,0,0,0");
			}
		}
	}

	QFont mLabelsFont;

};

}

// Implemented in .h file for correct linkage
