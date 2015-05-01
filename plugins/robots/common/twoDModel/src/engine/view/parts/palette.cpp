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

#include "palette.h"

#include <QtWidgets/QAction>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>

using namespace twoDModel::view;

Palette::Palette(QWidget *parent)
	: QWidget(parent)
	, mCursorAction(new QAction(QIcon(":/icons/2d_none.png"), tr("Cursor (N)"), nullptr))
	, mGroup(new QActionGroup(this))
	, mToolBar(new QToolBar(this))
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

	QVBoxLayout * const layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setMargin(0);
	layout->setSpacing(0);

	mToolBar->setOrientation(Qt::Vertical);
	layout->addWidget(mToolBar);

	mCursorAction->setShortcut(QKeySequence(Qt::Key_N));
	registerTool(mCursorAction.data());
}

Palette::~Palette()
{
}

const QAction &Palette::cursorAction() const
{
	return *mCursorAction;
}

void Palette::registerTool(QAction * const tool)
{
	tool->setCheckable(true);
	tool->setParent(this);
	mGroup->addAction(tool);
	mToolBar->addAction(tool);
}

void Palette::unselect()
{
	mCursorAction->trigger();
}
