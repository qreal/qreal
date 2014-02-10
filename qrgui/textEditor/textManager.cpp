#include <QtCore/QTextStream>
#include <QtCore/QFile>

#include <qrutils/outFile.h>
#include <qrutils/qRealFileDialog.h>

#include "textManager.h"
#include "mainwindow/mainWindow.h"

using namespace qReal;
using namespace gui;

TextManager::TextManager(SystemEvents *systemEvents, MainWindow *mainWindow)
	: mMainWindow(mainWindow)
	, mSystemEvents(systemEvents)

{
}

bool TextManager::openFile(QString const &filePath, QString const &genName)
{
	QFile file(filePath);
	QTextStream *inStream = NULL;

	if (!file.isOpen() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		inStream = new QTextStream(&file);
		inStream->setCodec(QTextCodec::codecForName("UTF-8"));

		QScintillaTextEdit *area = new QScintillaTextEdit();

		area->setText(inStream->readAll());
		mText.insert(filePath, area);
		mPath.insert(area, filePath);
		mPathType.insert(filePath, true);
		mModified.insert(filePath, QPair<bool, bool>(false, false));
		mGenName.insert(filePath, genName);

		connect(area, SIGNAL(textWasModified(gui::QScintillaTextEdit*)), this, SLOT(setModified(gui::QScintillaTextEdit*)));

		return true;
	}

	return false;
}

bool TextManager::bindCode(EditorView* diagram,  QString const &filePath)
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
	mGenName.remove(filePath);
	return mText.remove(filePath);
}

void TextManager::changeFilePath(QString const &from, QString const &to)
{
	QScintillaTextEdit *code = mText.value(from);
	QPair<bool, bool> mod(true, false);
	closeFile(from);
	mText.insert(to, code);
	mPath.insert(code, to);
	mPathType.insert(to, false);
	mModified.insert(to, mod);

	EditorView* diagram = mDiagramCodeManager.key(from, NULL);

	if (!diagram) {
		mDiagramCodeManager.remove(diagram, from);
		mDiagramCodeManager.insert(diagram, to);
	}
}

QScintillaTextEdit *TextManager::code(QString const &filePath)
{
	return mText.value(filePath);
}

QList<gui::QScintillaTextEdit *> TextManager::code(EditorView* diagram)
{
	QList<gui::QScintillaTextEdit *> codeList;

	foreach (QString const &filePath, mDiagramCodeManager.values(diagram)) {
		codeList += mText.value(filePath);
	}

	return codeList;
}

bool TextManager::contains(QString const &filePath)
{
	return mText.contains(filePath);
}

bool TextManager::removeDiagram(EditorView *diagram)
{
	return mDiagramCodeManager.remove(diagram);
}

EditorView *TextManager::diagram(gui::QScintillaTextEdit *code)
{
	return mDiagramCodeManager.key(mText.key(code), NULL);
}

QString TextManager::path(gui::QScintillaTextEdit *code)
{
	return mPath.value(code);
}

bool TextManager::isDefaultPath(QString const &path)
{
	return mPathType.value(path);
}

bool TextManager::isModified(QString const &path)
{
	return mModified.value(path).second;
}

bool TextManager::isModifiedEver(QString const &path)
{
	return mModified.value(path).first;
}

void TextManager::setModified(gui::QScintillaTextEdit *code)
{
	QPair<bool, bool> mod = mModified.value(mPath.value(code));
	bool const changed = mod.second;

	mod.second = code->isUndoAvailable();
	mModified.insert(mPath.value(code), mod);

	if (!changed) {
		emit textChanged(true);
	} else {
		if (!code->isUndoAvailable()) {
			emit textChanged(false);
		}
	}
}

void TextManager::addExtension(QString const &name, QString const &description)
{
	mExtensions.insert(name, description);
}

QString TextManager::extDescription(QString const &name)
{
	return mExtensions.value(name);
}

void TextManager::showInTextEditor(QFileInfo const &fileInfo, QString const &genName)
{
	if (dynamic_cast<EditorView *>(mMainWindow->getCurrentTab())) {
		QString const filePath = fileInfo.absoluteFilePath();

		QScintillaTextEdit * area;

		if (!contains(filePath)) {
			openFile(filePath, genName);
			area = code(filePath);
			area->show();
			bindCode(mMainWindow->getCurrentTab(), filePath);
			emit mSystemEvents->newCodeAppeared(mMainWindow->activeDiagram(), QFileInfo(filePath));
		} else {
			area = code(filePath);
		}

		mMainWindow->openTab(area,  fileInfo.fileName());
	}
}

bool TextManager::saveText(bool saveAs)
{
	if (!dynamic_cast<EditorView *>(mMainWindow->getCurrentTab())) {
		QScintillaTextEdit * const area = dynamic_cast<QScintillaTextEdit *>(mMainWindow->currentTab());

		QFileInfo fileInfo;
		QString const filepath = path(area);
		bool const defaultPath = isDefaultPath(filepath);
		QString const genName = generatorName(filepath);
		QString const extDescr = genName == "" ? tr("All files (*.*)") : extDescription(genName);

		if (saveAs) {
			fileInfo = QFileInfo(utils::QRealFileDialog::getSaveFileName("SaveTextFromTextManager"
					, mMainWindow, tr("Save generated code"), "", extDescr));
		} else {
			fileInfo = path(area);
		}

		if (fileInfo.fileName() != "") {
			mMainWindow->setTabText(area, fileInfo.fileName());

			utils::OutFile out(fileInfo.absoluteFilePath());

			out() << area->text();

			if (saveAs) {
				emit mSystemEvents->codePathChanged(diagram(area)->mvIface()->rootId(), path(area), fileInfo);
			}

			if (defaultPath) {
				changeFilePath(fileInfo.absoluteFilePath(), fileInfo.absoluteFilePath());
			}

			mMainWindow->changeWindowTitle(0);
		}

		return true;
	}

	return false;
}

QString TextManager::generatorName(QString const &filepath)
{
	return mGenName.value(filepath, "");
}
