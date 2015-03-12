#include "saveAsTaskManager.h"

#include <QtWidgets/QFileDialog>

#include <qrgui/models/logicalModelAssistApi.h>
#include <qrrepo/repoControlInterface.h>
#include <qrutils/qRealFileDialog.h>

#include "src/ui/saveAsTaskDialog.h"

using namespace interpreterCore;
using namespace interpreterBase;

SaveAsTaskManager::SaveAsTaskManager(qReal::LogicalModelAssistInterface &logicalModel
		, qrRepo::RepoControlInterface &repoControlApi)
	: mLogicalModel(logicalModel)
	, mRepoControlApi(repoControlApi)
{
}

SaveAsTaskManager::~SaveAsTaskManager()
{
}

void SaveAsTaskManager::save()
{
	ui::SaveAsTaskDialog dialog;
	if (dialog.exec() == QDialog::Rejected) {
		return;
	}

	ReadOnlyFlags flags = dialog.readOnlyFlags();
	QHash<ReadOnly::ReadOnlyEnum, bool> oldFlags;

	const auto save = [this, &flags, &oldFlags] (const QString &tag, ReadOnly::ReadOnlyEnum flag) {
		oldFlags.insert(flag, mLogicalModel.logicalRepoApi().metaInformation(tag).toBool());
		mLogicalModel.mutableLogicalRepoApi().setMetaInformation(tag, flags.flag(flag));
	};

	save("twoDModelWorldReadOnly", ReadOnly::World);
	save("twoDModelSensorsReadOnly", ReadOnly::Sensors);
	save("twoDModelRobotPositionReadOnly", ReadOnly::RobotPosition);
	save("twoDModelRobotConfigurationReadOnly", ReadOnly::RobotSetup);
	save("twoDModelSimulationSettingsReadOnly", ReadOnly::SimulationSettings);

	QString fileName = utils::QRealFileDialog::getSaveFileName("SaveAsTask"
			, nullptr
			, QObject::tr("Select file to export save to")
			, "."
			, QObject::tr("QReal Save File(*.qrs)"));

	if (fileName.isEmpty()) {
		return;
	}

	if (!fileName.isEmpty() && !fileName.endsWith(".qrs", Qt::CaseInsensitive)) {
		fileName += ".qrs";
	}

	const QString currentWorkingFile = mRepoControlApi.workingFile();

	mRepoControlApi.saveTo(fileName);

	// Restore old flags.
	for (ReadOnly::ReadOnlyEnum flag : oldFlags.keys()) {
		flags.setFlag(flag, oldFlags[flag]);
	}

	save("twoDModelWorldReadOnly", ReadOnly::World);
	save("twoDModelSensorsReadOnly", ReadOnly::Sensors);
	save("twoDModelRobotPositionReadOnly", ReadOnly::RobotPosition);
	save("twoDModelRobotConfigurationReadOnly", ReadOnly::RobotSetup);
	save("twoDModelSimulationSettingsReadOnly", ReadOnly::SimulationSettings);

	// Save model again to set its working directory to old value.
	mRepoControlApi.saveTo(currentWorkingFile);
}
