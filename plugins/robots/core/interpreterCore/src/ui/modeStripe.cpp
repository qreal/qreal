/* Copyright 2015 QReal Research Group, Dmitry Mordvinov
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

#include "modeStripe.h"

#include <QtWidgets/QAction>

using namespace interpreterCore::ui;

ModeStripe::ModeStripe(QAction &modeAction, const QString &text, QWidget *parent)
	: QLabel(("&nbsp;&nbsp;&nbsp;<b>%1</b> - " + tr("press %2 or click here to switch to %3"))
			.arg(text, modeAction.shortcut().toString(), modeAction.property("modeName").toString()), parent)
	, mAction(modeAction)
{
	setFrameShape(QFrame::NoFrame);
	setFrameShadow(QFrame::Plain);
	setLineWidth(0);
	addAction(&modeAction);
	connect(&modeAction, &QAction::changed, this, [&modeAction, this]() {
		setVisible(modeAction.isVisible() && !modeAction.isChecked());
	});
}

void ModeStripe::mousePressEvent(QMouseEvent *event)
{
	mAction.trigger();
	QLabel::mousePressEvent(event);
}
