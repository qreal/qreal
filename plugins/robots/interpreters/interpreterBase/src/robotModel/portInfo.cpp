#include "interpreterBase/robotModel/portInfo.h"

using namespace interpreterBase::robotModel;

PortInfo::PortInfo()
{
}

PortInfo::PortInfo(const QString &name, Direction direction
		, const QStringList &nameAliases, const QString &reservedVariableName
		, ReservedVariableType reservedVariableType)
	: mName(name)
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
		// Q_ASSERT?
		return PortInfo();
	}

	return PortInfo(parts[0], parts[1] == "input" ? input : output
			, parts[2].split("$$$", QString::SkipEmptyParts), parts[3]);
}
