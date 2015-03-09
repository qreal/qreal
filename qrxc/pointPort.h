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
	virtual bool init(const QDomElement &element, int width, int height);
	virtual void generateCode(utils::OutFile &out, const QStringList &portTypes);
	virtual Port* clone() const;

private:
	utils::ScalableCoordinate mX;
	utils::ScalableCoordinate mY;
	int mInitWidth;
	int mInitHeight;
};
