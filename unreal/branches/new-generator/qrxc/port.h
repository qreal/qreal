#pragma once

#include <QDomElement>

class OutFile;

class Port
{
public:
	virtual bool init(QDomElement const &element, int width, int height) = 0;
	virtual void generateCode(OutFile &out) = 0;
};
