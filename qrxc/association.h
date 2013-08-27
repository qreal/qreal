#pragma once

#include <QtXml/QDomElement>

class Association
{
public:
	bool init(QDomElement const &element);
	// Default copy constructor ok.

private:
	QString mBeginName;
	QString mEndName;
};
