#include "nullTextManager.h"

using namespace qReal;

bool NullTextManager::openFile(const QString &filePath
		, const QString &generatorName, const text::LanguageInfo &language)
{
	Q_UNUSED(filePath)
	Q_UNUSED(generatorName)
	Q_UNUSED(language)
	return true;
}

bool NullTextManager::bindCode(const Id &diagram, const QString &filePath)
{
	Q_UNUSED(diagram)
	Q_UNUSED(filePath)
	return true;
}

bool NullTextManager::unbindCode(const QString &filePath)
{
	Q_UNUSED(filePath)
	return true;
}

bool NullTextManager::unbindCode(text::QScintillaTextEdit *code)
{
	Q_UNUSED(code)
	return true;
}

bool NullTextManager::closeFile(const QString &filePath)
{
	Q_UNUSED(filePath)
	return true;
}

void NullTextManager::changeFilePath(const QString &from, const QString &to)
{
	Q_UNUSED(from)
	Q_UNUSED(to)
}

text::QScintillaTextEdit *NullTextManager::code(const QString &filePath) const
{
	Q_UNUSED(filePath)
	return nullptr;
}

QList<text::QScintillaTextEdit *> NullTextManager::code(const Id &diagram) const
{
	Q_UNUSED(diagram)
	return {};
}

bool NullTextManager::contains(const QString &filePath) const
{
	Q_UNUSED(filePath)
	return false;
}

Id NullTextManager::diagram(text::QScintillaTextEdit *code) const
{
	Q_UNUSED(code)
	return Id();
}

QString NullTextManager::path(text::QScintillaTextEdit *code) const
{
	Q_UNUSED(code)
	return QString();
}

bool NullTextManager::isDefaultPath(const QString &path) const
{
	Q_UNUSED(path)
	return true;
}

bool NullTextManager::isModified(const QString &path) const
{
	Q_UNUSED(path)
	return false;
}

bool NullTextManager::isModifiedEver(const QString &path) const
{
	Q_UNUSED(path)
	return false;
}

void NullTextManager::showInTextEditor(const QFileInfo &fileInfo
		, const QString &genName, const text::LanguageInfo &language)
{
	Q_UNUSED(fileInfo)
	Q_UNUSED(genName)
	Q_UNUSED(language)
}

void NullTextManager::showInTextEditor(const QFileInfo &fileInfo, const text::LanguageInfo &language)
{
	Q_UNUSED(fileInfo)
	Q_UNUSED(language)
}

bool NullTextManager::saveText(bool saveAs)
{
	Q_UNUSED(saveAs)
	return true;
}

QString NullTextManager::generatorName(const QString &filePath) const
{
	Q_UNUSED(filePath)
	return QString();
}
