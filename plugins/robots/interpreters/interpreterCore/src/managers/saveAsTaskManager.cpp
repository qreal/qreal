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

	const auto save = [this, &flags] (const QString &tag, ReadOnly::ReadOnlyEnum flag) {
		mLogicalModel.mutableLogicalRepoApi().setMetaInformation(tag, flags.flag(flag));
	};

	save("twoDModelWorldReadOnly", ReadOnly::World);
	save("twoDModelSensorsReadOnly", ReadOnly::Sensors);
	save("twoDModelRobotPositionReadOnly", ReadOnly::RobotPosition);
	save("twoDModelRobotConfigurationReadOnly", ReadOnly::RobotConfiguration);
	save("twoDModelSimulationSettingsReadOnly", ReadOnly::SimulationSettings);

	QString fileName = utils::QRealFileDialog::getSaveFileName("SaveAsTask"
			, nullptr
			, QObject::tr("Select file to export save to")
			, "."
			, QObject::tr("QReal Save File(*.qrs)"));

	if (!fileName.isEmpty() && !fileName.endsWith(".qrs", Qt::CaseInsensitive)) {
		fileName += ".qrs";
	}

	const QString currentWorkingFile = mRepoControlApi.workingFile();
	mRepoControlApi.saveTo(fileName);

	flags = {};

	/// @todo: not good, can accidentally remove flags from existing save.
	save("twoDModelWorldReadOnly", ReadOnly::World);
	save("twoDModelSensorsReadOnly", ReadOnly::Sensors);
	save("twoDModelRobotPositionReadOnly", ReadOnly::RobotPosition);
	save("twoDModelRobotConfigurationReadOnly", ReadOnly::RobotConfiguration);
	save("twoDModelSimulationSettingsReadOnly", ReadOnly::SimulationSettings);

	mRepoControlApi.saveTo(currentWorkingFile);
}
