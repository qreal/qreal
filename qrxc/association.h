#pragma once

#include <QtXml/QDomElement>

class Association
{
public:
	bool init(const QDomElement &element);
	// Default copy constructor ok.
	Association* clone() const;

private:
	QString mBeginName;
	QString mEndName;
};
