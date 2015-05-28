#pragma once

#include <QtWidgets/QPlainTextEdit>

#include <qrutils/outputWidgets/outputWidget.h>

namespace kitBase {

class RobotOutputWidget : public utils::OutputWidget
{
	Q_OBJECT
public:
	explicit RobotOutputWidget(QWidget *parent = nullptr);

public slots:
	void print(const QString &text);

	void clear();

private:
	QPlainTextEdit mOutput;
};

}