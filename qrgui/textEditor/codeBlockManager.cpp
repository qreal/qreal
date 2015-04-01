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

QPair<int, int> CodeBlockManager::intervalById(QString const &path, Id const &id)
{
	return mBlockToCode.contains(path) ? mBlockToCode[path].value(id, QPair<int, int>()) : QPair<int, int>();
}

void CodeBlockManager::readDbgFile(QString const &path)
{
	QFile file(path + ".dbg");

	if(!file.open(QIODevice::ReadOnly)) {
		return;
	}

	QTextStream in(&file);

	mBlockToCode.insert(path, QMap<Id, QPair<int, int> >());

	while(!in.atEnd()) {
		QString line = in.readLine();
		QStringList fields = line.split("@");
		mBlockToCode[path].insert(Id::loadFromString(fields[0])
				, QPair<int, int>(fields[1].toInt(), fields[2].toInt()));
	}

	file.close();
}
