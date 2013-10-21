#include <QtCore/QTextStream>
#include <QtCore/QFile>

#include "textManager.h"


using namespace qReal;
using namespace gui;

TextManager::TextManager()
{
}

bool TextManager::openFile(QString const &filePath)
{
	QFile file(filePath);
	QTextStream *inStream = NULL;

	if (!file.isOpen() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		inStream = new QTextStream(&file);

		QScintillaTextEdit *area = new QScintillaTextEdit();

		area->setText(inStream->readAll());
		mText.insert(filePath, area);
		mPath.insert(area, filePath);
		mPathType.insert(filePath, true);
		mModified.insert(filePath, QPair<bool, bool>(false, false));

		connect(area, SIGNAL(textWasModified(gui::QScintillaTextEdit*)), this, SLOT(setModified(gui::QScintillaTextEdit*)));

		return true;
	}

	return false;
}

bool TextManager::bindCode(EditorView* diagram,  QString const &filePath)
{
	if (mText.contains(filePath) || openFile(filePath)) {
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
	return mText.remove(filePath) != 0;
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

	if (diagram != NULL) {
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

	foreach(QString filePath, mDiagramCodeManager.values(diagram)) {
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
	return mDiagramCodeManager.remove(diagram) != 0;
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

	mod.second = true;
	mModified.insert(mPath.value(code), mod);

	if (!changed) {
		emit textChanged();
	}
}

void TextManager::addExtension(QString const &ext, QString const &description)
{
	mExtensions.insert(ext, QString("%1 (*.%2)").arg(description, ext));
}

QString TextManager::extDescription(QString const &ext)
{
	return mExtensions.value(ext);
}
