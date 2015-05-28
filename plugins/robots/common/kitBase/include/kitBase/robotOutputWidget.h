#pragma once

#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QAction>

#include <qrutils/outputWidgets/outputWidget.h>

namespace kitBase {

class RobotOutputWidget : public utils::OutputWidget
{
	Q_OBJECT
public:
	explicit RobotOutputWidget(QWidget *parent = nullptr);

	QString title() const override;

	QAction *action() override;

	QString shortcutName() const override;

public slots:
	void print(const QString &text);

	void clear();

private:
	QPlainTextEdit mOutput;
	QAction mAction;
};

}