#include "association.h"

#include <QDebug>

bool Association::init(QDomElement const &element)
{
	mBeginName = element.attribute("beginName");
	mEndName = element.attribute("endName");
	if ((mBeginName == "") || (mEndName == ""))
	{
		qDebug() << "Error: can't parse association";
		return false;
	}
	return true;
}