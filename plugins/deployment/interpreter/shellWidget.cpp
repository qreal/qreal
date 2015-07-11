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

#include "shellWidget.h"

#include <QtWidgets/QPushButton>

using namespace deployment;

ShellWidget::ShellWidget(QWidget *parent)
	: QTextEdit(parent)
{
	QPalette pal = palette();
	pal.setColor(QPalette::Base, Qt::black);
	setPalette(pal);
	setTextColor(Qt::white);
	setReadOnly(true);

	QPushButton * const button = new QPushButton(tr("Clear"), this);
	button->move(0, 0);
	button->setFixedHeight(15);
	connect(button, &QPushButton::clicked, this, &QTextEdit::clear);
}
