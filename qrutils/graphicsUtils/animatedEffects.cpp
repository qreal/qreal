#include "animatedEffects.h"

#include <QtCore/QVariantAnimation>
#include <QtCore/QPropertyAnimation>
#include <QtWidgets/QGraphicsOpacityEffect>
#include <QtWidgets/QWidget>


using namespace graphicsUtils;

void AnimatedEffects::highlight(QGraphicsItem * const item)
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
			, [item](QVariant const &value) { item->setOpacity(value.toReal()); });
	QObject::connect(animation, &QVariantAnimation::finished
			, [item, oldOpacity]() { item->setOpacity(oldOpacity); });
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}


void AnimatedEffects::disappear(QWidget *widget, int duration)
{
	if (!widget) {
		return;
	}

	QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(widget);
	opacityEffect->setOpacity(1);
	widget->setGraphicsEffect(opacityEffect);
	QPropertyAnimation *opacityAnim = new QPropertyAnimation(widget);
	opacityAnim->setTargetObject(opacityEffect);
	opacityAnim->setPropertyName("opacity");
	opacityAnim->setDuration(duration);
	opacityAnim->setStartValue(opacityEffect->opacity());
	opacityAnim->setEndValue(0);
	opacityAnim->setEasingCurve(QEasingCurve::OutQuad);
	opacityAnim->start(QAbstractAnimation::DeleteWhenStopped);
}
