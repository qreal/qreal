#include "interpreterBase/robotModel/portInfo.h"

using namespace interpreterBase::robotModel;

PortInfo::PortInfo(QString const &name, QStringList const &nameAliases, Direction direction)
	: mName(name)
	, mNameAliases(nameAliases)
	, mDirection(direction)
{
}

QString const &PortInfo::name() const
{
	return mName;
}

QStringList const &PortInfo::nameAliases() const
{
	return mNameAliases;
}

PortInfo::Direction PortInfo::direction() const
{
	return mDirection;
}
