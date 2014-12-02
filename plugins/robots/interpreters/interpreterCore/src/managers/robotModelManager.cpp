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
		/// @todo if some settings key is modified here do not forget to modify
		/// it in UsbRobotCommunicationThread::checkConsistency
		QString const selectedKit = actualModel->kitId();
		qReal::SettingsManager::setValue("SelectedRobotKit", selectedKit);
		/// @todo select kit here if needed
		QString const key = "SelectedModelFor" + selectedKit;
		qReal::SettingsManager::setValue(key, actualModel->name());
		mRobotModel = actualModel;

		connect(mRobotModel, &RobotModelInterface::connected, this, &RobotModelManager::connected);
		connect(mRobotModel, &RobotModelInterface::disconnected, this, &RobotModelManager::disconnected);
		connect(mRobotModel, &RobotModelInterface::allDevicesConfigured
				, this, &RobotModelManager::allDevicesConfigured);

		mRobotModel->init();

		emit robotModelChanged(*mRobotModel);
	}
}

void RobotModelManager::sync()
{
	emit robotModelChanged(*mRobotModel);
}
