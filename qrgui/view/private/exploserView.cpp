#include "exploserView.h"

#include "mainwindow/mainWindow.h"
#include "umllib/element.h"
#include "umllib/nodeElement.h"
#include "view/editorViewScene.h"
#include "controller/commands/createElementCommand.h"
#include "umllib/private/expandCommand.h"
#include "dialogs/metamodelingOnFly/propertiesDialog.h"

using namespace qReal;
using namespace view::details;

ExploserView::ExploserView(MainWindow &mainWindow
		, models::LogicalModelAssistApi &logicalApi
		, models::GraphicalModelAssistApi &graphicalApi
		, Exploser &exploser
		, QObject *parent)
	: QObject(parent)
	, mMainWindow(mainWindow)
	, mLogicalApi(logicalApi)
	, mGraphicalApi(graphicalApi)
	, mExploser(exploser)
{
}

void ExploserView::createAddExplosionMenu(Element const * const element
		, QMenu &contextMenu, QList<Explosion> const &explosions
		, Id const &alreadyConnectedElement) const
{
	bool hasAnyActions = false;
	QString const menuName = alreadyConnectedElement.isNull()
			? mMainWindow.toolManager().customizer()->addExplosionMenuName()
			: mMainWindow.toolManager().customizer()->changeExplosionMenuName();
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
		Id const diagramType = mMainWindow.editorManager().findElementByType(explosion.target().element());
		QString const name = mMainWindow.editorManager().friendlyName(diagramType);
		QString const editorName = mMainWindow.editorManager().friendlyName(Id(diagramType.editor()));
		QAction *action = addExplosionMenu->addAction(tr("New ") + editorName + "/" + name);
		hasAnyActions = true;
		connect(action, SIGNAL(triggered()), SLOT(addExplosionActionTriggered()));
		action->setData(QVariantList() << element->logicalId().toVariant() << diagramType.toVariant());
	}
	contextMenu.addMenu(addExplosionMenu);

	if (alreadyConnectedElement != Id()) {
		QAction * const gotoAction = contextMenu.addAction(
				mMainWindow.toolManager().customizer()->goToConnectedMenuName(), this, SLOT(goToActionTriggered()));
		gotoAction->setData(alreadyConnectedElement.toVariant());
	}
}

void ExploserView::createRemoveExplosionMenu(Element const * const element, QMenu &contextMenu
		, Id const &outgoingConnection) const
{
	if (outgoingConnection.isNull()) {
		return;
	}

	QAction *action = contextMenu.addAction(mMainWindow.toolManager().customizer()->deleteExplosionMenuName());
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

	QAction * expandAction = contextMenu.addAction(node->isExpanded()
			? mMainWindow.toolManager().customizer()->collapseExplosionActionText()
			: mMainWindow.toolManager().customizer()->expandExplosionActionText());
	connect(expandAction, SIGNAL(triggered()), SLOT(expandExplosionActionTriggered()));

	expandAction->setData(element->id().toVariant());
}

void ExploserView::createConnectionSubmenus(QMenu &contextMenu, Element const * const element) const
{
	if (mMainWindow.editorManager().isInterpretationMode()) {
		contextMenu.addSeparator();
		QAction * const changePropertiesAction = contextMenu.addAction(tr("Change Properties"));
		connect(changePropertiesAction, SIGNAL(triggered()), SLOT(changePropertiesActionTriggered()));
		changePropertiesAction->setData(element->id().toVariant());
		QAction * const changeAppearancePaletteAction = contextMenu.addAction(tr("Change Appearance"));
		connect(changeAppearancePaletteAction, SIGNAL(triggered()), SLOT(changeAppearanceActionTriggered()));
		changeAppearancePaletteAction->setData(element->id().toVariant());
		if (mMainWindow.editorManager().getIsHidden(element->id()) == "true")
		{
			QAction * const addElementToPaletteAction = contextMenu.addAction(tr("Add element to palette"));
			connect(addElementToPaletteAction, SIGNAL(triggered()), SLOT(addElementToPaletteActionTriggered()));
			addElementToPaletteAction->setData(element->id().toVariant());
		}
	}

	QList<Explosion> const explosions = mMainWindow.editorManager().explosions(element->id().type());
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
		QList<Explosion> const explosions = mMainWindow.editorManager().explosions(id);
		if (!explosions.isEmpty()) {
			Id const diagramType = mMainWindow.editorManager().findElementByType(explosions[0].target().element());
			commands::AbstractCommand *createCommand =
					mExploser.createElementWithIncomingExplosionCommand(
							id, diagramType, mGraphicalApi);
			mMainWindow.controller()->executeGlobal(createCommand);
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

void ExploserView::goTo(Id const &id)
{
	if (!id.isNull()) {
		mMainWindow.activateItemOrDiagram(id);
	}
}

void ExploserView::addExplosionActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> const connection = action->data().toList();
	Id const source = connection[0].value<Id>();
	Id const destination = connection[1].value<Id>();
	if (action->text().startsWith(tr("New "))) {
		mMainWindow.controller()->execute(mExploser.createElementWithIncomingExplosionCommand(
				source, destination, mGraphicalApi));
	} else {
		mMainWindow.controller()->execute(mExploser.addExplosionCommand(source, destination));
	}
}

void ExploserView::goToActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	Id const target = action->data().value<Id>();
	goTo(target);
}

void ExploserView::removeExplosionActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> const connection = action->data().toList();
	Id const source = connection[0].value<Id>();
	Id const destination = connection[1].value<Id>();
	mMainWindow.controller()->execute(mExploser.removeExplosionCommand(source, destination));
}

void ExploserView::expandExplosionActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	Id elem = action->data().value<Id>();
	EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(mMainWindow.getCurrentTab()->scene());
	if (!evScene) {
		return;
	}

	NodeElement *node = evScene->getNodeById(elem);
	if (node) {
		mMainWindow.controller()->execute(new qReal::commands::ExpandCommand(node));
	}
}

void ExploserView::changePropertiesActionTriggered()
{
	QAction const * const action = static_cast<QAction const *>(sender());
	Id const id = action->data().value<Id>();
	qReal::gui::PropertiesDialog * const propertiesDialog
			= new qReal::gui::PropertiesDialog(mMainWindow, mMainWindow.editorManager(), id);
	propertiesDialog->setModal(true);
	propertiesDialog->show();
}

void ExploserView::changeAppearanceActionTriggered()
{
	QAction const * const action = static_cast<QAction const *>(sender());
	Id const id = action->data().value<Id>();
	QString const propertyValue = mMainWindow.editorManager().shape(id);
	mMainWindow.openShapeEditor(id, propertyValue, &(mMainWindow.editorManager()), false);
}

void ExploserView::addElementToPaletteActionTriggered()
{
	QAction const * const action = static_cast<QAction const *>(sender());
	Id const id = action->data().value<Id>();
	mMainWindow.editorManager().resetIsHidden(id);
	mMainWindow.loadPlugins();
}
