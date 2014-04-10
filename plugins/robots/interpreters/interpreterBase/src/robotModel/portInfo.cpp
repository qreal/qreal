#include "interpreterBase/robotModel/portInfo.h"

using namespace interpreterBase::robotModel;

PortInfo::PortInfo()
{
}

PortInfo::PortInfo(QString const &name, QStringList const &nameAliases, QString const &reservedVariableName)
	: mName(name)
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
	return QString("%1###%2###%3").arg(mName, mNameAliases.join("$$$"), mReservedVariable);
}

PortInfo PortInfo::fromString(QString const &string)
{
	QStringList const parts = string.split("###");
	if (parts.count() != 3) {
		// Q_ASSERT?
		return PortInfo();
	}

	return PortInfo(parts[0], parts[1].split("$$$", QString::SkipEmptyParts), parts[2]);
}
