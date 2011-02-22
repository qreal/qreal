#pragma once

#include <QtCore/QString>

namespace qrmc {
class ScalableCoordinate
{
public:
	ScalableCoordinate();
	ScalableCoordinate(qreal value, int width, bool isScalable);
	qreal value() const;
	bool isScalable() const;

	QString toString(bool isScaled) const;

	QString getScalability() const;

private:
	qreal mValue;
	int mWidth;
	bool mIsScalable;
};
}
