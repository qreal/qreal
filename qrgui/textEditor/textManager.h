#pragma once

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QMultiHash>

#include "textManagerInterface.h"
#include "view/editorView.h"
#include "toolPluginInterface/systemEventsInterface.h"

namespace qReal {

class TextManager : public TextManagerInterface
{
	Q_OBJECT

public:
	TextManager(SystemEventsInterface *systemEvents, MainWindow *mainWindow);

	/// Reads code source file and create new QScintillaTextEdit associated with this file (rather with filepath)
	bool openFile(QString const &filePath, QString const &genName);

	/// Remove all info about filePath (including QScintillaTextEdit associated with it)
	bool closeFile(QString const &filePath);

	void changeFilePath(QString const &from, QString const &to);

	/// Binds diagram with another code source file.
	bool bindCode(EditorView* diagram,  QString const &filePath);

	bool unbindCode(QString const &filePath);
	bool unbindCode(gui::QScintillaTextEdit *code);
	gui::QScintillaTextEdit *code(QString const &filePath) const;
	QList<gui::QScintillaTextEdit *> code(EditorView *diagram) const;
	bool contains(QString const &filePath) const;
	bool removeDiagram(EditorView *diagram);
	EditorView *diagram(gui::QScintillaTextEdit *code) const;
	QString path(gui::QScintillaTextEdit *code) const;
	bool isDefaultPath(QString const &path) const;
	bool isModified(QString const &path) const;
	bool isModifiedEver(QString const &path) const;
	void addExtDescrByGenerator(QString const &genName, QString const &description);
	void addExtDescrByExtension(QString const &ext, QString const &description);
	void removeExtensions();
	QString extDescrByGenerator(QString const &genName) const;
	QString extDescrByExtension(QString const &ext) const;
	QList<QString> extDescriptions() const;

	/// Opens new tab with file created by some generator in text editor and shows a text in it
	/// @param fileInfo A filepath to file with text
	/// @param genName A name of generator which created this file
	void showInTextEditor(QFileInfo const &fileInfo, QString const &genName);

	/// Opens new tab with file
	/// @param fileInfo A filepath to file with text
	void showInTextEditor(QFileInfo const &fileInfo);

	/// Saves text from tab to another or same file
	/// @param saveAs Defines what to do: save to the same file or in another
	bool saveText(bool saveAs);

	QString generatorName(QString const &filepath) const;

private slots:
	void setModified(gui::QScintillaTextEdit *code);
	void onTabClosed(QFileInfo const &file);

private:
	QMap<QString, gui::QScintillaTextEdit *> mText;
	QMap<gui::QScintillaTextEdit *, QString> mPath;

	/// If default path - true.
	QMap<QString, bool> mPathType;

	/// Contains names of generator, which generate each сщfile
	QMap<QString, QString> mGenName;

	/// First - was modified once and more (with save), second - was modified recently and not saved
	QMap<QString, QPair<bool, bool> > mModified;

	/// mCodeTabManager - Map that keeps pairs of opened tabs and their code areas.
	QMultiHash<EditorView*, QString> mDiagramCodeManager;

	/// Contains descriptions of file extensions (generator name, description of extension). Need for save dialog.
	QMap<QString, QString> mExtDescrByGenerator;

	/// Contains descriptions of file extensions (extension, description of extension). Need for save dialog.
	QMap<QString, QString> mExtDescrByExtension;

	MainWindow *mMainWindow;
	SystemEventsInterface *mSystemEvents;
};

}
