#include "exerciseExportManager.h"

#include <QtWidgets/QFileDialog>

#include <qrgui/models/logicalModelAssistApi.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/projectManagementInterface.h>
#include <qrrepo/repoControlInterface.h>
#include <qrutils/qRealFileDialog.h>

#include "src/ui/exerciseExportDialog.h"

using namespace interpreterCore;
using namespace kitBase;

ExerciseExportManager::ExerciseExportManager(qReal::LogicalModelAssistInterface &logicalModel
		, qrRepo::RepoControlInterface &repoControlApi, qReal::ProjectManagementInterface &projectManager)
	: mLogicalModel(logicalModel)
	, mRepoControlApi(repoControlApi)
	, mProjectManager(projectManager)
{
}

ExerciseExportManager::~ExerciseExportManager()
{
}

void ExerciseExportManager::save()
{
	ui::ExerciseExportDialog dialog;
	if (dialog.exec() == QDialog::Rejected) {
		return;
	}

	ReadOnlyFlags flags = dialog.readOnlyFlags();

	const auto save = [this, &flags/*, &oldFlags*/] (const QString &tag, ReadOnly::ReadOnlyEnum flag) {
		mLogicalModel.mutableLogicalRepoApi().setMetaInformation(tag, flags.testFlag(flag));
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

	mRepoControlApi.saveTo(fileName);

	mProjectManager.open(fileName);
}
