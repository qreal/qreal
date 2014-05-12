#include "hintReporter.h"

using namespace qReal;
using namespace gui;

HintReporter::HintReporter(MainWindow *mainWindow, QString const &message, int const lifeTime)
	: QLabel(mainWindow)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setWindowFlags(Qt::WindowStaysOnTopHint);

	setAutoFillBackground(false);
	setStyleSheet("background-color: #ffffff; border: 1px solid; border-radius: 7px; border-color: #303030;"); // по вкусу
	setFixedSize(300, 50);
	move(mainWindow->width() - this->width(), 0);
	show();
	raise();

	addHint(message);
	mDuration = lifeTime/2;
	QTimer::singleShot(mDuration, this, SLOT(disappear()));
}

void HintReporter::addHint(QString const &hint)
{
	QString const message= "<b>Подсказка :\n</b>"+hint;
	setText(message);

}

void HintReporter::disappear()
{
	QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(this);
	opacityEffect->setOpacity(1);
	setGraphicsEffect(opacityEffect);
	QPropertyAnimation* opacityAnim = new QPropertyAnimation(this);
	opacityAnim->setTargetObject(opacityEffect);
	opacityAnim->setPropertyName("opacity");
	opacityAnim->setDuration(mDuration);
	opacityAnim->setStartValue(opacityEffect->opacity());
	opacityAnim->setEndValue(0);
	opacityAnim->setEasingCurve(QEasingCurve::OutQuad);
	opacityAnim->start(QAbstractAnimation::DeleteWhenStopped);
}

void HintReporter::mousePressEvent(QMouseEvent * event)
{
	emit mousePressEvent();
}
