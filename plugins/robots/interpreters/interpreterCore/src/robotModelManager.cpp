#include "robotModelManager.h"

#include <qrkernel/settingsManager.h>

using namespace interpreterCore;

RobotModelManager *RobotModelManager::instance()
{
	static RobotModelManager instance;
	return &instance;
}

interpreterBase::robotModel::RobotModelInterface *RobotModelManager::selectedModel()
{
	return instance()->mRobotModel;
}

void RobotModelManager::setRobotModel(interpreterBase::robotModel::RobotModelInterface * const robotModel)
{
	// TODO: assign default robotModel if null passed
	// TODO: move DefaultRobotModel class into interpreterCore
	// TODO: remove proxy model?
	if (instance()->mRobotModel != robotModel) {
		// TODO: implement hierarchical structure in settings manager
		QString const selectedKit = qReal::SettingsManager::value("SelectedRobotKit").toString();
		// TODO: select kit here if needed
		QString const key = "SelectedModelFor" + selectedKit;
		qReal::SettingsManager::setValue(key, robotModel->name());
		instance()->mRobotModel = robotModel;
		emit instance()->robotModelChanged();
	}
}

RobotModelManager::RobotModelManager()
{
}

RobotModelManager::~RobotModelManager()
{
}
