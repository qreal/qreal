/* Copyright 2014-2015 QReal Research Group, Dmitry Chernov, Dmitry Mordvinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "animatedEffects.h"

#include <QtCore/QVariantAnimation>
#include <QtCore/QPropertyAnimation>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsOpacityEffect>

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

	QGraphicsOpacityEffect * const opacityEffect = new QGraphicsOpacityEffect(widget);
	opacityEffect->setOpacity(1);
	widget->setGraphicsEffect(opacityEffect);
	QPropertyAnimation *opacityAnim = new QPropertyAnimation(opacityEffect, "opacity", widget);
	opacityAnim->setDuration(duration);
	opacityAnim->setStartValue(opacityEffect->opacity());
	opacityAnim->setEndValue(0);
	opacityAnim->setEasingCurve(QEasingCurve::OutQuad);
	opacityAnim->start(QAbstractAnimation::DeleteWhenStopped);
}
