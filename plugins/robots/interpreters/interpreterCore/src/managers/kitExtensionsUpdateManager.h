#pragma once

#include "kitPluginManager.h"

namespace interpreterCore {

/// Manages and keeps updated available extensions for text files.
/// @todo Rename it.
class KitExtensionsUpdateManager : public QObject
{
	Q_OBJECT

public:
	KitExtensionsUpdateManager(interpreterCore::KitPluginManager const &kitManager
			, TextManagerInterface &textManager
			, QObject *parent = 0);

public slots:
	void updateExtensions(interpreterBase::robotModel::RobotModelInterface &currentModel);

private:
	interpreterCore::KitPluginManager const &mKitManager;
	TextManagerInterface &mTextManager;
};

}
