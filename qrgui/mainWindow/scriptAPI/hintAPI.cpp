#include "hintAPI.h"

#include <QtCore/QtMath>

#include <qrutils/mathUtils/math.h>

#include "scriptAPI.h"
#include "arrow.h"
#include "hintReporter.h"

using namespace qReal;
using namespace gui;
using namespace mathUtils;

HintAPI::HintAPI()
{
}

void HintAPI::addHint(const QString &message, int lifeTime, QWidget *parent)
{
	// The message will show and dispose itself.
	new HintReporter(parent, message, lifeTime);
}

void HintAPI::arrowToWidget(QWidget *target, qreal angle, int lifeTime, QWidget *parent)
{
	const int xcoord = target->mapToGlobal(QPoint(0, 0)).x();
	const int ycoord = target->mapToGlobal(QPoint(0, 0)).y();

	const int sourcePointDeviation = 50;

	const qreal angleInRads = angle * pi / 180;

	const QPoint sourcePoint(xcoord + sourcePointDeviation * qCos(angleInRads)
			, ycoord + sourcePointDeviation * qSin(angleInRads));
	const QPoint destPoint(xcoord, ycoord);

	// The arrow will show and dispose itself.
	new Arrow(sourcePoint, destPoint, lifeTime, parent);
}
