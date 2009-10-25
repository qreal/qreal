#pragma once

#include "port.h"

#include <QDomElement>

class OutFile;

class PointPort : public Port
{
public:
	virtual bool init(QDomElement const &element, int width, int height);
	virtual void generateCode(OutFile &out);

private:
	qreal mX;
	qreal mY;
};
