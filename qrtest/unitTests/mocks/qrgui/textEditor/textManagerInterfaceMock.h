/* Copyright 2016 CyberTech Labs Ltd.
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

#include <QtCore/QString>

#include <qrgui/textEditor/textManagerInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

/// Mock class for QrGui text manager.
class TextManagerInterfaceMock : public qReal::TextManagerInterface
{
public:
	MOCK_METHOD3(openFile, bool(const QString &, const QString &, const qReal::text::LanguageInfo &));
	MOCK_METHOD1(closeFile, bool(const QString &));
	MOCK_METHOD2(changeFilePath, void(const QString &, const QString &));
	MOCK_METHOD2(bindCode, bool(const qReal::Id &, const QString &));
	MOCK_METHOD1(unbindCode, bool(const QString &));
	MOCK_METHOD1(unbindCode, bool(qReal::text::QScintillaTextEdit *));
	MOCK_CONST_METHOD1(code, qReal::text::QScintillaTextEdit *(const QString &));
	MOCK_CONST_METHOD1(code, QList<qReal::text::QScintillaTextEdit *>(const qReal::Id &));
	MOCK_CONST_METHOD1(contains, bool(const QString &));
	MOCK_CONST_METHOD1(diagram, qReal::Id(qReal::text::QScintillaTextEdit *));
	MOCK_CONST_METHOD1(path, QString(qReal::text::QScintillaTextEdit *));
	MOCK_CONST_METHOD1(isDefaultPath, bool(const QString &));
	MOCK_CONST_METHOD1(isModified, bool(const QString &));
	MOCK_CONST_METHOD1(isModifiedEver, bool(const QString &));
	MOCK_METHOD3(showInTextEditor, void(const QFileInfo &, const QString &, const qReal::text::LanguageInfo &));
	MOCK_METHOD2(showInTextEditor, void(const QFileInfo &, const qReal::text::LanguageInfo &));
	MOCK_METHOD1(saveText, bool(bool));
	MOCK_CONST_METHOD1(generatorName, QString(const QString &));
};

}
