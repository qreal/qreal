#include "interpreterBase/robotModel/portInfo.h"

using namespace interpreterBase::robotModel;

PortInfo::PortInfo(QString const &name, Direction direction, QStringList const &nameAliases)
	: mName(name)
	, mDirection(direction)
	, mNameAliases(nameAliases)
{
}

QString PortInfo::name() const
{
	return mName;
}

PortInfo::Direction PortInfo::direction() const
{
	return mDirection;
}

QStringList PortInfo::nameAliases() const
{
	return mNameAliases;
}
