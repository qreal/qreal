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
