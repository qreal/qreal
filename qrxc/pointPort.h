#pragma once

#include "port.h"
#include "../qrutils/scalableCoordinate.h"

#include <QDomElement>

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
