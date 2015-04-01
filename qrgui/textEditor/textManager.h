#pragma once

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QMultiHash>

#include "qrgui/textEditor/textEditorDeclSpec.h"
#include "qrgui/textEditor/textManagerInterface.h"
#include "qrgui/textEditor/codeBlockManager.h"

namespace qReal {

namespace gui {
class MainWindowInterpretersInterface;
}

namespace text {

class QRGUI_TEXT_EDITOR_EXPORT TextManager : public TextManagerInterface
{
	Q_OBJECT

public:
	TextManager(SystemEvents &systemEvents, gui::MainWindowInterpretersInterface &mainWindow);

	/// Reads code source file and create new QScintillaTextEdit associated with this file (rather with filepath)
	bool openFile(const QString &filePath, const QString &genName, const LanguageInfo &language);

	/// Remove all info about filePath (including QScintillaTextEdit associated with it)
	bool closeFile(const QString &filePath);

	void changeFilePath(const QString &from, const QString &to);

	/// Binds diagram with another code source file.
	bool bindCode(const Id &diagram, const QString &filePath);

	bool unbindCode(const QString &filePath);
	bool unbindCode(text::QScintillaTextEdit *code);
	text::QScintillaTextEdit *code(const QString &filePath) const;
	QList<text::QScintillaTextEdit *> code(const Id &diagram) const;
	bool contains(const QString &filePath) const;
	bool removeDiagram(const Id &diagram);
	Id diagram(text::QScintillaTextEdit *code) const;
	QString path(text::QScintillaTextEdit *code) const;
	bool isDefaultPath(const QString &path) const;
	bool isModified(const QString &path) const;
	bool isModifiedEver(const QString &path) const;

	/// Opens new tab with file created by some generator in text editor and shows a text in it
	/// @param fileInfo A filepath to file with text
	/// @param genName A name of generator which created this file
	void showInTextEditor(const QFileInfo &fileInfo, const QString &genName, const LanguageInfo &language);

	/// Opens new tab with file
	/// @param fileInfo A filepath to file with text
	void showInTextEditor(const QFileInfo &fileInfo, const LanguageInfo &language);

	/// Saves text from tab to another or same file
	/// @param saveAs Defines what to do: save to the same file or in another
	bool saveText(bool saveAs);

	QString generatorName(const QString &filepath) const;
	CodeBlockManager &codeBlockManager();

private slots:
	void setModified(text::QScintillaTextEdit *code, bool modified = true);
	void onTabClosed(const QFileInfo &file);

private:
	QMap<QString, text::QScintillaTextEdit *> mText;
	QMap<text::QScintillaTextEdit *, QString> mPath;

	/// If default path - true.
	QMap<QString, bool> mPathType;

	/// Contains names of generator, which generate each сщfile
	QMap<QString, QString> mGeneratorName;

	/// First - was modified once and more (with save), second - was modified recently and not saved
	QMap<QString, QPair<bool, bool> > mModified;

	/// mCodeTabManager - Map that keeps pairs of opened tabs and their code areas.
	QMultiHash<Id, QString> mDiagramCodeManager;

	gui::MainWindowInterpretersInterface &mMainWindow;
	SystemEvents &mSystemEvents;
	CodeBlockManager mCodeBlockManager;
};

}
}
