#pragma once

#include <QtXml/QDomElement>

#include <qrutils/scalableCoordinate.h>

#include "port.h"

namespace utils {
	class OutFile;
}

class LinePort : public Port
{
public:
	virtual bool init(const QDomElement &element, int width, int height);
	virtual void generateCode(utils::OutFile &out, const QStringList &portTypes);
	virtual Port* clone() const;

private:
	utils::ScalableCoordinate mStartX;
	utils::ScalableCoordinate mStartY;
	utils::ScalableCoordinate mEndX;
	utils::ScalableCoordinate mEndY;
	int mInitWidth;
	int mInitHeight;
};
