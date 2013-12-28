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

	/// Widget passed to this method will be highlighted on mouse hover over this button
	void bindHighlightedOnHover(QWidget * const widget);

protected:
	virtual void enterEvent(QEvent *event) override;
	virtual void leaveEvent(QEvent *event) override;

private:
	void highlight(bool on);

	QWidgetList mChildren;
};

}
