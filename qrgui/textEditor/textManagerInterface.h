#pragma once

#include <qrkernel/ids.h>

#include "qrgui/textEditor/languageInfo.h"

class QFileInfo;

namespace qReal {

class SystemEvents;

namespace text {
class QScintillaTextEdit;
}

class TextManagerInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~TextManagerInterface() {}

	virtual bool openFile(QString const &filePath, QString const &genName, text::LanguageInfo const &language) = 0;
	virtual bool closeFile(QString const &filePath) = 0;
	virtual void changeFilePath(QString const &from, QString const &to) = 0;
	virtual bool bindCode(Id const &diagram, QString const &filePath) = 0;
	virtual bool unbindCode(QString const &filePath) = 0;
	virtual bool unbindCode(text::QScintillaTextEdit *code) = 0;
	virtual text::QScintillaTextEdit *code(QString const &filePath) const = 0;
	virtual QList<text::QScintillaTextEdit *> code(Id const &diagram) const = 0;
	virtual bool contains(QString const &filePath) const = 0;
	virtual Id diagram(text::QScintillaTextEdit *code) const = 0;
	virtual QString path(text::QScintillaTextEdit *code) const = 0;
	virtual bool isDefaultPath(QString const &path) const = 0;
	virtual bool isModified(QString const &path) const = 0;
	virtual bool isModifiedEver(QString const &path) const = 0;
	virtual void showInTextEditor(QFileInfo const &fileInfo
			, QString const &genName, text::LanguageInfo const &language) = 0;
	virtual void showInTextEditor(QFileInfo const &fileInfo, text::LanguageInfo const &language) = 0;
	virtual bool saveText(bool saveAs) = 0;
	virtual QString generatorName(QString const &filepath) const  = 0;

signals:
	void textChanged(bool changed);
};

}
