#pragma once

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QMultiHash>

#include "textManagerInterface.h"
#include "../mainwindow/qscintillaTextEdit.h"
#include "../view/editorView.h"

namespace qReal
{

class TextManager : public TextManagerInterface
{
public:
	TextManager();

	bool openFile(QString const &filePath);
	bool closeFile(QString const &filePath);
	void changeFilePath(QString const &from, QString const &to);
	bool bindCode(EditorView* diagram,  QString const &filePath);
	bool unbindCode(QString const &filePath);
	bool unbindCode(gui::QScintillaTextEdit *code);
	gui::QScintillaTextEdit *code(QString const &filePath);
	QList<gui::QScintillaTextEdit *> code(EditorView *diagram);
	bool contains(QString const &filePath);
	bool removeDiagram(EditorView *diagram);
	EditorView *diagram(gui::QScintillaTextEdit *code);
	QString path(gui::QScintillaTextEdit *code);
	bool isDefaultPath(QString const &path);
	bool isModified(QString const &path);
	bool isModifiedEver(QString const &path);
	void addExtension(QString const &ext, QString const &description);
	QString extDescription(QString const &ext);

private slots:
	void setModified(gui::QScintillaTextEdit *code);

private:
	QMap<QString, gui::QScintillaTextEdit *> mText;
	QMap<gui::QScintillaTextEdit *, QString> mPath;
	QMap<QString, bool> mPathType;

	//first - was modified once and more (with save), second - was modified recently and not saved
	QMap<QString, QPair<bool, bool> > mModified;

	/// mCodeTabManager - Map that keeps pairs of opened tabs and their code areas.
	QMultiHash<EditorView*, QString> mDiagramCodeManager;
	QMap<QString, QString> mExtensions;
};

}
