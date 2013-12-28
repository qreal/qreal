#pragma once

#include <QtWidgets/QPushButton>
#include <QtWidgets/QBoxLayout>

namespace qReal {

class StyledButton : public QPushButton
{
	Q_OBJECT

public:
	StyledButton(QString const &text, QString const &icon = QString()
			, QBoxLayout::Direction direction = QBoxLayout::LeftToRight, QWidget *parent = 0);

protected:
	virtual void enterEvent(QEvent *event) override;
	virtual void leaveEvent(QEvent *event) override;

private:
	void highlight(bool on);

	QWidgetList mChildren;
};

}
