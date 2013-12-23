#pragma once

#include "pluginManager/toolPluginManager.h"
#include "fonts.h"
#include "styles.h"

namespace qReal {

/// Provides information about everything about application appearance: fonts, styles and so on
class BrandManager
{
public:
	static BrandManager &instance();

	/// Retrieves from plugins customization information
	static void configure(ToolPluginManager const *toolPluginManager);

	// TODO: Pluginize two following methods

	/// Returns information about all fonts in system
	static Fonts const *fonts();

	/// Returns information about all json stylesheets in system
	static Styles const *styles();

	/// Returns application`s name and probably version
	static QString applicationName();

	/// Returns application`s logo image in wide format
	static QImage const applicationLogo();

	/// Returns small application`s icon
	static QIcon const applicationIcon();

private:
	BrandManager();
	~BrandManager();

	Customizer *mCustomizer;  // Doesn`t take ownership
	Fonts const *mFonts;  // Takes ownership
	Styles const *mStyles;  // Takes ownership
};

}
