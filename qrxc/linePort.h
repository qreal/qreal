#pragma once

#include <QDomElement>

#include "port.h"
#include "../qrutils/scalableCoordinate.h"

namespace utils {
	class OutFile;
}

class LinePort : public Port
{
public:
	virtual bool init(QDomElement const &element, int width, int height);
	virtual void generateCode(utils::OutFile &out, QStringList const &portTypes);
	virtual Port* clone() const;

private:
	utils::ScalableCoordinate mStartX;
	utils::ScalableCoordinate mStartY;
	utils::ScalableCoordinate mEndX;
	utils::ScalableCoordinate mEndY;
	int mInitWidth;
	int mInitHeight;
};
