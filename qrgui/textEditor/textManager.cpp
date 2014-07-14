#include <QtCore/QTextStream>
#include <QtCore/QFile>

#include <qrutils/outFile.h>
#include <qrutils/qRealFileDialog.h>

#include "textManager.h"
#include "mainwindow/mainWindow.h"

using namespace qReal;
using namespace gui;

TextManager::TextManager(SystemEventsInterface *systemEvents, MainWindow *mainWindow)
	: mMainWindow(mainWindow)
	, mSystemEvents(systemEvents)

{
	connect(mSystemEvents, &SystemEventsInterface::codeTabClosed, this, &TextManager::onTabClosed);
}

bool TextManager::openFile(QString const &filePath, QString const &genName)
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
	unbindCode(filePath);
	return mText.remove(filePath);
}

void TextManager::changeFilePath(QString const &from, QString const &to)
{
	QScintillaTextEdit *code = mText.value(from);
	QPair<bool, bool> mod(true, false);
	QString const genName = generatorName(from);
	EditorView *diagram = mDiagramCodeManager.key(from, nullptr);

	closeFile(from);

	mText.insert(to, code);
	mPath.insert(code, to);
	mPathType.insert(to, false);
	mModified.insert(to, mod);
	mGenName.insert(to, genName);

	if (diagram != nullptr) {
		bindCode(diagram, to);
	}
}

QScintillaTextEdit *TextManager::code(QString const &filePath) const
{
	return mText.value(filePath);
}

QList<gui::QScintillaTextEdit *> TextManager::code(EditorView* diagram) const
{
	QList<gui::QScintillaTextEdit *> codeList;

	foreach (QString const &filePath, mDiagramCodeManager.values(diagram)) {
		codeList += mText.value(filePath);
	}

	return codeList;
}

bool TextManager::contains(QString const &filePath) const
{
	return mText.contains(filePath);
}

bool TextManager::removeDiagram(EditorView *diagram)
{
	return mDiagramCodeManager.remove(diagram);
}

EditorView *TextManager::diagram(gui::QScintillaTextEdit *code) const
{
	return mDiagramCodeManager.key(mPath.value(code), nullptr);
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

	mod.second = modified;
	code->setModified(modified);
	mModified.insert(mPath.value(code), mod);

	emit textChanged(modified);
}

void TextManager::onTabClosed(QFileInfo const &file)
{
	closeFile(file.absoluteFilePath());
}

void TextManager::addExtDescrByGenerator(QString const &genName, QString const &description)
{
	mExtDescrByGenerator.insert(genName, description);
}

void TextManager::addExtDescrByExtension(QString const &ext, QString const &description)
{
	mExtDescrByExtension.insert(ext, description);
}

void TextManager::removeExtensions()
{
	mExtDescrByGenerator.clear();
	mExtDescrByExtension.clear();
}

QString TextManager::extDescrByGenerator(QString const &genName) const
{
	return mExtDescrByGenerator.value(genName, QString());
}

QString TextManager::extDescrByExtension(QString const &ext) const
{
	return mExtDescrByExtension.value(ext, QString());
}

QList<QString> TextManager::extDescriptions() const
{
	return mExtDescrByGenerator.values();
}

void TextManager::showInTextEditor(QFileInfo const &fileInfo, QString const &genName)
{
	Q_ASSERT(!fileInfo.baseName().isEmpty());

	if (dynamic_cast<EditorView *>(mMainWindow->getCurrentTab())) {
		QString const filePath = fileInfo.absoluteFilePath();

		if (contains(filePath)) {
			mMainWindow->closeTab(code(filePath));
		}

		openFile(filePath, genName);
		QScintillaTextEdit *area = code(filePath);
		area->show();
		bindCode(mMainWindow->getCurrentTab(), filePath);
		mSystemEvents->emitNewCodeAppeared(mMainWindow->activeDiagram(), QFileInfo(filePath));

		mMainWindow->openTab(area,  fileInfo.fileName());
	}
}

void TextManager::showInTextEditor(QFileInfo const &fileInfo)
{
	Q_ASSERT(!fileInfo.baseName().isEmpty());

	QString const filePath = fileInfo.absoluteFilePath();

	if (contains(filePath)) {
		mMainWindow->closeTab(code(filePath));
	}

	openFile(filePath, "");
	QScintillaTextEdit *area = code(filePath);
	area->show();

	mMainWindow->openTab(area,  fileInfo.fileName());
}

bool TextManager::saveText(bool saveAs)
{
	if (!dynamic_cast<EditorView *>(mMainWindow->getCurrentTab())) {
		QScintillaTextEdit * const area = dynamic_cast<QScintillaTextEdit *>(mMainWindow->currentTab());
		EditorView * const diagram = TextManager::diagram(area);
		QFileInfo fileInfo;
		QString const filepath = path(area);
		bool const defaultPath = isDefaultPath(filepath);
		QString const genName = generatorName(filepath);
		QString const extensions = QStringList(extDescriptions()).join(";;");
		QString *curExtDescr = new QString((!genName.isEmpty() ? extDescrByGenerator(genName)
				: extDescrByExtension(QFileInfo(filepath).suffix())));
		QString extDescrs = tr("All files (*)") + (extensions.isEmpty() ? "" : ";;" + extensions);

		if (curExtDescr->isEmpty()) {
			curExtDescr = nullptr;
		}

		if (saveAs) {
			fileInfo = QFileInfo(utils::QRealFileDialog::getSaveFileName("SaveTextFromTextManager"
					, mMainWindow, tr("Save generated code"), "", extDescrs, curExtDescr));
		} else {
			fileInfo = path(area);
		}

		delete curExtDescr;

		if (fileInfo.fileName() != "") {
			mMainWindow->setTabText(area, fileInfo.fileName());

			utils::OutFile out(fileInfo.absoluteFilePath());

			out() << area->text();

			if (defaultPath || saveAs) {
				changeFilePath(path(area), fileInfo.absoluteFilePath());
			}

			setModified(area, false);

			if (saveAs && diagram != nullptr) {
				emit mSystemEvents->codePathChanged(diagram->mvIface()->rootId(), path(area), fileInfo);
			}
		}

		return true;
	}

	return false;
}

QString TextManager::generatorName(QString const &filepath) const
{
	return mGenName.value(filepath, "");
}
