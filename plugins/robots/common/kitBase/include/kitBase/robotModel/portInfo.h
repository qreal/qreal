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

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>

#include "direction.h"
#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {

/// A class describing a port on the robot where different devices can be plugged.
class ROBOTS_KIT_BASE_EXPORT PortInfo
{
public:
	enum class ReservedVariableType {
		scalar
		, vector
	};

	/// Deserializes PortInfo instance from the string obtained by toString() method.
	static PortInfo fromString(const QString &string);

	/// Constructs invalid PortInfo instance.
	PortInfo();

	/// Constructs new PortInfo instance. If name is empty then the port is considered to be invalid.
	/// Ports with same names but different directions are considered to be different.
	PortInfo(const QString &name, Direction direction
			, const QStringList &nameAliases = QStringList()
			, const QString &reservedVariableName = QString()
			, ReservedVariableType reservedVariableType = ReservedVariableType::scalar);

	/// Constructs new PortInfo instance with given user-friendly name.
	PortInfo(const QString &name, const QString &userFriendlyName, Direction direction
			, const QStringList &nameAliases = QStringList()
			, const QString &reservedVariableName = QString()
			, ReservedVariableType reservedVariableType = ReservedVariableType::scalar);

	/// Returns true if this PortInfo is non-empty (i.e. really describes some port)
	bool isValid() const;

	/// Returns internal name of the port.
	QString name() const;

	/// Returns user-friendly name of the port.
	QString userFriendlyName() const;

	/// Returns the direction of this port. Ports with similar names but different
	/// directions are considered to be different.
	Direction direction() const;

	/// Returns a list of names that will be treated as corresponding to the same port as name() does.
	/// Useful for compability of diagrams for the different robot models (e.g. 'M1' for TRIK and 'A' for NXT).
	QStringList nameAliases() const;

	/// Returns a name of the reserved variable for this port (that varible can be used to access
	/// devices values on this port).
	QString reservedVariable() const;

	ReservedVariableType reservedVariableType() const;

	/// Serializes this PortInfo instance into the inner string representation.
	QString toString() const;

private:
	QString mName;
	QString mUserFriendlyName;
	Direction mDirection;
	QStringList mNameAliases;
	QString mReservedVariable;
	ReservedVariableType mReservedVariableType;
};

inline bool operator ==(const PortInfo &left, const PortInfo &right)
{
	return left.name() == right.name()
			&& left.direction() == right.direction();
}

inline bool operator !=(const PortInfo &left, const PortInfo &right)
{
	return !(left == right);
}

inline uint qHash(const PortInfo &key)
{
	return qHash(key.name()) ^ qHash(QString(key.direction() == input ? "input" : "output"));
}

inline bool operator <(const PortInfo &left, const PortInfo &right)
{
	if (left.name() == right.name()) {
		return left.direction() == input && right.direction() == output;
	}

	return left.name() < right.name();
}

}
}

Q_DECLARE_METATYPE(kitBase::robotModel::PortInfo)
