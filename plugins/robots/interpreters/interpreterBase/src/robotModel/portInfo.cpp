#include "interpreterBase/robotModel/portInfo.h"

using namespace interpreterBase::robotModel;

PortInfo::PortInfo()
{
}

PortInfo::PortInfo(QString const &name, Direction direction
		, QStringList const &nameAliases, QString const &reservedVariableName)
	: mName(name)
	, mDirection(direction)
	, mNameAliases(nameAliases)
	, mReservedVariable(reservedVariableName)
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

QString PortInfo::toString() const
{
	return QString("%1###%2###%3###%4").arg(mName, mDirection == input ? "input" : "output"
			, mNameAliases.join("$$$"), mReservedVariable);
}

PortInfo PortInfo::fromString(QString const &string)
{
	QStringList const parts = string.split("###");
	if (parts.count() != 4) {
		// Q_ASSERT?
		return PortInfo();
	}

	return PortInfo(parts[0], parts[1] == "input" ? input : output
			, parts[2].split("$$$", QString::SkipEmptyParts), parts[3]);
}
