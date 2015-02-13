#pragma once

#include <QtWidgets/QPushButton>
#include <QtWidgets/QBoxLayout>

namespace qReal {

/// Represents styled push button on the start tab. Contains label and an icon in the circle.
/// Style can be customized from corresponding brand manager stylesheet
class StyledButton : public QPushButton
{
	Q_OBJECT

public:
	StyledButton(const QString &text, const QString &icon = QString()
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
