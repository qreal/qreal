#include "generatorBase/robotsGeneratorPluginBase.h"

#include <qrutils/inFile.h>
#include <qrgui/mainwindow/qscintillaTextEdit.h>
#include <qrutils/nameNormalizer.h>

using namespace generatorBase;
using namespace qReal;
using namespace gui;
using namespace utils;

RobotsGeneratorPluginBase::RobotsGeneratorPluginBase()
{
}

QString RobotsGeneratorPluginBase::defaultFilePath(QString const &projectName) const
{
	return projectName;
}

QString RobotsGeneratorPluginBase::extension() const
{
	return QString();
}

QString RobotsGeneratorPluginBase::extensionDescription() const
{
	return QString();
}

QString RobotsGeneratorPluginBase::generatorName() const
{
	return QString();
}

QString RobotsGeneratorPluginBase::defaultProjectName() const
{
	QString const filePath = mProjectManager->saveFilePath();
	return filePath.isEmpty() ? "example" : QFileInfo(filePath).baseName();
}

bool RobotsGeneratorPluginBase::canGenerateTo(QString const &project)
{
	QFileInfo const fileInfo(QApplication::applicationDirPath() + "/" + defaultFilePath(project));
	return !fileInfo.exists() || fileInfo.created() == fileInfo.lastModified();
}

QFileInfo RobotsGeneratorPluginBase::srcPath()
{
	Id const &activeDiagram = mMainWindowInterface->activeDiagram();

	int exampleNumber = 0;
	QString projectName;

	do {
		projectName = NameNormalizer::normalizeStrongly(defaultProjectName(), false)
				+ (exampleNumber > 0 ? QString::number(exampleNumber) : "");
		++exampleNumber;
	} while (!canGenerateTo(projectName));

	QFileInfo fileInfo = QFileInfo(QApplication::applicationDirPath() + "/" + defaultFilePath(projectName));
	QList<QFileInfo> const pathsList = mCodePath.values(activeDiagram);

	if (!pathsList.isEmpty()) {
		for (QFileInfo const &path : pathsList) {
			if (mTextManager->isDefaultPath(path.absoluteFilePath())
				&& !mTextManager->isModifiedEver(path.absoluteFilePath())
				&& !mTextManager->generatorName(path.absoluteFilePath()).compare(generatorName()))
			{
				fileInfo = path;
				break;
			}
		}
	}

	return fileInfo;
}

QFileInfo RobotsGeneratorPluginBase::generateCodeForProcessing()
{
	QFileInfo fileInfo;
	Id const &activeDiagram = mMainWindowInterface->activeDiagram();

	if (!activeDiagram.isNull()) {
		if (generateCode(false)) {
			foreach (QFileInfo const &path, mCodePath.values(activeDiagram)) {
				if (mTextManager->isDefaultPath(path.absoluteFilePath())
					&& (!mTextManager->isModifiedEver(path.absoluteFilePath()))
					&& !mTextManager->generatorName(path.absoluteFilePath()).compare(generatorName())) {
					fileInfo = path;
					break;
				}
			}
		} else {
			return QFileInfo();
		}
	} else if (QScintillaTextEdit *code = dynamic_cast<QScintillaTextEdit *>(mMainWindowInterface->currentTab())) {
		fileInfo = QFileInfo(mTextManager->path(code));
		mTextManager->saveText(false);
	}

	return fileInfo;
}

void RobotsGeneratorPluginBase::init(PluginConfigurator const &configurator
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager)
{
	mProjectManager = &configurator.projectManager();
	mSystemEvents = &configurator.systemEvents();
	mTextManager = &configurator.textManager();

	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
	mRepo = dynamic_cast<qrRepo::RepoApi const *>(&configurator.logicalModelApi().logicalRepoApi());
	mProjectManager = &configurator.projectManager();
	mRobotModelManager = &robotModelManager;

	connect(mSystemEvents, SIGNAL(codePathChanged(qReal::Id, QFileInfo, QFileInfo))
			, this, SLOT(regenerateCode(qReal::Id, QFileInfo, QFileInfo)));
	connect(mSystemEvents, SIGNAL(newCodeAppeared(qReal::Id, QFileInfo)), this, SLOT(addNewCode(qReal::Id, QFileInfo)));
	connect(mSystemEvents, SIGNAL(diagramClosed(qReal::Id)), this, SLOT(removeDiagram(qReal::Id)));
	connect(mSystemEvents, SIGNAL(codeTabClosed(QFileInfo)), this, SLOT(removeCode(QFileInfo)));
}

bool RobotsGeneratorPluginBase::generateCode(bool openTab)
{
	mProjectManager->save();
	mMainWindowInterface->errorReporter()->clearErrors();

	MasterGeneratorBase * const generator = masterGenerator();
	QFileInfo const path = srcPath();

	generator->initialize();
	generator->setProjectDir(path);

	QString const generatedSrcPath = generator->generate();

	if (mMainWindowInterface->errorReporter()->wereErrors()) {
		delete generator;
		return false;
	}

	Id const activeDiagram = mMainWindowInterface->activeDiagram();

	QString const generatedCode = utils::InFile::readAll(generatedSrcPath);
	if (!generatedCode.isEmpty()) {
		mTextManager->showInTextEditor(path, generatorName());
	}

	if (!openTab) {
		mMainWindowInterface->activateItemOrDiagram(activeDiagram);
	}

	delete generator;
	return true;
}

void RobotsGeneratorPluginBase::regenerateCode(qReal::Id const &diagram
		, QFileInfo const &oldFileInfo
		, QFileInfo const &newFileInfo)
{
	if (!oldFileInfo.completeSuffix().compare(extension())) {
		mCodePath.remove(diagram, oldFileInfo);
		mCodePath.insert(diagram, newFileInfo);
		regenerateExtraFiles(newFileInfo);
	}
}

void RobotsGeneratorPluginBase::addNewCode(Id const &diagram, QFileInfo const &fileInfo)
{
	mCodePath.insert(diagram, fileInfo);
}

void RobotsGeneratorPluginBase::removeDiagram(qReal::Id const &diagram)
{
	mCodePath.remove(diagram);
}

void RobotsGeneratorPluginBase::removeCode(QFileInfo const &fileInfo)
{
	Id const &diagram = mCodePath.key(fileInfo);
	mCodePath.remove(diagram, fileInfo);
}
