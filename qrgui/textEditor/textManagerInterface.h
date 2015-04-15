/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <qrkernel/ids.h>

#include "qrgui/textEditor/languageInfo.h"

class QFileInfo;

namespace qReal {

class SystemEvents;

namespace text {
class QScintillaTextEdit;
}

class QRGUI_TEXT_EDITOR_EXPORT TextManagerInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~TextManagerInterface() {}

	virtual bool openFile(const QString &filePath, const QString &genName, const text::LanguageInfo &language) = 0;
	virtual bool closeFile(const QString &filePath) = 0;
	virtual void changeFilePath(const QString &from, const QString &to) = 0;
	virtual bool bindCode(const Id &diagram, const QString &filePath) = 0;
	virtual bool unbindCode(const QString &filePath) = 0;
	virtual bool unbindCode(text::QScintillaTextEdit *code) = 0;
	virtual text::QScintillaTextEdit *code(const QString &filePath) const = 0;
	virtual QList<text::QScintillaTextEdit *> code(const Id &diagram) const = 0;
	virtual bool contains(const QString &filePath) const = 0;
	virtual Id diagram(text::QScintillaTextEdit *code) const = 0;
	virtual QString path(text::QScintillaTextEdit *code) const = 0;
	virtual bool isDefaultPath(const QString &path) const = 0;
	virtual bool isModified(const QString &path) const = 0;
	virtual bool isModifiedEver(const QString &path) const = 0;
	virtual void showInTextEditor(const QFileInfo &fileInfo
			, const QString &genName, const text::LanguageInfo &language) = 0;
	virtual void showInTextEditor(const QFileInfo &fileInfo, const text::LanguageInfo &language) = 0;
	virtual bool saveText(bool saveAs) = 0;
	virtual QString generatorName(const QString &filepath) const  = 0;

signals:
	void textChanged(bool changed);
};

}
