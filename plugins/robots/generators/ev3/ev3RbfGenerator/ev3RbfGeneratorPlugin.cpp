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

#include "ev3RbfGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDir>

#include "ev3RbfMasterGenerator.h"

#include <QtCore/QProcess>
#include <QtCore/QDebug>

using namespace ev3::rbf;
using namespace qReal;

Ev3RbfGeneratorPlugin::Ev3RbfGeneratorPlugin()
	: Ev3GeneratorPluginBase("Ev3RbfGeneratorRobotModel", tr("Generation (EV3 RBF)"), 7) ///priority????
	, mGenerateCodeAction(new QAction(nullptr))
	, mUploadProgramAction(new QAction(nullptr))
{
	mGenerateCodeAction->setText(tr("Generate to Ev3 Robot Byte Code File"));
	mGenerateCodeAction->setIcon(QIcon(":/ev3/images/generateRbfCode.svg"));
	connect(mGenerateCodeAction, &QAction::triggered, this, &Ev3RbfGeneratorPlugin::generateCode);

	mUploadProgramAction->setText(tr("Upload program"));
	mUploadProgramAction->setIcon(QIcon(":/ev3/images/uploadProgram.svg"));
	connect(mUploadProgramAction, &QAction::triggered, this, &Ev3RbfGeneratorPlugin::uploadProgram);
}

QList<ActionInfo> Ev3RbfGeneratorPlugin::customActions()
{
	const ActionInfo generateCodeActionInfo(mGenerateCodeAction, "generators", "tools");
	const ActionInfo uploadProgramActionInfo(mUploadProgramAction, "interpreters", "tools");
	return {generateCodeActionInfo, uploadProgramActionInfo};
}

QList<HotKeyActionInfo> Ev3RbfGeneratorPlugin::hotKeyActions()
{
	mGenerateCodeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_G));
	mUploadProgramAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));

	HotKeyActionInfo generateActionInfo("Generator.GenerateEv3Rbf", tr("Generate Ev3 Robot Byte Code File"), mGenerateCodeAction);
	HotKeyActionInfo uploadProgramInfo("Generator.UploadEv3", tr("Upload EV3 Program"), mUploadProgramAction);

	return { generateActionInfo, uploadProgramInfo };
}

QIcon Ev3RbfGeneratorPlugin::iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon(":/ev3/images/switch-to-ev3-rbf.svg");
}

QString Ev3RbfGeneratorPlugin::defaultFilePath(QString const &projectName) const
{
	return QString("ev3-rbf/%1/%1.lms").arg(projectName);
}

///todo: add info
text::LanguageInfo Ev3RbfGeneratorPlugin::language() const
{
	return text::LanguageInfo();
}

QString Ev3RbfGeneratorPlugin::generatorName() const
{
	return "ev3Rbf";
}

bool Ev3RbfGeneratorPlugin::uploadProgram()
{
	if (!javaInstalled()) {
		mMainWindowInterface->errorReporter()->addError(tr("Java JRE not found"));
		return false;
	}
	QFileInfo const fileInfo = generateCodeForProcessing();
	qDebug() << fileInfo.absoluteFilePath();
	qDebug() << fileInfo.absolutePath();
	qDebug() << fileInfo.absoluteDir();
	return true;
}

bool Ev3RbfGeneratorPlugin::javaInstalled()
{
	QProcess myProcess;
	myProcess.setEnvironment(QProcess::systemEnvironment());
	myProcess.start("java");
	myProcess.waitForFinished();
	return !myProcess.readAllStandardError().isEmpty();
}

generatorBase::MasterGeneratorBase *Ev3RbfGeneratorPlugin::masterGenerator()
{
	return new Ev3RbfMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mParserErrorReporter
			, *mRobotModelManager
			, *mTextLanguage
			, mMainWindowInterface->activeDiagram()
			, generatorName());
}
