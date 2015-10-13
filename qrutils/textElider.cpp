/* Copyright 2007-2015 QReal Research Group
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

#include "textElider.h"

using namespace utils;

TextElider::TextElider(QWidget *widget, Qt::TextElideMode mode)
	: QObject(widget)
	, mWidget(widget)
	, mMode(mode)
{
	connect(this, SIGNAL(textChanged(QString)), mWidget, SLOT(setText(QString)));
}

void TextElider::setText(const QString &text)
{
	if (mWidget) {
		int width = 0;
		if (mWidget->isVisible()) {
			width = mWidget->contentsRect().width();
		} else {
			int left, top, right, bottom;
			mWidget->getContentsMargins(&left, &top, &right, &bottom);
			width = mWidget->sizeHint().width() - left - right;
		}

		const QString &elidedText = mWidget->fontMetrics().elidedText(text, mMode, width);
		emit textChanged(elidedText);
	}
}
