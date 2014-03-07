#include "textElider.h"

using namespace utils;

TextElider::TextElider(QWidget *widget, Qt::TextElideMode mode)
	: QObject(widget)
	, mWidget(widget)
	, mMode(mode)
{
	connect(this, SIGNAL(textChanged(QString)), mWidget, SLOT(setText(QString)));
}

void TextElider::setText(QString const &text)
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

		QString const &elidedText = mWidget->fontMetrics().elidedText(text, mMode, width);
		emit textChanged(elidedText);
	}
}
