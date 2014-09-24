#pragma once

#include <qrkernel/ids.h>

class QFileInfo;

namespace qReal {

class SystemEvents;

namespace gui {
class MainWindowInterpretersInterface;
class QScintillaTextEdit;
}

class TextManagerInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~TextManagerInterface() {}

	virtual bool openFile(QString const &filePath, QString const &genName) = 0;
	virtual bool closeFile(QString const &filePath) = 0;
	virtual	void changeFilePath(QString const &from, QString const &to) = 0;
	virtual bool bindCode(Id const &diagram, QString const &filePath) = 0;
	virtual bool unbindCode(QString const &filePath) = 0;
	virtual bool unbindCode(gui::QScintillaTextEdit *code) = 0;
	virtual gui::QScintillaTextEdit *code(QString const &filePath) const = 0;
	virtual QList<gui::QScintillaTextEdit *> code(Id const &diagram) const = 0;
	virtual bool contains(QString const &filePath) const = 0;
	virtual Id diagram(gui::QScintillaTextEdit *code) const = 0;
	virtual QString path(gui::QScintillaTextEdit *code) const = 0;
	virtual bool isDefaultPath(QString const &path) const = 0;
	virtual bool isModified(QString const &path) const = 0;
	virtual bool isModifiedEver(QString const &path) const = 0;
	virtual void addExtensionDescriptionByGenerator(QString const &genName, QString const &description) = 0;
	virtual void addExtensionDescriptionByExtension(QString const &ext, QString const &description) = 0;
	virtual void removeExtensions() = 0;
	virtual QString extensionDescriptionByGenerator(QString const &genName) const = 0;
	virtual QString extensionDescriptionByExtension(QString const &ext) const = 0;
	virtual QList<QString> extensionDescriptions() const = 0;
	virtual void showInTextEditor(QFileInfo const &fileInfo, QString const &genName) = 0;
	virtual void showInTextEditor(QFileInfo const &fileInfo) = 0;
	virtual bool saveText(bool saveAs) = 0;
	virtual QString generatorName(QString const &filepath) const  = 0;

signals:
	void textChanged(bool changed);

private slots:
	virtual void setModified(gui::QScintillaTextEdit *code, bool modified = true) = 0;
};

}
