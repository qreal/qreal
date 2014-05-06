#include "setBackgroundBlock.h"

#include <QtGui/QColor>

#include <interpreterBase/robotModel/robotModelUtils.h>

#include "src/robotModel/parts/trikDisplay.h"

using namespace trikKitInterpreter::blocks::details;
using namespace interpreterBase::robotModel::robotParts;

SetBackgroundBlock::SetBackgroundBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

void SetBackgroundBlock::run()
{
	robotModel::parts::TrikDisplay * const display =
			interpreterBase::robotModel::RobotModelUtils::findDevice<robotModel::parts::TrikDisplay>(
					mRobotModel, "DisplayPort"
			);

	display->setBackground(propertyToColor(stringProperty("Color")));
	emit done(mNextBlockId);
}

QColor SetBackgroundBlock::propertyToColor(QString const &property) const
{
	if (property == QString::fromUtf8("Белый")) {
		return QColor(Qt::white);
	} else if (property == QString::fromUtf8("Чёрный")) {
		return QColor(Qt::black);
	} else if (property == QString::fromUtf8("Красный")) {
		return QColor(Qt::red);
	} else if (property == QString::fromUtf8("Тёмно-красный")) {
		return QColor(Qt::darkRed);
	} else if (property == QString::fromUtf8("Зелёный")) {
		return QColor(Qt::green);
	} else if (property == QString::fromUtf8("Тёмно-зелёный")) {
		return QColor(Qt::darkGreen);
	} else if (property == QString::fromUtf8("Синий")) {
		return QColor(Qt::blue);
	} else if (property == QString::fromUtf8("Тёмно-синий")) {
		return QColor(Qt::darkBlue);
	} else if (property == QString::fromUtf8("Сине-зелёный")) {
		return QColor(Qt::cyan);
	} else if (property == QString::fromUtf8("Тёмносине-зелёный")) {
		return QColor(Qt::darkCyan);
	} else if (property == QString::fromUtf8("Пурпурный")) {
		return QColor(Qt::magenta);
	} else if (property == QString::fromUtf8("Тёмно-пурпурный")) {
		return QColor(Qt::darkMagenta);
	} else if (property == QString::fromUtf8("Жёлтый")) {
		return QColor(Qt::yellow);
	} else if (property == QString::fromUtf8("Тёмно-жёлтый")) {
		return QColor(Qt::darkYellow);
	} else if (property == QString::fromUtf8("Серый")) {
		return QColor(Qt::gray);
	} else if (property == QString::fromUtf8("Тёмно-серый")) {
		return QColor(Qt::darkGray);
	} else if (property == QString::fromUtf8("Светло-серый")) {
		return QColor(Qt::lightGray);
	}

	return QColor(Qt::transparent);
}
