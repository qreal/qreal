#include "hintAPI.h"

#include <QtCore/QtMath>

#include "scriptAPI.h"
#include "arrow.h"
#include "hintReporter.h"

#include <qrutils/mathUtils/math.h>

using namespace qReal;
using namespace gui;
using namespace mathUtils;

HintAPI::HintAPI(ScriptAPI *scriptAPI)
	: mScriptAPI(scriptAPI)
	, mHintReporter(nullptr)
	, mArrow(nullptr)
{
}

void HintAPI::addHint(QString const &message, int const lifeTime, QWidget *parent)
{
	mHintReporter = new HintReporter(parent, message, lifeTime);
}

void HintAPI::arrowToWidget(QWidget *target, qreal angle, int lifeTime, QWidget *parent)
{
	int const xcoord = target->mapToGlobal(QPoint(0,0)).x();
	int const ycoord = target->mapToGlobal(QPoint(0,0)).y();

	int const sourcePointDeviation = 50;

	qreal const angleInRads = angle * pi / 180;

	QPoint const sourcePoint = QPoint(xcoord + sourcePointDeviation * qCos(angleInRads)
									, ycoord + sourcePointDeviation * qSin(angleInRads));
	QPoint const destPoint = QPoint(xcoord, ycoord);

	mArrow = new Arrow(sourcePoint, destPoint, lifeTime, parent);
}
