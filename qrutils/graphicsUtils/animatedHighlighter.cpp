#include "animatedHighlighter.h"

#include <QtCore/QVariantAnimation>

using namespace graphicsUtils;

void AnimatedHighlighter::highlight(QGraphicsItem * const item)
{
	if (!item) {
		return;
	}

	QVariantAnimation *const animation = new QVariantAnimation;
	animation->setDuration(500);
	animation->setStartValue(1.0);
	animation->setEndValue(0.0);
	qreal const oldOpacity = item->opacity();
	QObject::connect(animation, &QVariantAnimation::valueChanged
			, [item](const QVariant &value) { item->setOpacity(value.toReal()); });
	QObject::connect(animation, &QVariantAnimation::finished
			, [item, oldOpacity]() { item->setOpacity(oldOpacity); });
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}
