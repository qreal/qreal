#include "robotModelManager.h"

#include <qrkernel/settingsManager.h>

using namespace interpreterCore;
using namespace interpreterBase::robotModel;

RobotModelManager::RobotModelManager()
	: mRobotModel(&mDefaultRobotModel)
{
}

RobotModelInterface &RobotModelManager::model() const
{
	return *mRobotModel;
}

void RobotModelManager::setModel(RobotModelInterface * const robotModel)
{
	if (mRobotModel != robotModel) {
		disconnect(mRobotModel, nullptr, this, nullptr);
		auto const actualModel = robotModel ? robotModel : &mDefaultRobotModel;
		/// @todo implement hierarchical structure in settings manager
		QString const selectedKit = qReal::SettingsManager::value("SelectedRobotKit").toString();
		/// @todo select kit here if needed
		QString const key = "SelectedModelFor" + selectedKit;
		qReal::SettingsManager::setValue(key, actualModel->name());
		mRobotModel = actualModel;

		connect(mRobotModel, &RobotModelInterface::connected, this, &RobotModelManager::connected);
		connect(mRobotModel, &RobotModelInterface::disconnected, this, &RobotModelManager::disconnected);
		connect(mRobotModel, &RobotModelInterface::allDevicesConfigured
				, this, &RobotModelManager::allDevicesConfigured);

		emit robotModelChanged(*mRobotModel);
	}
}

void RobotModelManager::sync()
{
	emit robotModelChanged(*mRobotModel);
}
