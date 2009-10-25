#pragma once

#include <QDomElement>

class Association
{
public:
	bool init(QDomElement const &element);

private:
	QString mBeginName;
	QString mEndName;
};
