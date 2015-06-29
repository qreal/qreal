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

#include "widgetFinder.h"

using namespace utils;

QWidget *WidgetFinder::widget(QWidget *parent, const QString &type, const QString &name)
{
	const QList<QWidget *> widgetList = parent->findChildren<QWidget *>(name);

	for (QWidget * const widget : widgetList) {
		const char *typeName = type.toLocal8Bit().data();
		if (widget->inherits(typeName) && widget->isVisible()) {
			return widget;
		}
	}

	return nullptr;
}
