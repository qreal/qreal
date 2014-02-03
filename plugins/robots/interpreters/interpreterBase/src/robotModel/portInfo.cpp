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
