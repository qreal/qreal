#pragma once

#include <QtCore/QObject>

#include "../mainwindow/qscintillaTextEdit.h"
#include "../view/editorView.h"

namespace qReal
{

class TextManagerInterface : public QObject
{
	Q_OBJECT

public:
	virtual bool openFile(QString const &filePath) = 0;
	virtual bool closeFile(QString const &filePath) = 0;
	virtual	void changeFilePath(QString const &from, QString const &to) = 0;
	virtual bool bindCode(EditorView *diagram,  QString const &filePath) = 0;
	virtual bool unbindCode(QString const &filePath) = 0;
	virtual bool unbindCode(gui::QScintillaTextEdit *code) = 0;
	virtual gui::QScintillaTextEdit *code(QString const &filePath) = 0;
	virtual QList<gui::QScintillaTextEdit *> code(EditorView *diagram) = 0;
	virtual bool contains(QString const &filePath) = 0;
	virtual EditorView *diagram(gui::QScintillaTextEdit *code) = 0;
	virtual QString path(gui::QScintillaTextEdit *code) = 0;
	virtual bool isDefaultPath(QString const &path) = 0;
	virtual bool isModified(QString const &path) = 0;
	virtual bool isModifiedEver(QString const &path) = 0;

signals:
	void textChanged();

private slots:
	virtual void setModified(gui::QScintillaTextEdit *code) = 0;
};

}
