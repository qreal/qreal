/* Copyright 2007-2016 QReal Research Group
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

#include "element.h"

#include <QtGui/QKeyEvent>
#include <QtWidgets/QGraphicsColorizeEffect>

#include <qrkernel/settingsListener.h>
#include <qrgui/models/models.h>
#include <qrgui/models/commands/changePropertyCommand.h>

#include "qrgui/editor/labels/label.h"

using namespace qReal;
using namespace qReal::gui::editor;

const qreal disabledEffectStrength = 0.9;

Element::Element(ElementImpl *elementImpl, const Id &id, const models::Models &models)
	: mMoving(false)
	, mEnabled(true)
	, mId(id)
	, mElementImpl(elementImpl)
	, mModels(models)
	, mLogicalAssistApi(models.logicalModelAssistApi())
	, mGraphicalAssistApi(models.graphicalModelAssistApi())
	, mController(nullptr)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemIsFocusable | ItemClipsChildrenToShape |
			ItemClipsToShape | ItemSendsGeometryChanges);

	setAcceptDrops(true);
	setCursor(Qt::PointingHandCursor);

	updateEnabledState();
	setHideNonHardLabels(SettingsManager::value("hideNonHardLabels").toBool());
	SettingsListener::listen("hideNonHardLabels", this, &Element::setHideNonHardLabels);
}

Id Element::id() const
{
	return mId;
}

Id Element::logicalId() const
{
	return mGraphicalAssistApi.logicalId(mId);
}

QString Element::name() const
{
	return mGraphicalAssistApi.name(id());
}

void Element::updateData()
{
	setToolTip(mGraphicalAssistApi.toolTip(id()));
}

QString Element::logicalProperty(const QString &roleName) const
{
	return mLogicalAssistApi.propertyByRoleName(logicalId(), roleName).toString();
}

void Element::setLogicalProperty(const QString &roleName, const QString &value, bool withUndoRedo)
{
	commands::AbstractCommand *command = new commands::ChangePropertyCommand(&mLogicalAssistApi
			, roleName, logicalId(), value);
	if (withUndoRedo) {
		mController->execute(command);
	} else {
		command->redo();
		delete command;
	}
}

void Element::setController(Controller *controller)
{
	mController = controller;
}

Controller * Element::controller() const
{
	return mController;
}

void Element::initTitles()
{
}

ElementImpl* Element::elementImpl() const
{
	return mElementImpl;
}

bool Element::createChildrenFromMenu() const
{
	return mElementImpl->createChildrenFromMenu();
}

void Element::updateEnabledState()
{
	const bool enabled = mLogicalAssistApi.editorManagerInterface().elements(
			Id(mId.editor(), mId.diagram())).contains(mId.type());

	mEnabled = enabled;
	if (mEnabled) {
		setGraphicsEffect(nullptr);
		setOpacity(1);
	} else {
		QGraphicsColorizeEffect * const grayScale = new QGraphicsColorizeEffect(this);
		grayScale->setColor(Qt::gray);
		grayScale->setStrength(disabledEffectStrength);
		setGraphicsEffect(grayScale);
		setOpacity(disabledEffectStrength);
	}
}

void Element::setHideNonHardLabels(bool hide)
{
	for (Label * const label : mLabels) {
		label->setVisible(label->isHard() || !hide || label->isSelected());
	}
}

void Element::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_F2) {
		for (Label * const label : mLabels) {
			if (!label->isReadOnly()) {
				label->startTextInteraction();
				event->accept();
				return;
			}
		}
	}

	QGraphicsItem::keyPressEvent(event);
}
