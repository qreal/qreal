#pragma once

#include "port.h"

#include <QDomElement>

namespace utils {
	class OutFile;
}

class PointPort : public Port
{
public:
	virtual bool init(QDomElement const &element, int width, int height);
	virtual void generateCode(utils::OutFile &out);
	virtual Port* clone() const;

private:
	qreal mX;
	qreal mY;
};
