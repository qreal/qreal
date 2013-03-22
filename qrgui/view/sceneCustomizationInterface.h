#pragma once

namespace qReal
{

/// Provides some methods for QReal scene customization
class SceneCustomizationInterface
{
public:
	virtual ~SceneCustomizationInterface() {}

	/// Shows or hides all the titles at QReal scene
	virtual void setTitlesVisible(bool visible) = 0;
};

}
