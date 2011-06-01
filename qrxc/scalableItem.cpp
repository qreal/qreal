#include "scalableItem.h"

ScalableCoordinate ScalableItem::initCoordinate(QString const &coordinate, int maxValue)
{
	ScalableCoordinate field;
	QString coord = coordinate;
	if (coord.endsWith("a")) {
		coord.remove(coord.length() - 1, 1);
		field = ScalableCoordinate((static_cast<qreal>(coord.toInt())) / maxValue, true);
	} else if (coord.endsWith("%")) {
		coord.remove(coord.length() - 1, 1);
		field = ScalableCoordinate((static_cast<qreal>(coord.toInt())) / 100, false);
	} else {
		field = ScalableCoordinate((static_cast<qreal>(coord.toInt())) / maxValue, false);
	}
	return field;
}
