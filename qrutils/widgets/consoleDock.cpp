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

#include "consoleDock.h"

#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QScrollBar>

using namespace qReal::ui;

ConsoleDock::ConsoleDock(const QString &title, QWidget *parent)
	: QDockWidget(title, parent)
	, mOutput(new QPlainTextEdit(this))
{
	setWidget(mOutput);
	mOutput->setReadOnly(true);

	setObjectName("consoleDockContents");
}

bool ConsoleDock::isEmpty() const
{
	return mOutput->toPlainText().isEmpty();
}

void ConsoleDock::print(const QString &text)
{
	mOutput->appendPlainText(text);
	mOutput->verticalScrollBar()->setValue(mOutput->verticalScrollBar()->maximum());
	show();
}

void ConsoleDock::clear()
{
	mOutput->clear();
	hide();
}
