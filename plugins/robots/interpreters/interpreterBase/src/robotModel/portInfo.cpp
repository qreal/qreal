#include "interpreterBase/robotModel/portInfo.h"

using namespace interpreterBase::robotModel;

PortInfo::PortInfo()
{
}

PortInfo::PortInfo(QString const &name, QStringList const &nameAliases)
	: mName(name)
	, mNameAliases(nameAliases)
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

QString PortInfo::toString() const
{
	return QString("%1###%2").arg(mName, mNameAliases.join("$$$"));
}

PortInfo PortInfo::fromString(QString const &string)
{
	QStringList const parts = string.split("###");
	if (parts.count() != 2) {
		// Q_ASSERT?
		return PortInfo();
	}

	return PortInfo(parts[0], parts[1].split("$$$"));
}
