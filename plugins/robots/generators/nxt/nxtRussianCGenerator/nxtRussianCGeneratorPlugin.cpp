/* Copyright 2007-2015 QReal Research Group
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

#include "nxtRussianCGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDir>

#include "nxtRussianCMasterGenerator.h"

using namespace nxt::russianC;

NxtRussianCGeneratorPlugin::NxtRussianCGeneratorPlugin()
	: NxtGeneratorPluginBase("NxtRussialCGeneratorRobotModel", tr("Generation (Russian C)"), 7 /* Last order */)
	, mGenerateCodeAction(new QAction(nullptr))
{
	mGenerateCodeAction->setText(tr("Generate to Russian C"));
	mGenerateCodeAction->setIcon(QIcon(":/nxt/russianC/images/generateRussianCCode.svg"));
	mGenerateCodeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_G));
	connect(mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));
}

QList<qReal::ActionInfo> NxtRussianCGeneratorPlugin::customActions()
{
	return { qReal::ActionInfo(mGenerateCodeAction, "generators", "tools") };
}

QList<qReal::HotKeyActionInfo> NxtRussianCGeneratorPlugin::hotKeyActions()
{
	return { qReal::HotKeyActionInfo("Generator.GenerateNxtRussianC"
			, tr("Generate Russian C Code"), mGenerateCodeAction) };
}

QIcon NxtRussianCGeneratorPlugin::iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon(":/nxt/russianC/images/switch-to-nxt-russian-c.svg");
}

QString NxtRussianCGeneratorPlugin::defaultFilePath(const QString &projectName) const
{
	return QString::fromUtf8("russianC/%1/%1.си").arg(projectName);
}

qReal::text::LanguageInfo NxtRussianCGeneratorPlugin::language() const
{
	return qReal::text::Languages::russianC();
}

QString NxtRussianCGeneratorPlugin::generatorName() const
{
	return "nxtRussianC";
}

generatorBase::MasterGeneratorBase *NxtRussianCGeneratorPlugin::masterGenerator()
{
	return new NxtRussianCMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mParserErrorReporter
			, *mRobotModelManager
			, *mTextLanguage
			, mMainWindowInterface->activeDiagram()
			, generatorName());
}

void NxtRussianCGeneratorPlugin::regenerateExtraFiles(const QFileInfo &newFileInfo)
{
	Q_UNUSED(newFileInfo);
}
