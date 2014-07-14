#pragma once

#include "kitPluginManager.h"

namespace interpreterCore {

class KitExtensionsUpdateManager : public QObject
{
	Q_OBJECT

public:                                  
	KitExtensionsUpdateManager(interpreterCore::KitPluginManager const &kitManager
			 , TextManagerInterface *textManager
			 , QObject *parent = 0);

public slots:
	void updateExtensions(interpreterBase::robotModel::RobotModelInterface &currentModel);

private:
	interpreterCore::KitPluginManager const &mKitManager;
	TextManagerInterface *mTextManager;
};

}
