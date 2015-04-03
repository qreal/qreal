#include <QtCore/QFileInfo>

#include "codeBlockManager.h"

using namespace qReal;
using namespace text;

CodeBlockManager::CodeBlockManager()
{
}

void CodeBlockManager::addNewCode(QString const &path)
{
	readDbgFile(path);
}

void CodeBlockManager::removeCode(QString const &path)
{
	mBlockToCode.remove(path);
}

void CodeBlockManager::setActive(QString const &path, bool active)
{
	mActive.insert(path, active);
}

QPair<int, int> CodeBlockManager::intervalById(QString const &path, Id const &id)
{
	return mActive.value(path, false) && mBlockToCode.contains(path)
			? mBlockToCode[path].value(id, QPair<int, int>()) : QPair<int, int>();
}

QList<Id> CodeBlockManager::IdsByLineNumber(QString const &path, int lineNumber)
{
	return mActive.value(path, false) && mCodeToBlock.contains(path)
			? mCodeToBlock[path].value(lineNumber, QList<Id>()) : QList<Id>();
}

void CodeBlockManager::readDbgFile(QString const &path)
{
	QFile file(path + ".dbg");

	if(!file.open(QIODevice::ReadOnly)) {
		return;
	}

	QTextStream in(&file);

	mBlockToCode.insert(path, QMap<Id, QPair<int, int> >());
	mCodeToBlock.insert(path, QMap<int, QList<Id> >());
	mActive.insert(path, true);

	while(!in.atEnd()) {
		QString line = in.readLine();
		QStringList fields = line.split("@");
		Id id = Id::loadFromString(fields[0]);
		int const from = fields[1].toInt();
		int const to = fields[2].toInt();

		mBlockToCode[path].insert(id, QPair<int, int>(from, to));

		for (int i = from; i <= to; i++) {
			if (!mCodeToBlock[path].contains(i)) {
				mCodeToBlock[path].insert(i, QList<Id>());
			}
			mCodeToBlock[path][i].append(id);
		}
	}

	file.close();
}
