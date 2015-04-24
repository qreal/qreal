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

#include <qrgui/textEditor/textManagerInterface.h>

#include "qrgui/systemFacade/systemFacadeDeclSpec.h"

namespace qReal {

class QRGUI_SYSTEM_FACADE_EXPORT NullTextManager : public TextManagerInterface
{
	Q_OBJECT

public:
	bool openFile(const QString &filePath, const QString &genName, const text::LanguageInfo &language) override;
	bool closeFile(const QString &filePath) override;

	void changeFilePath(const QString &from, const QString &to) override;

	bool bindCode(const Id &diagram, const QString &filePath) override;
	bool unbindCode(const QString &filePath) override;
	bool unbindCode(text::QScintillaTextEdit *code) override;

	text::QScintillaTextEdit *code(const QString &filePath) const override;
	QList<text::QScintillaTextEdit *> code(const Id &diagram) const override;

	bool contains(const QString &filePath) const override;
	Id diagram(text::QScintillaTextEdit *code) const override;
	QString path(text::QScintillaTextEdit *code) const override;
	bool isDefaultPath(const QString &path) const override;
	bool isModified(const QString &path) const override;
	bool isModifiedEver(const QString &path) const override;

	void showInTextEditor(const QFileInfo &fileInfo, const QString &genName
			, const text::LanguageInfo &language) override;
	void showInTextEditor(const QFileInfo &fileInfo, const text::LanguageInfo &language) override;

	bool saveText(bool saveAs) override;

	QString generatorName(const QString &filepath) const override;
};

}
