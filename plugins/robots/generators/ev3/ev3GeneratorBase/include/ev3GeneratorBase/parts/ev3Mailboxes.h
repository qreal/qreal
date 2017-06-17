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

#pragma once

#include <QtCore/QString>
#include <QtCore/QMap>

#include <generatorBase/templateParametrizedEntity.h>

#include "ev3GeneratorBase/ev3GeneratorBaseDeclSpec.h"


namespace ev3 {
namespace parts {

/// Class responsible for everything with images generation
class ROBOTS_EV3_GENERATOR_BASE_EXPORT Mailboxes : public generatorBase::TemplateParametrizedEntity
{
public:
	explicit Mailboxes(const QStringList &pathsToTemplates);

	/// Prepares this instance for new usage
	void reinit();

	/// Generates code for mailboxes declaration inside the program
	QString generateOpening() const;

	/// Generates code for mailboxes closing inside the program. WARNING: call only after generateOpening()
	QString generateClosing() const;

	/// Returns a resulting set of read mailboxes with types (map<Name, Type>)
	QMap<QString, QString> &readMailboxes();

	/// Returns a resulting set of write mailboxes with types (map<Name, Type>)
	QMap<QString, QString> &writeMailboxes();

	/// Returns the count of registered in the moment read mailboxes
	int mailboxesCount() const;

	/// Tries add new read mailbox to program.
	/// Returns false if mailbox already exists or old and new types are not equal.
	bool tryRegisterReadMailbox(const QString &name, const QString &type);

	/// Tries add new write mailbox to program.
	/// Returns false if mailbox already exists or old and new types are not equal.
	bool tryRegisterWriteMailbox(const QString &name, const QString &type);

	/// Returns id of given mailbox (read only mailbox).
	QString mailboxNameToId(const QString &name) const;

	/// Returns type length of given message type.
	QString messageTypeToTypeLength(const QString &name, bool readBox = true) const;

	/// Returns ev3 type encoding.
	QString typeToEv3Type(const QString &typeName) const;

private:
	QMap<QString, QString> mReadMailboxNameToTypeMap;
	QMap<QString, QString> mWriteMailboxNameToTypeMap;
	QMap<QString, QString> mMailboxNameToId;
};

}
}
