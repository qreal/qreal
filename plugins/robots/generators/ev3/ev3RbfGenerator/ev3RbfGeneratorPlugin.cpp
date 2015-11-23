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
#include <QtCore/QDirIterator>
#include <QtCore/QProcess>

#include <ev3Kit/communication/bluetoothRobotCommunicationThread.h>

#include "ev3RbfMasterGenerator.h"

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

	text::Languages::registerLanguage(text::LanguageInfo{ "rbf"
			, tr("EV3 Source Code language")
			, true
			, 4
			, nullptr
			, {}
	});
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

	HotKeyActionInfo generateActionInfo("Generator.GenerateEv3Rbf"
			, tr("Generate Ev3 Robot Byte Code File"), mGenerateCodeAction);
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

text::LanguageInfo Ev3RbfGeneratorPlugin::language() const
{
	return text::Languages::pickByExtension("rbf");
}

QString Ev3RbfGeneratorPlugin::generatorName() const
{
	return "ev3Rbf";
}

bool Ev3RbfGeneratorPlugin::uploadProgram()
{
	if (!javaInstalled()) {
		mMainWindowInterface->errorReporter()->addError(tr("<a href=\"https://java.com/ru/download/\">Java</a> is "\
				"not installed. Please download and install it."));
		return false;
	}

	QFileInfo const fileInfo = generateCodeForProcessing();
	if (!fileInfo.exists()) {
		return false;
	}

	if (!copySystemFiles(fileInfo.absolutePath())) {
		mMainWindowInterface->errorReporter()->addError(tr("Can't write source code files to disk!"));
	}

	if (!compile(fileInfo)) {
		mMainWindowInterface->errorReporter()->addError(tr("Compilation error occured."));
	}

	if (!upload(fileInfo)) {
		mMainWindowInterface->errorReporter()->addError(tr("Could not upload file to robot. "\
				"Connect to a robot via Bluetooth."));
	}

	return true;
}

bool Ev3RbfGeneratorPlugin::javaInstalled()
{
	QProcess java;
	java.setEnvironment(QProcess::systemEnvironment());

	java.start("java");
	java.waitForFinished();
	return !java.readAllStandardError().isEmpty();
}

bool Ev3RbfGeneratorPlugin::copySystemFiles(const QString &destination)
{
	QDirIterator iterator(":/ev3Rbf/thirdparty");
	while (iterator.hasNext()) {
		const QFileInfo fileInfo(iterator.next());
		const QString destFile = destination + "/" + fileInfo.fileName();
		if (!QFile::exists(destFile) && !QFile::copy(fileInfo.absoluteFilePath(), destFile)) {
			return false;
		}
	}

	return true;
}

bool Ev3RbfGeneratorPlugin::compile(const QFileInfo &lmsFile)
{
	QProcess java;
	java.setEnvironment(QProcess::systemEnvironment());
	java.setWorkingDirectory(lmsFile.absolutePath());
	java.start("cmd /c java -jar assembler.jar " + lmsFile.absolutePath() + "/" + lmsFile.baseName());
	java.waitForFinished();
	return true;
}

bool Ev3RbfGeneratorPlugin::upload(const QFileInfo &lmsFile)
{
	const QString targetPath = "../prjs/" + lmsFile.baseName();
	const QString rbfPath = lmsFile.absolutePath() + "/" + lmsFile.baseName() + ".rbf";
	bool connected = false;
	communication::BluetoothRobotCommunicationThread communicator;
	connect(&communicator, &communication::BluetoothRobotCommunicationThread::connected
			, [&connected](bool success, const QString &) { connected = success; });
	communicator.connect();
	if (connected) {
		return communicator.uploadFile(rbfPath, targetPath);
	}

	return false;
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
