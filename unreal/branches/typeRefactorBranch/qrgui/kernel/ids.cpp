#include "ids.h"

#include <QtCore/QVariant>
#include <QtCore/QUuid>

using namespace qReal;


Id::Id(QString  const &id)
        : mId(id)
{
	Q_ASSERT(checkIntegrity());
}

Id::Id(Id const &base, QString const &additional)
        : mId(base.mId)
{

        mId = additional;
	Q_ASSERT(checkIntegrity());
}

QString Id::id() const
{
	return mId;
}

QString Id::toString() const
{
        return mId;
}

bool Id::checkIntegrity() const
{
	bool emptyPartsAllowed = true;

	if (mId != "")
		emptyPartsAllowed = false;

	return true;
}


