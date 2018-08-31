/* Copyright 2017 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "pioneerLuaGeneratorPlugin.h"

#include <qrkernel/logging.h>
#include <qrkernel/settingsManager.h>
#include <pioneerKit/blocks/pioneerBlocksFactory.h>
#include <pioneerKit/constants.h>

#include "pioneerLuaMasterGenerator.h"
#include "communicator/communicationManager.h"
#include "robotModel/pioneerGeneratorRobotModel.h"
#include "widgets/pioneerAdditionalPreferences.h"

using namespace pioneer;
using namespace pioneer::lua;
using namespace qReal;

PioneerLuaGeneratorPlugin::PioneerLuaGeneratorPlugin()
	: mGenerateCodeAction(new QAction(nullptr))
	, mUploadProgramAction(new QAction(nullptr))
	, mRunProgramAction(new QAction(nullptr))
	, mStopProgramAction(new QAction(nullptr))
	, mBlocksFactory(new blocks::PioneerBlocksFactory)
	, mGeneratorForRealCopterRobotModel(
			new PioneerGeneratorRobotModel(
					kitId()
					, "Pioneer"
					, modelNames::realCopter
					, tr("Pioneer model (real copter)")
					, 9
				)
		)
	, mGeneratorForSimulatorRobotModel(
		new PioneerGeneratorRobotModel(
				kitId()
				, "Pioneer"
				, modelNames::simulator
				, tr("Pioneer model (simulator)")
				, 10
			)
		)
{
	mAdditionalPreferences = new PioneerAdditionalPreferences;

	mGenerateCodeAction->setText(tr("Generate to Pioneer Lua"));
	mGenerateCodeAction->setIcon(QIcon(":/pioneer/lua/images/generateLuaCode.svg"));
	connect(
			mGenerateCodeAction
			, &QAction::triggered
			, this
			, [this](){ PioneerLuaGeneratorPlugin::generateCode(true); }
			, Qt::UniqueConnection
	);

	mUploadProgramAction->setText(tr("Upload generated program to Pioneer"));
	mUploadProgramAction->setIcon(QIcon(":/pioneer/lua/images/upload.svg"));
	connect(
			mUploadProgramAction
			, &QAction::triggered
			, this
			, &PioneerLuaGeneratorPlugin::uploadProgram
			, Qt::UniqueConnection
	);

	mRunProgramAction->setText(tr("Run program on a Pioneer"));
	mRunProgramAction->setIcon(QIcon(":/pioneer/lua/images/run.svg"));
	connect(
			mRunProgramAction
			, &QAction::triggered
			, this
			, &PioneerLuaGeneratorPlugin::runProgram
			, Qt::UniqueConnection
	);

	mStopProgramAction->setText(tr("Stop currently executing program"));
	mStopProgramAction->setIcon(QIcon(":/pioneer/lua/images/stop.svg"));
	connect(
			mStopProgramAction
			, &QAction::triggered
			, this
			, &PioneerLuaGeneratorPlugin::stopProgram
			, Qt::UniqueConnection
	);

	text::Languages::registerLanguage(text::LanguageInfo{ "lua"
			, tr("Lua language")
			, true
			, 4
			, nullptr
			, {}
	});
}

PioneerLuaGeneratorPlugin::~PioneerLuaGeneratorPlugin()
{
	if (mOwnsAdditionalPreferences) {
		delete mAdditionalPreferences;
	}
}

void PioneerLuaGeneratorPlugin::init(const kitBase::KitPluginConfigurator &configurator)
{
	generatorBase::RobotsGeneratorPluginBase::init(configurator);

	mMetamodel = &configurator.qRealConfigurator().logicalModelApi().editorManagerInterface();

	mCommunicationManager.reset(
			new CommunicationManager(*mMainWindowInterface->errorReporter(), *mRobotModelManager)
	);

	connect(
			mCommunicationManager.data()
			, &CommunicationManager::uploadCompleted
			, [this]() { setActionsEnabled(true); }
	);

	connect(
			mCommunicationManager.data()
			, &CommunicationManager::runCompleted
			, [this]() { setActionsEnabled(true); }
	);

	connect(
			mCommunicationManager.data()
			, &CommunicationManager::stopCompleted
			, [this]() { setActionsEnabled(true); }
	);

	connect(
			mAdditionalPreferences
			, &PioneerAdditionalPreferences::settingsChanged
			, this
			, &PioneerLuaGeneratorPlugin::onSettingsChanged
	);
}

QList<ActionInfo> PioneerLuaGeneratorPlugin::customActions()
{
	const ActionInfo generateCodeActionInfo(mGenerateCodeAction, "generators", "tools");
	const ActionInfo uploadProgramActionInfo(mUploadProgramAction, "generators", "tools");
	const ActionInfo runProgramActionInfo(mRunProgramAction, "generators", "tools");
	const ActionInfo stopProgramActionInfo(mStopProgramAction, "generators", "tools");
	return { generateCodeActionInfo, uploadProgramActionInfo, /*runProgramActionInfo, stopProgramActionInfo*/ };
}

QList<HotKeyActionInfo> PioneerLuaGeneratorPlugin::hotKeyActions()
{
	mGenerateCodeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_G));
	mUploadProgramAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
	mRunProgramAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F5));
	mStopProgramAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F5));

	HotKeyActionInfo generateActionInfo(
			"Generator.GeneratePioneerLua"
			, tr("Generate Lua script for Pioneer Quadcopter")
			, mGenerateCodeAction);

	HotKeyActionInfo uploadProgramInfo(
			"Generator.UploadPioneerLua"
			, tr("Upload Pioneer program")
			, mUploadProgramAction);

	HotKeyActionInfo runProgramInfo("Generator.RunPioneerLua", tr("Run Pioneer Program"), mRunProgramAction);

	HotKeyActionInfo stopProgramInfo("Generator.StopPioneerLua", tr("Stop Pioneer Program"), mStopProgramAction);

	return { generateActionInfo, uploadProgramInfo/*, runProgramInfo, stopProgramInfo */};
}

QIcon PioneerLuaGeneratorPlugin::iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon(":/pioneer/lua/images/switchToPioneerGenerator.svg");
}

int PioneerLuaGeneratorPlugin::priority() const
{
	return 9;
}

QString PioneerLuaGeneratorPlugin::kitId() const
{
	return "pioneerKit";
}

QList<kitBase::robotModel::RobotModelInterface *> PioneerLuaGeneratorPlugin::robotModels()
{
	return { mGeneratorForRealCopterRobotModel.data()
/// disabled for now @TODO: restore later
//#ifndef WIN32
//			// Simulator does not work for Windows.
//			, mGeneratorForSimulatorRobotModel.data()
//#endif
	};
}

kitBase::blocksBase::BlocksFactoryInterface *PioneerLuaGeneratorPlugin::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	Q_UNUSED(model)
	return mBlocksFactory;
}

QList<kitBase::AdditionalPreferences *> PioneerLuaGeneratorPlugin::settingsWidgets()
{
	mOwnsAdditionalPreferences = false;
	return { mAdditionalPreferences };
}

QString PioneerLuaGeneratorPlugin::defaultSettingsFile() const
{
	return ":/pioneer/lua/pioneerLuaDefaultSettings.ini";
}

void PioneerLuaGeneratorPlugin::onCurrentRobotModelChanged(kitBase::robotModel::RobotModelInterface &model)
{
	RobotsGeneratorPluginBase::onCurrentRobotModelChanged(model);
	mGenerateCodeAction->setVisible(model.kitId() == kitId());
	mUploadProgramAction->setVisible(model.kitId() == kitId());
//	mRunProgramAction->setVisible(model.kitId() == kitId());
//	mStopProgramAction->setVisible(model.kitId() == kitId());
}

void PioneerLuaGeneratorPlugin::onCurrentDiagramChanged(const qReal::TabInfo &info)
{
	generatorBase::RobotsGeneratorPluginBase::onCurrentDiagramChanged(info);
	if (mStopProgramAction->isEnabled()) {
		checkAndSetStopProgramAction();
	}
}

QString PioneerLuaGeneratorPlugin::defaultFilePath(const QString &projectName) const
{
	return QString("pioneer/%1/%1.lua").arg(projectName);
}

text::LanguageInfo PioneerLuaGeneratorPlugin::language() const
{
	return qReal::text::Languages::lua(QStringList{ "function", "end", "math" });
}

QString PioneerLuaGeneratorPlugin::generatorName() const
{
	return "pioneer/lua";
}

generatorBase::MasterGeneratorBase *PioneerLuaGeneratorPlugin::masterGenerator()
{
	return new PioneerLuaMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mParserErrorReporter
			, *mRobotModelManager
			, *mTextLanguage
			, mMainWindowInterface->activeDiagram()
			, generatorName()
			, *mMetamodel);
}

void PioneerLuaGeneratorPlugin::regenerateExtraFiles(const QFileInfo &newFileInfo)
{
	Q_UNUSED(newFileInfo)
}

void PioneerLuaGeneratorPlugin::uploadProgram()
{
	const QFileInfo program = generateCodeForProcessing();
	setActionsEnabled(false);
	mCommunicationManager->uploadProgram(program);
}

void PioneerLuaGeneratorPlugin::runProgram()
{
	const QFileInfo program = generateCodeForProcessing();
	setActionsEnabled(false);
	mCommunicationManager->runProgram(program);
}

void PioneerLuaGeneratorPlugin::stopProgram()
{
	setActionsEnabled(false);
	mCommunicationManager->stopProgram();
}

void PioneerLuaGeneratorPlugin::onSettingsChanged()
{
	checkAndSetStopProgramAction();
}

void PioneerLuaGeneratorPlugin::setActionsEnabled(bool isEnabled)
{
	mUploadProgramAction->setEnabled(isEnabled);
	mRunProgramAction->setEnabled(isEnabled);
	mStopProgramAction->setEnabled(isEnabled);
	if (isEnabled) {
		checkAndSetStopProgramAction();
	}
}

void PioneerLuaGeneratorPlugin::checkAndSetStopProgramAction()
{
	mStopProgramAction->setEnabled(SettingsManager::value(settings::pioneerUseController).toBool());
}
