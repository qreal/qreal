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

#include "kitBase/robotModel/portInfo.h"

using namespace kitBase::robotModel;

PortInfo::PortInfo()
	: mDirection(output)
	, mReservedVariableType(ReservedVariableType::scalar)
{
}

PortInfo::PortInfo(const QString &name, Direction direction
		, const QStringList &nameAliases, const QString &reservedVariableName
		, ReservedVariableType reservedVariableType)
	: PortInfo(name, name, direction, nameAliases, reservedVariableName, reservedVariableType)
{
}

PortInfo::PortInfo(const QString &name, const QString &userFriendlyName, Direction direction
		, const QStringList &nameAliases
		, const QString &reservedVariableName
		, ReservedVariableType reservedVariableType)
	: mName(name)
	, mUserFriendlyName(userFriendlyName)
	, mDirection(direction)
	, mNameAliases(nameAliases)
	, mReservedVariable(reservedVariableName)
	, mReservedVariableType(reservedVariableType)
{
}

bool PortInfo::isValid() const
{
	return !mName.isEmpty();
}

QString PortInfo::name() const
{
	return mName;
}

QString PortInfo::userFriendlyName() const
{
	return mUserFriendlyName;
}

Direction PortInfo::direction() const
{
	return mDirection;
}

QStringList PortInfo::nameAliases() const
{
	return mNameAliases;
}

QString PortInfo::reservedVariable() const
{
	return mReservedVariable;
}

PortInfo::ReservedVariableType PortInfo::reservedVariableType() const
{
	return mReservedVariableType;
}

QString PortInfo::toString() const
{
	return QString("%1###%2###%3###%4").arg(mName, mDirection == input ? "input" : "output"
			, mNameAliases.join("$$$"), mReservedVariable);
}

PortInfo PortInfo::fromString(const QString &string)
{
	const QStringList parts = string.split("###");
	if (parts.count() != 4) {
		return PortInfo();
	}

	return PortInfo(parts[0], parts[1] == "input" ? input : output
			, parts[2].split("$$$", QString::SkipEmptyParts), parts[3]);
}
