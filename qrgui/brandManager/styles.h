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

	/// Returns a path to a file with json stylesheet for command buttons
	virtual QString commandButtonStylePath() const
	{
		return ":/styles/commandButton.js";
	}

	/// Returns json stylesheet for command buttons
	virtual QString commandButtonStyle() const
	{
		return utils::InFile::readAll(commandButtonStylePath()).replace("@@FONT@@", mFonts.commandButtonsFont());
	}

	/// Returns a path to a file with json stylesheet for styled text of level 1 heading
	virtual QString headerLevel1StylePath() const
	{
		return ":/styles/headerLevel1.js";
	}

	/// Returns json stylesheet for styled text of level 1 heading
	virtual QString headerLevel1Style() const
	{
		return utils::InFile::readAll(headerLevel1StylePath()).replace("@@FONT@@", mFonts.styledTextFont());
	}

	/// Returns a path to a file with json stylesheet for styled text of level 2 heading
	virtual QString headerLevel2StylePath() const
	{
		return ":/styles/headerLevel2.js";
	}

	/// Returns json stylesheet for styled text of level 2 heading
	virtual QString headerLevel2Style() const
	{
		return utils::InFile::readAll(headerLevel2StylePath()).replace("@@FONT@@", mFonts.styledTextFont());
	}

	/// Returns a path to a file with json stylesheet for styled text of level 3 heading
	virtual QString headerLevel3StylePath() const
	{
		return ":/styles/headerLevel3.js";
	}

	/// Returns json stylesheet for styled text of level 3 heading
	virtual QString headerLevel3Style() const
	{
		return utils::InFile::readAll(headerLevel3StylePath()).replace("@@FONT@@", mFonts.styledTextFont());
	}

	/// Returns a path to a file with json stylesheet for styled text of level 4 heading
	virtual QString headerLevel4StylePath() const
	{
		return ":/styles/headerLevel4.js";
	}

	/// Returns json stylesheet for styled text of level 4 heading
	virtual QString headerLevel4Style() const
	{
		return utils::InFile::readAll(headerLevel4StylePath()).replace("@@FONT@@", mFonts.styledTextFont());
	}

protected:
	Fonts const &mFonts;
};

}

// Implemented in .h file for correct linkage
