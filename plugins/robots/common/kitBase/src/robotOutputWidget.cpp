/* Copyright 2015 QReal Research Group
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

#include "kitBase/robotOutputWidget.h"

#include <QtWidgets/QScrollBar>

using namespace kitBase;

RobotOutputWidget::RobotOutputWidget(QWidget *parent)
	: utils::OutputWidget(tr("Robot output"), parent)
{
	setWidget(&mOutput);
	mOutput.setReadOnly(true);

	setObjectName("robotOutput");

	action()->setShortcut(QKeySequence(Qt::ALT + Qt::Key_2));
}

void RobotOutputWidget::print(const QString &text)
{
	mOutput.appendPlainText(text);
	mOutput.verticalScrollBar()->setValue(mOutput.verticalScrollBar()->maximum());
	emit showMe();
}

void RobotOutputWidget::clear()
{
	mOutput.clear();
	hide();
}

QString RobotOutputWidget::shortcutName() const
{
	return "Output.Robot";
}
