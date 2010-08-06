#pragma once

#include "classes/port.h"

#include <QDomElement>

namespace utils {
	class OutFile;
}
class MetaCompiler;

class PointPort : public Port
{
public:
	virtual bool init(QDomElement const &element, int width, int height);
	virtual Port* clone() const;

	virtual QString generateSdf(MetaCompiler *compiler) const;
	virtual QString generateInit(MetaCompiler *compiler) const;

private:
	qreal mX;
	qreal mY;
	int mWidth;
	int mHeight;
};
