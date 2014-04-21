#include "generatorBase/robotsGeneratorPluginBase.h"

#include <qrutils/inFile.h>
#include <qrgui/mainwindow/qscintillaTextEdit.h>

using namespace generatorBase;
using namespace qReal;
using namespace gui;

RobotsGeneratorPluginBase::RobotsGeneratorPluginBase()
{
	mAppTranslator.load(":/generatorBase_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);
}

QString RobotsGeneratorPluginBase::defaultFilePath(QString const &projectName) const
{
	return projectName;
}

QString RobotsGeneratorPluginBase::extension() const
{
	return QString();
}

QString RobotsGeneratorPluginBase::extDescrition() const
{
	return QString();
}

QString RobotsGeneratorPluginBase::generatorName() const
{
	return QString();
}

bool RobotsGeneratorPluginBase::canGenerateTo(QString const &project)
{
	/// @todo: In some scenarios user hand-coded programs will be rewritten with auto-generated
	Q_UNUSED(project)
	return true;
}

QFileInfo RobotsGeneratorPluginBase::srcPath()
{
	Id const &activeDiagram = mMainWindowInterface->activeDiagram();

	int exampleNumber = 0;
	while (!canGenerateTo("example" + QString::number(exampleNumber))) {
		++exampleNumber;
	}

	QString const projectName = "example" + QString::number(exampleNumber);
	QFileInfo fileInfo = QFileInfo(QApplication::applicationDirPath() + "/" + defaultFilePath(projectName));
	QList<QFileInfo> const pathsList = mCodePath.values(activeDiagram);

	if (!pathsList.isEmpty()) {
		foreach (QFileInfo const &path, pathsList) {
			if (mTextManager->isDefaultPath(path.absoluteFilePath())
				&& (!mTextManager->isModifiedEver(path.absoluteFilePath()))
				&& !mTextManager->generatorName(path.absoluteFilePath()).compare(generatorName())) {
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
	} else {
		QScintillaTextEdit *code = static_cast<QScintillaTextEdit *>(mMainWindowInterface->currentTab());
		fileInfo = QFileInfo(mTextManager->path(code));
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

	mTextManager->addExtension(generatorName(), QString("%1 (*.%2)").arg(extDescrition(), extension()));

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
		mTextManager->changeFilePath(oldFileInfo.absoluteFilePath(), newFileInfo.absoluteFilePath());
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
