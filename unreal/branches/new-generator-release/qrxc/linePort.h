#pragma once

#include <QDomElement>

#include "port.h"
#include "scalableCoordinate.h"

class OutFile;

class LinePort : public Port
{
public:
	virtual bool init(QDomElement const &element, int width, int height);
	virtual void generateCode(OutFile &out);

private:
	ScalableCoordinate mStartX;
	ScalableCoordinate mStartY;
	ScalableCoordinate mEndX;
	ScalableCoordinate mEndY;

	void initCoordinate(ScalableCoordinate &field, QString &coordinate, int maxValue);
};
