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

#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>
#include <QtCore/QFile>

#include <qrutils/outFile.h>
#include <qrutils/qRealFileDialog.h>

#include <plugins/toolPluginInterface/systemEvents.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>

#include "textManager.h"
#include "qscintillaTextEdit.h"

using namespace qReal;
using namespace text;

TextManager::TextManager(SystemEvents &systemEvents, gui::MainWindowInterpretersInterface &mainWindow)
	: mMainWindow(mainWindow)
	, mSystemEvents(systemEvents)
{
	connect(&mSystemEvents, &SystemEvents::codeTabClosed, this, &TextManager::onTabClosed);
}

bool TextManager::openFile(const QString &filePath, const QString &generatorName, const text::LanguageInfo &language)
{
	QFile file(filePath);
	QTextStream *inStream = nullptr;

	if (!file.isOpen() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		inStream = new QTextStream(&file);
		inStream->setCodec(QTextCodec::codecForName("UTF-8"));

		QScintillaTextEdit *area = new QScintillaTextEdit();

		area->setCurrentLanguage(language);
		area->setText(inStream->readAll());
		mText.insert(filePath, area);
		mPath.insert(area, filePath);
		mPathType.insert(filePath, true);
		mModified.insert(filePath, QPair<bool, bool>(false, false));
		mGeneratorName.insert(filePath, generatorName);

		connect(area, SIGNAL(textWasModified(text::QScintillaTextEdit*))
				, this, SLOT(setModified(text::QScintillaTextEdit*)));

		return true;
	}

	return false;
}

bool TextManager::bindCode(const Id &diagram,  const QString &filePath)
{
	if (mText.contains(filePath)) {
		mDiagramCodeManager.insert(diagram, filePath);
		return true;
	}
	return false;
}

bool TextManager::unbindCode(const QString &filePath)
{
	return mDiagramCodeManager.remove(mDiagramCodeManager.key(filePath), filePath) != 0;
}

bool TextManager::unbindCode(text::QScintillaTextEdit *code)
{
	return unbindCode(mPath.value(code));
}

bool TextManager::closeFile(const QString &filePath)
{
	mPath.remove(mText.value(filePath));
	mPathType.remove(filePath);
	mModified.remove(filePath);
	mGeneratorName.remove(filePath);
	unbindCode(filePath);
	return mText.remove(filePath);
}

void TextManager::changeFilePath(const QString &from, const QString &to)
{
	QScintillaTextEdit *code = mText.value(from);
	QPair<bool, bool> mod(true, false);
	const QString genName = generatorName(from);
	const Id diagram = mDiagramCodeManager.key(from);

	closeFile(from);

	mText.insert(to, code);
	mPath.insert(code, to);
	mPathType.insert(to, false);
	mModified.insert(to, mod);
	mGeneratorName.insert(to, genName);

	if (!diagram.isNull()) {
		bindCode(diagram, to);
	}
}

QScintillaTextEdit *TextManager::code(const QString &filePath) const
{
	return mText.value(filePath);
}

QList<text::QScintillaTextEdit *> TextManager::code(const Id &diagram) const
{
	QList<text::QScintillaTextEdit *> codeList;

	for (const QString &filePath : mDiagramCodeManager.values(diagram)) {
		codeList += mText.value(filePath);
	}

	return codeList;
}

bool TextManager::contains(const QString &filePath) const
{
	return mText.contains(filePath);
}

bool TextManager::removeDiagram(const Id &diagram)
{
	return mDiagramCodeManager.remove(diagram);
}

Id TextManager::diagram(text::QScintillaTextEdit *code) const
{
	return mDiagramCodeManager.key(mPath.value(code));
}

QString TextManager::path(text::QScintillaTextEdit *code) const
{
	return mPath.value(code);
}

bool TextManager::isDefaultPath(const QString &path) const
{
	return mPathType.value(path);
}

bool TextManager::isModified(const QString &path) const
{
	return mModified.value(path).second;
}

bool TextManager::isModifiedEver(const QString &path) const
{
	return mModified.value(path).first;
}

void TextManager::setModified(text::QScintillaTextEdit *code, bool modified)
{
	QPair<bool, bool> mod = mModified.value(mPath.value(code));
	mod.first = !modified || mod.first;
	mod.second = modified && code->isUndoAvailable();
	code->setModified(mod.second);
	mModified.insert(mPath.value(code), mod);

	emit textChanged(modified && code->isUndoAvailable());
}

void TextManager::onTabClosed(const QFileInfo &file)
{
	closeFile(file.absoluteFilePath());
}

void TextManager::showInTextEditor(const QFileInfo &fileInfo
		, const QString &genName, const text::LanguageInfo &language)
{
	/// @todo: Uncomment it
	// Q_ASSERT(!fileInfo.baseName().isEmpty());

	if (!mMainWindow.activeDiagram().isNull()) {
		const QString filePath = fileInfo.absoluteFilePath();

		if (contains(filePath)) {
			mMainWindow.closeTab(code(filePath));
		}

		openFile(filePath, genName, language);
		QScintillaTextEdit *area = code(filePath);
		area->show();
		bindCode(mMainWindow.activeDiagram(), filePath);
		emit mSystemEvents.newCodeAppeared(mMainWindow.activeDiagram(), QFileInfo(filePath));

		mMainWindow.openTab(area, fileInfo.fileName());
	}
}

void TextManager::showInTextEditor(const QFileInfo &fileInfo, const text::LanguageInfo &language)
{
	Q_ASSERT(!fileInfo.completeBaseName().isEmpty());

	const QString filePath = fileInfo.absoluteFilePath();

	if (contains(filePath)) {
		mMainWindow.closeTab(code(filePath));
	}

	openFile(filePath, QString(), language);
	QScintillaTextEdit *area = code(filePath);
	if (!area) {
		return;
	}

	area->show();

	mMainWindow.openTab(area,  fileInfo.fileName());
}

bool TextManager::saveText(bool saveAs)
{
	QScintillaTextEdit * const area = dynamic_cast<QScintillaTextEdit *>(mMainWindow.currentTab());
	if (!area) {
		return false;
	}

	const Id diagram = TextManager::diagram(area);
	QFileInfo fileInfo;
	const QString filepath = path(area);
	const bool defaultPath = isDefaultPath(filepath);

	QString editorExtension = QString("%1 (*.%2)").arg(
			area->currentLanguage().extensionDescription
			, area->currentLanguage().extension);
	const QString extensionDescriptions = editorExtension + ";;" + tr("All files (*)");
	QString *currentExtensionDescription = &editorExtension;

	if (saveAs) {
		fileInfo = QFileInfo(utils::QRealFileDialog::getSaveFileName("SaveTextFromTextManager"
				, mMainWindow.windowWidget()
				, tr("Save generated code")
				, QString()
				, extensionDescriptions
				, QString()
				, currentExtensionDescription));
	} else {
		fileInfo = path(area);
	}

	if (!fileInfo.fileName().isEmpty()) {
		mMainWindow.setTabText(area, fileInfo.fileName());

		utils::OutFile out(fileInfo.absoluteFilePath());

		out() << area->text();

		if (defaultPath || saveAs) {
			changeFilePath(path(area), fileInfo.absoluteFilePath());
		}

		setModified(area, false);

		if (saveAs && !diagram.isNull()) {
			emit mSystemEvents.codePathChanged(diagram, path(area), fileInfo);
		}
	}

	return true;
}

QString TextManager::generatorName(const QString &filePath) const
{
	return mGeneratorName.value(filePath, "");
}
