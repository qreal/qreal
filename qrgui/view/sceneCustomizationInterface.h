#pragma once

namespace qReal
{

/// Provides some methods for QReal scene customization
/// \todo Move it to ToolPluginInterface
class SceneCustomizationInterface
{
public:
	virtual ~SceneCustomizationInterface() {}

	/// Shows or hides all the titles at QReal scene
	virtual void setTitlesVisible(bool visible) = 0;
};

}
