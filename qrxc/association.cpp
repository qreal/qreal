#include "association.h"

#include <QtCore/QDebug>

bool Association::init(QDomElement const &element)
{
	mBeginName = element.attribute("beginName");
	mEndName = element.attribute("endName");
	if (mBeginName.isEmpty() || mEndName.isEmpty()) {
		qDebug() << "ERROR: can't parse association 2";
		return false;
	}
	return true;
}
