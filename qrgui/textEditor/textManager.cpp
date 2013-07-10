#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QDebug>

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
	return mText.remove(filePath) != 0;
}

void TextManager::changeFilePath(QString const &from, QString const &to)
{
	QScintillaTextEdit *code = mText.value(from);
	closeFile(from);
	mText.insert(to, code);
	mPath.insert(code, to);
	EditorView* diagram = mDiagramCodeManager.key(from);
	mDiagramCodeManager.remove(diagram, from);
	mDiagramCodeManager.insert(diagram, to);
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
