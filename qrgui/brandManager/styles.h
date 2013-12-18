#pragma once

#include <qrutils/inFile.h>

#include "fonts.h"

namespace qReal {

/// Manages all json stylesheets in system
class Styles
{
public:
	explicit Styles(Fonts const &fonts)
		: mFonts(fonts)
	{
	}

	virtual ~Styles()
	{
	}

	virtual QString commandButtonStylePath() const
	{
		return ":/styles/commandButton.js";
	}

	virtual QString commandButtonStyle() const
	{
		return utils::InFile::readAll(commandButtonStylePath()).replace("@@FONT@@", mFonts.commandButtonsFont());
	}

protected:
	Fonts const &mFonts;
};

}

// Implemented in .h file for correct linkage
