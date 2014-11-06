#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>
#include <QtCore/QFile>

#include <qrutils/outFile.h>
#include <qrutils/qRealFileDialog.h>

#include <plugins/toolPluginInterface/systemEvents.h>
#include <mainWindow/mainWindowInterpretersInterface.h>

#include "textManager.h"
#include "qscintillaTextEdit.h"

using namespace qReal;
using namespace gui;

TextManager::TextManager(SystemEvents &systemEvents, MainWindowInterpretersInterface &mainWindow)
	: mMainWindow(mainWindow)
	, mSystemEvents(systemEvents)

{
	connect(&mSystemEvents, &SystemEvents::codeTabClosed, this, &TextManager::onTabClosed);
}

bool TextManager::openFile(QString const &filePath, QString const &generatorName)
{
	QFile file(filePath);
	QTextStream *inStream = nullptr;

	if (!file.isOpen() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		inStream = new QTextStream(&file);
		inStream->setCodec(QTextCodec::codecForName("UTF-8"));

		QScintillaTextEdit *area = new QScintillaTextEdit();

		area->setText(inStream->readAll());
		mText.insert(filePath, area);
		mPath.insert(area, filePath);
		mPathType.insert(filePath, true);
		mModified.insert(filePath, QPair<bool, bool>(false, false));
		mGeneratorName.insert(filePath, generatorName);

		connect(area, SIGNAL(textWasModified(gui::QScintillaTextEdit*)), this, SLOT(setModified(gui::QScintillaTextEdit*)));

		return true;
	}

	return false;
}

bool TextManager::bindCode(Id const &diagram,  QString const &filePath)
{
	if (mText.contains(filePath)) {
		mDiagramCodeManager.insert(diagram, filePath);
		return true;
	}
	return false;
}

bool TextManager::unbindCode(QString const &filePath)
{
	return mDiagramCodeManager.remove(mDiagramCodeManager.key(filePath), filePath) != 0;
}

bool TextManager::unbindCode(gui::QScintillaTextEdit *code)
{
	return unbindCode(mPath.value(code));
}

bool TextManager::closeFile(QString const &filePath)
{
	mPath.remove(mText.value(filePath));
	mPathType.remove(filePath);
	mModified.remove(filePath);
	mGeneratorName.remove(filePath);
	unbindCode(filePath);
	return mText.remove(filePath);
}

void TextManager::changeFilePath(QString const &from, QString const &to)
{
	QScintillaTextEdit *code = mText.value(from);
	QPair<bool, bool> mod(true, false);
	QString const genName = generatorName(from);
	Id const diagram = mDiagramCodeManager.key(from);

	closeFile(from);

	mText.insert(to, code);
	mPath.insert(code, to);
	mPathType.insert(to, false);
	mModified.insert(to, mod);
	mGeneratorName.insert(to, genName);

	if (!diagram.isNull()) {
		bindCode(diagram, to);
	}
}

QScintillaTextEdit *TextManager::code(QString const &filePath) const
{
	return mText.value(filePath);
}

QList<gui::QScintillaTextEdit *> TextManager::code(Id const &diagram) const
{
	QList<gui::QScintillaTextEdit *> codeList;

	for (QString const &filePath : mDiagramCodeManager.values(diagram)) {
		codeList += mText.value(filePath);
	}

	return codeList;
}

bool TextManager::contains(QString const &filePath) const
{
	return mText.contains(filePath);
}

bool TextManager::removeDiagram(Id const &diagram)
{
	return mDiagramCodeManager.remove(diagram);
}

Id TextManager::diagram(gui::QScintillaTextEdit *code) const
{
	return mDiagramCodeManager.key(mPath.value(code));
}

QString TextManager::path(gui::QScintillaTextEdit *code) const
{
	return mPath.value(code);
}

bool TextManager::isDefaultPath(QString const &path) const
{
	return mPathType.value(path);
}

bool TextManager::isModified(QString const &path) const
{
	return mModified.value(path).second;
}

bool TextManager::isModifiedEver(QString const &path) const
{
	return mModified.value(path).first;
}

void TextManager::setModified(gui::QScintillaTextEdit *code, bool modified)
{
	QPair<bool, bool> mod = mModified.value(mPath.value(code));
	mod.first = !modified || mod.first;
	mod.second = modified && code->isUndoAvailable();
	code->setModified(mod.second);
	mModified.insert(mPath.value(code), mod);

	emit textChanged(modified && code->isUndoAvailable());
}

void TextManager::onTabClosed(QFileInfo const &file)
{
	closeFile(file.absoluteFilePath());
}

void TextManager::addExtensionDescriptionByGenerator(QString const &generatorName, QString const &description)
{
	mExtensionDescriptionByGenerator.insert(generatorName, description);
}

void TextManager::addExtensionDescriptionByExtension(QString const &extension, QString const &description)
{
	mExtensionDescriptionByExtension.insert(extension, description);
}

void TextManager::removeExtensions()
{
	mExtensionDescriptionByGenerator.clear();
	mExtensionDescriptionByExtension.clear();
}

QString TextManager::extensionDescriptionByGenerator(QString const &generatorName) const
{
	return mExtensionDescriptionByGenerator.value(generatorName, QString());
}

QString TextManager::extensionDescriptionByExtension(QString const &extension) const
{
	return mExtensionDescriptionByExtension.value(extension, QString());
}

QList<QString> TextManager::extensionDescriptions() const
{
	return mExtensionDescriptionByGenerator.values();
}

void TextManager::showInTextEditor(QFileInfo const &fileInfo, QString const &genName)
{
	/// @todo: Uncomment it
	// Q_ASSERT(!fileInfo.baseName().isEmpty());

	if (!mMainWindow.activeDiagram().isNull()) {
		QString const filePath = fileInfo.absoluteFilePath();

		if (contains(filePath)) {
			mMainWindow.closeTab(code(filePath));
		}

		openFile(filePath, genName);
		QScintillaTextEdit *area = code(filePath);
		area->show();
		bindCode(mMainWindow.activeDiagram(), filePath);
		emit mSystemEvents.newCodeAppeared(mMainWindow.activeDiagram(), QFileInfo(filePath));

		mMainWindow.openTab(area, fileInfo.fileName());
	}
}

void TextManager::showInTextEditor(QFileInfo const &fileInfo)
{
	Q_ASSERT(!fileInfo.baseName().isEmpty());

	QString const filePath = fileInfo.absoluteFilePath();

	if (contains(filePath)) {
		mMainWindow.closeTab(code(filePath));
	}

	openFile(filePath, "");
	QScintillaTextEdit *area = code(filePath);
	area->show();

	mMainWindow.openTab(area,  fileInfo.fileName());
}

bool TextManager::saveText(bool saveAs)
{
	if (!mMainWindow.activeDiagram().isNull()) {
		return false;
	}

	QScintillaTextEdit * const area = dynamic_cast<QScintillaTextEdit *>(mMainWindow.currentTab());
	Id const diagram = TextManager::diagram(area);
	QFileInfo fileInfo;
	QString const filepath = path(area);
	bool const defaultPath = isDefaultPath(filepath);
	QString const generatorName = this->generatorName(filepath);
	QString const extensions = QStringList(extensionDescriptions()).join(";;");
	QString *currentExtensionDescription = new QString((!generatorName.isEmpty()
			? extensionDescriptionByGenerator(generatorName)
			: extensionDescriptionByExtension(QFileInfo(filepath).suffix())));

	QString extensionDescriptions = tr("All files (*)") + (extensions.isEmpty() ? "" : ";;" + extensions);

	if (currentExtensionDescription->isEmpty()) {
		currentExtensionDescription = nullptr;
	}

	if (saveAs) {
		fileInfo = QFileInfo(utils::QRealFileDialog::getSaveFileName("SaveTextFromTextManager"
				, mMainWindow.windowWidget()
				, tr("Save generated code")
				, QString()
				, extensionDescriptions
				, currentExtensionDescription));
	} else {
		fileInfo = path(area);
	}

	delete currentExtensionDescription;

	if (!fileInfo.fileName().isEmpty()) {
		mMainWindow.setTabText(area, fileInfo.fileName());

		utils::OutFile out(fileInfo.absoluteFilePath());

		out() << area->text();

		if (defaultPath || saveAs) {
			changeFilePath(path(area), fileInfo.absoluteFilePath());
		}

		setModified(area, false);

		if (saveAs && !diagram.isNull()) {
			emit mSystemEvents.codePathChanged(diagram, path(area), fileInfo);
		}
	}

	return true;
}

QString TextManager::generatorName(QString const &filePath) const
{
	return mGeneratorName.value(filePath, "");
}
