#include "exploserView.h"

#include <QtWidgets/QApplication>

#include <qrgui/models/models.h>
#include <qrgui/models/commands/createElementCommand.h>
#include <qrgui/dialogs/metamodelingOnFly/propertiesDialog.h>

#include "editor/editorViewScene.h"
#include "editor/sceneCustomizer.h"
#include "editor/commands/expandCommand.h"

using namespace qReal;
using namespace view::details;

ExploserView::ExploserView(models::Models const &models
		, Controller &controller
		, SceneCustomizer const &customizer
		, QObject *parent)
	: QObject(parent)
	, mLogicalApi(models.logicalModelAssistApi())
	, mGraphicalApi(models.graphicalModelAssistApi())
	, mExploser(models.exploser())
	, mController(controller)
	, mCustomizer(customizer)
{
}

void ExploserView::createAddExplosionMenu(Element const * const element
		, QMenu &contextMenu, QList<Explosion> const &explosions
		, Id const &alreadyConnectedElement) const
{
	bool hasAnyActions = false;
	QString const menuName = alreadyConnectedElement.isNull()
			? mCustomizer.addExplosionMenuName()
			: mCustomizer.changeExplosionMenuName();
	QMenu *addExplosionMenu = new QMenu(menuName);

	for (Explosion const &explosion : explosions) {
		for (Id const &elementId : mLogicalApi.logicalRepoApi().logicalElements(explosion.target())) {
			if (alreadyConnectedElement == elementId) {
				continue;
			}
			QAction *action = addExplosionMenu->addAction(mLogicalApi.logicalRepoApi().name(elementId));
			hasAnyActions = true;
			connect(action, SIGNAL(triggered()), SLOT(addExplosionActionTriggered()));
			QList<QVariant> tag;
			tag << element->logicalId().toVariant() << elementId.toVariant();
			action->setData(tag);
		}
	}

	if (hasAnyActions) {
		addExplosionMenu->addSeparator();
	}

	for (Explosion const &explosion : explosions) {
		Id const diagramType = mLogicalApi.editorManagerInterface().findElementByType(explosion.target().element());
		QString const name = mLogicalApi.editorManagerInterface().friendlyName(diagramType);
		QString const editorName = mLogicalApi.editorManagerInterface().friendlyName(Id(diagramType.editor()));
		QAction *action = addExplosionMenu->addAction(tr("New ") + editorName + "/" + name);
		hasAnyActions = true;
		connect(action, SIGNAL(triggered()), SLOT(addExplosionActionTriggered()));
		action->setData(QVariantList() << element->logicalId().toVariant() << diagramType.toVariant());
	}
	contextMenu.addMenu(addExplosionMenu);

	if (alreadyConnectedElement != Id()) {
		QAction * const gotoAction = contextMenu.addAction(mCustomizer.goToConnectedMenuName()
				, this, SLOT(goToActionTriggered()));
		gotoAction->setData(alreadyConnectedElement.toVariant());
	}
}

void ExploserView::createRemoveExplosionMenu(Element const * const element, QMenu &contextMenu
		, Id const &outgoingConnection) const
{
	if (outgoingConnection.isNull()) {
		return;
	}

	QAction * const action = contextMenu.addAction(mCustomizer.deleteExplosionMenuName());
	connect(action, SIGNAL(triggered()), SLOT(removeExplosionActionTriggered()));
	action->setData(QVariantList() << element->logicalId().toVariant() << outgoingConnection.toVariant());
}

void ExploserView::createExpandAction(Element const * const element, QMenu &contextMenu
		, Id const &alreadyConnectedElement) const
{
	if (alreadyConnectedElement.isNull()) {
		return;
	}

	NodeElement const * const node = dynamic_cast<NodeElement const * const>(element);
	if (!node) {
		return;
	}

	QAction *expandAction = contextMenu.addAction(node->isExpanded()
			? mCustomizer.collapseExplosionActionText()
			: mCustomizer.expandExplosionActionText());
	connect(expandAction, SIGNAL(triggered()), SLOT(expandExplosionActionTriggered()));

	expandAction->setData(element->id().toVariant());
}

void ExploserView::createConnectionSubmenus(QMenu &contextMenu, Element const * const element) const
{
	if (mLogicalApi.editorManagerInterface().isInterpretationMode()) {
		contextMenu.addSeparator();
		QAction * const changePropertiesAction = contextMenu.addAction(tr("Change Properties"));
		connect(changePropertiesAction, SIGNAL(triggered()), SLOT(changePropertiesActionTriggered()));
		changePropertiesAction->setData(element->id().toVariant());
		QAction * const changeAppearancePaletteAction = contextMenu.addAction(tr("Change Appearance"));
		connect(changeAppearancePaletteAction, SIGNAL(triggered()), SLOT(changeAppearanceActionTriggered()));
		changeAppearancePaletteAction->setData(element->id().toVariant());
		if (mLogicalApi.editorManagerInterface().getIsHidden(element->id()) == "true")
		{
			QAction * const addElementToPaletteAction = contextMenu.addAction(tr("Add element to palette"));
			connect(addElementToPaletteAction, SIGNAL(triggered()), SLOT(addElementToPaletteActionTriggered()));
			addElementToPaletteAction->setData(element->id().toVariant());
		}
	}

	QList<Explosion> const explosions = mLogicalApi.editorManagerInterface().explosions(element->id().type());
	if (explosions.isEmpty() || (explosions.count() == 1 && explosions[0].requiresImmediateLinkage())) {
		return;
	}

	// menu items "connect to"
	// TODO: move to elements, they can call the model and API themselves
	createAddExplosionMenu(element, contextMenu, explosions
			, mLogicalApi.logicalRepoApi().outgoingExplosion(element->logicalId()));

	createRemoveExplosionMenu(element, contextMenu
			, mLogicalApi.logicalRepoApi().outgoingExplosion(element->logicalId()));

	createExpandAction(element, contextMenu
			, mLogicalApi.logicalRepoApi().outgoingExplosion(element->logicalId()));
}

void ExploserView::handleDoubleClick(Id const &id)
{
	Id outgoingLink = mLogicalApi.logicalRepoApi().outgoingExplosion(id);
	if (outgoingLink.isNull()) {
		QList<Explosion> const explosions = mLogicalApi.editorManagerInterface().explosions(id);
		if (!explosions.isEmpty()) {
			Id const diagramType = mLogicalApi.editorManagerInterface()
					.findElementByType(explosions[0].target().element());
			commands::AbstractCommand *createCommand =
					mExploser.createElementWithIncomingExplosionCommand(
							id, diagramType, mGraphicalApi);
			mController.executeGlobal(createCommand);
			outgoingLink = static_cast<commands::CreateElementCommand *>(createCommand)->result();
		}
	}
	goTo(outgoingLink);
}

void ExploserView::handleCreationWithExplosion(commands::AbstractCommand *createCommand
		, Id const &source, Id const &target)
{
	if (target != Id()) {
		createCommand->addPostAction(mExploser.addExplosionCommand(source, target, &mGraphicalApi));
	} else {
		QList<Explosion> const explosions = mLogicalApi.editorManagerInterface().explosions(source);
		for (Explosion const &explosion : explosions) {
			if (explosion.source().type() == source.type() && explosion.requiresImmediateLinkage()) {
				createCommand->addPostAction(mExploser.createElementWithIncomingExplosionCommand(
						source, explosion.target(), mGraphicalApi));
			}
		}
	}
}

void ExploserView::addExplosionActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> const connection = action->data().toList();
	Id const source = connection[0].value<Id>();
	Id const destination = connection[1].value<Id>();
	if (action->text().startsWith(tr("New "))) {
		mController.execute(mExploser.createElementWithIncomingExplosionCommand(
				source, destination, mGraphicalApi));
	} else {
		mController.execute(mExploser.addExplosionCommand(source, destination));
	}
}

void ExploserView::goToActionTriggered()
{
	emit goTo(static_cast<QAction *>(sender())->data().value<Id>());
}

void ExploserView::removeExplosionActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> const connection = action->data().toList();
	Id const source = connection[0].value<Id>();
	Id const destination = connection[1].value<Id>();
	mController.execute(mExploser.removeExplosionCommand(source, destination));
}

void ExploserView::expandExplosionActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	Id const elem = action->data().value<Id>();
	emit expandElement(elem);
}

void ExploserView::changePropertiesActionTriggered()
{
	QAction const * const action = static_cast<QAction const *>(sender());
	Id const id = action->data().value<Id>();
	qReal::gui::PropertiesDialog * const propertiesDialog = new qReal::gui::PropertiesDialog(
			mLogicalApi.editorManagerInterface()
			, mLogicalApi.mutableLogicalRepoApi()
			, id
			, QApplication::allWidgets().isEmpty() ? nullptr : QApplication::allWidgets()[0]);
	propertiesDialog->setModal(true);
	propertiesDialog->show();
}

void ExploserView::changeAppearanceActionTriggered()
{
	QAction const * const action = static_cast<QAction const *>(sender());
	Id const id = action->data().value<Id>();
	QString const propertyValue = mLogicalApi.editorManagerInterface().shape(id);
	emit openShapeEditor(id, propertyValue, mLogicalApi.editorManagerInterface(), false);
}

void ExploserView::addElementToPaletteActionTriggered()
{
	QAction const * const action = static_cast<QAction const *>(sender());
	Id const id = action->data().value<Id>();
	mLogicalApi.editorManagerInterface().resetIsHidden(id);
	emit refreshPalette();
}
