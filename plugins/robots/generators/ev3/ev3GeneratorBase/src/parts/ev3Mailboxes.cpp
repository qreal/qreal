/* Copyright 2017 QReal Research Group
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

#include "ev3GeneratorBase/parts/ev3Mailboxes.h"

using namespace ev3;
using namespace ev3::parts;

const QMap<QString, QString> EV3_TYPE_ID = {{"int", "2"}, {"bool", "0"}, {"string", "4"}, {"float", "3"}};
const QMap<QString, QString> EV3_TYPE_LENGTH = {{"int", "4"}, {"bool", "1"}, {"string", "255"}, {"float", "4"}};

parts::Mailboxes::Mailboxes(const QStringList &pathsToTemplates)
	:TemplateParametrizedEntity(pathsToTemplates)
{
}

void parts::Mailboxes::reinit()
{
	mReadMailboxNameToTypeMap.clear();
	mWriteMailboxNameToTypeMap.clear();
	mMailboxNameToId.clear();
}

QString parts::Mailboxes::generateOpening() const
{
	const QString openTemplate = readTemplate("mailboxes/openMailbox.t");
	QString res = "";
	QMapIterator<QString, QString> iterator(mReadMailboxNameToTypeMap);
	while (iterator.hasNext()) {
		iterator.next();
		QString tmpTemplate = openTemplate;
		res += tmpTemplate.replace("@@MAILBOX_NAME@@", iterator.key())
				.replace("@@ID@@", mMailboxNameToId[iterator.key()])
				.replace("@@TYPE@@", iterator.value());
		res += "\n";
	}

	return res;
}

QString parts::Mailboxes::generateClosing() const
{
	const QString closeTemplate = readTemplate("mailboxes/closeMailbox.t");
	QString res = "";
	QMapIterator<QString, QString> iterator(mReadMailboxNameToTypeMap);
	while (iterator.hasNext()) {
		iterator.next();
		QString tmpTemplate = closeTemplate;
		res += tmpTemplate.replace("@@ID@@", mMailboxNameToId[iterator.key()]);
		res += "\n";
	}

	return res;
}

QMap<QString, QString> &parts::Mailboxes::readMailboxes()
{
	return mReadMailboxNameToTypeMap;
}

QMap<QString, QString> &parts::Mailboxes::writeMailboxes()
{
	return mWriteMailboxNameToTypeMap;
}

int parts::Mailboxes::mailboxesCount() const
{
	return mReadMailboxNameToTypeMap.size();
}

bool parts::Mailboxes::tryRegisterReadMailbox(const QString &name, const QString &type)
{
	if (!mReadMailboxNameToTypeMap.contains(name)) {
		mReadMailboxNameToTypeMap[name] = type;
		mMailboxNameToId[name] = QString::number(mReadMailboxNameToTypeMap.size());
		return true;
	}

	return mReadMailboxNameToTypeMap[name] == type;
}

bool parts::Mailboxes::tryRegisterWriteMailbox(const QString &name, const QString &type)
{
	if (!mWriteMailboxNameToTypeMap.contains(name)) {
		mWriteMailboxNameToTypeMap[name] = type;
		return true;
	}

	return mWriteMailboxNameToTypeMap[name] == type;
}

QString parts::Mailboxes::mailboxNameToId(const QString &name) const
{
	return mMailboxNameToId[name];
}

QString parts::Mailboxes::messageTypeToTypeLength(const QString &typeName) const
{
	return EV3_TYPE_LENGTH[typeName];
}

QString parts::Mailboxes::typeToEv3Type(const QString &typeName) const
{
	return EV3_TYPE_ID[typeName];
}
