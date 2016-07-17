/* Copyright 2015 Dmitry Kopytov
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

#include <QtCore/QFileInfo>

#include "codeBlockManager.h"

using namespace qReal;
using namespace text;

CodeBlockManager::CodeBlockManager()
{
}

void CodeBlockManager::addNewCode(const QString &path)
{
	readDbgFile(path);
}

void CodeBlockManager::removeCode(const QString &path)
{
	mBlockToCode.remove(path);
}

void CodeBlockManager::setActive(const QString &path, bool active)
{
	mActive.insert(path, active);
}

QPair<int, int> CodeBlockManager::intervalById(const QString &path, const Id &id) const
{
	return mActive.value(path, false) && mBlockToCode.contains(path)
			? mBlockToCode[path].value(id, QPair<int, int>()) : QPair<int, int>();
}

QList<Id> CodeBlockManager::idsByLineNumber(const QString &path, int lineNumber) const
{
	return mActive.value(path, false) && mCodeToBlock.contains(path)
			? mCodeToBlock[path].value(lineNumber, QList<Id>()) : QList<Id>();
}

void CodeBlockManager::readDbgFile(const QString &path)
{
	QFile file(path + ".dbg");

	if(!file.open(QIODevice::ReadOnly)) {
		return;
	}

	QTextStream in(&file);

	mBlockToCode.insert(path, QMap<Id, QPair<int, int> >());
	mCodeToBlock.insert(path, QMap<int, QList<Id> >());
	mActive.insert(path, true);

	while (!in.atEnd()) {
		QString line = in.readLine();
		QStringList fields = line.split("@");
		Id id = Id::loadFromString(fields[0]);
		const int from = fields[1].toInt();
		const int to = fields[2].toInt();

		mBlockToCode[path].insert(id, QPair<int, int>(from, to));

		for (int i = from; i <= to; ++i) {
			if (!mCodeToBlock[path].contains(i)) {
				mCodeToBlock[path].insert(i, QList<Id>());
			}

			mCodeToBlock[path][i].append(id);
		}
	}

	file.close();
}
