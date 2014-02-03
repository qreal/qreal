#include "robotModelManager.h"

#include <qrkernel/settingsManager.h>

using namespace interpreterCore;

RobotModelManager::RobotModelManager()
	: mRobotModel(&mDefaultRobotModel)
{
}

interpreterBase::robotModel::RobotModelInterface &RobotModelManager::model() const
{
	return *mRobotModel;
}

void RobotModelManager::setModel(interpreterBase::robotModel::RobotModelInterface * const robotModel)
{
	if (mRobotModel != robotModel) {
		auto const actualModel = robotModel ? robotModel : &mDefaultRobotModel;
		// TODO: implement hierarchical structure in settings manager
		QString const selectedKit = qReal::SettingsManager::value("SelectedRobotKit").toString();
		// TODO: select kit here if needed
		QString const key = "SelectedModelFor" + selectedKit;
		qReal::SettingsManager::setValue(key, actualModel->name());
		mRobotModel = actualModel;
		emit robotModelChanged();
	}
}
