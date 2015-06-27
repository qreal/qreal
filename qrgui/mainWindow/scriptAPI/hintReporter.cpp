#include "hintReporter.h"

#include <QtCore/QTimer>
#include <QtCore/QPropertyAnimation>
#include <QtWidgets/QGraphicsOpacityEffect>
#include <QtGui/QMouseEvent>

#include <qrutils/graphicsUtils/animatedEffects.h>

using namespace qReal;
using namespace gui;
using namespace graphicsUtils;

HintReporter::HintReporter(QWidget *parent, const QString &message, int lifeTime)
	: QLabel(parent)
	, mDuration(lifeTime / 2)  // lifeTime = showing + fading out
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setWindowFlags(Qt::WindowStaysOnTopHint);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

	setAutoFillBackground(false);
	setStyleSheet("background-color: #ffffff; border: 1px solid; border-radius: 7px; border-color: #303030;");
	setMinimumSize(300, 50);
	move(parent->width() - this->width(), 0);
	setWordWrap(true);
	show();
	raise();

	setText(message);

	QTimer::singleShot(mDuration, this, SLOT(disappear()));
	QTimer::singleShot(lifeTime, this, SLOT(deleteLater()));
}

void HintReporter::disappear()
{
	AnimatedEffects::disappear(this, mDuration);
}

void HintReporter::mouseReleaseEvent(QMouseEvent *)
{
	deleteLater();
}
