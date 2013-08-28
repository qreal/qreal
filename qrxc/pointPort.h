#pragma once

#include <QtXml/QDomElement>

#include <qrutils/scalableCoordinate.h>

#include "port.h"

namespace utils {
	class OutFile;
}

class PointPort : public Port
{
public:
	virtual bool init(QDomElement const &element, int width, int height);
	virtual void generateCode(utils::OutFile &out, QStringList const &portTypes);
	virtual Port* clone() const;

private:
	utils::ScalableCoordinate mX;
	utils::ScalableCoordinate mY;
	int mInitWidth;
	int mInitHeight;
};
