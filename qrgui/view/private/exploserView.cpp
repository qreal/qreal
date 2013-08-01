#include "exploserView.h"
#include "../../mainwindow/mainWindow.h"
#include "../../umllib/element.h"
#include "../../umllib/nodeElement.h"
#include "../../view/editorViewScene.h"
#include "../../controller/commands/createElementCommand.h"
#include "../../umllib/private/expandCommand.h"

using namespace qReal;
using namespace view::details;

ExploserView::ExploserView(MainWindow * const mainWindow
		, models::LogicalModelAssistApi * const logicalApi
		, models::GraphicalModelAssistApi * const graphicalApi
		, QObject *parent)
	: QObject(parent), mMainWindow(mainWindow)
	, mLogicalApi(logicalApi), mGraphicalApi(graphicalApi)
{
}

void ExploserView::createAddExplosionMenu(Element const * const element
		, QMenu &contextMenu, QList<Explosion> const &explosions
		, Id const &alreadyConnectedElement) const
{
	bool hasAnyActions = false;
	QString const menuName = alreadyConnectedElement == Id()
			? mMainWindow->toolManager().customizer()->addExplosionMenuName()
			: mMainWindow->toolManager().customizer()->changeExplosionMenuName();
	QMenu *addExplosionMenu = new QMenu(menuName);

	foreach (Explosion const &explosion, explosions) {
		foreach (Id const &elementId, mLogicalApi->logicalRepoApi().logicalElements(explosion.target())) {
			if (alreadyConnectedElement == elementId) {
				continue;
			}
			QAction *action = addExplosionMenu->addAction(mLogicalApi->logicalRepoApi().name(elementId));
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

	foreach (Explosion const &explosion, explosions) {
		Id const diagramType = mMainWindow->editorManager().findElementByType(explosion.target().element());
		QString const name = mMainWindow->editorManager().friendlyName(diagramType);
		QString const editorName = mMainWindow->editorManager().friendlyName(Id(diagramType.editor()));
		QAction *action = addExplosionMenu->addAction(tr("New ") + editorName + "/" + name);
		hasAnyActions = true;
		connect(action, SIGNAL(triggered()), SLOT(addExplosionActionTriggered()));
		action->setData(QVariantList() << element->logicalId().toVariant() << diagramType.toVariant());
	}
	contextMenu.addMenu(addExplosionMenu);

	if (alreadyConnectedElement != Id()) {
		QAction * const gotoAction = contextMenu.addAction(
				mMainWindow->toolManager().customizer()->goToConnectedMenuName(), this, SLOT(goToActionTriggered()));
		gotoAction->setData(alreadyConnectedElement.toVariant());
	}
}

void ExploserView::createRemoveExplosionMenu(Element const * const element, QMenu &contextMenu
		, Id const &outgoingConnection) const
{
	if (outgoingConnection == Id()) {
		return;
	}

	QAction *action = contextMenu.addAction(mMainWindow->toolManager().customizer()->deleteExplosionMenuName());
	connect(action, SIGNAL(triggered()), SLOT(removeExplosionActionTriggered()));
	action->setData(QVariantList() << element->logicalId().toVariant() << outgoingConnection.toVariant());
}

void ExploserView::createExpandAction(Element const * const element, QMenu &contextMenu
		, Id const &alreadyConnectedElement) const
{
	if (alreadyConnectedElement == Id()) {
		return;
	}

	NodeElement const * const node = dynamic_cast<NodeElement const * const>(element);
	if (!node) {
		return;
	}

	QAction *action;
	action = contextMenu.addAction(node->isExpanded()
			? mMainWindow->toolManager().customizer()->collapseExplosionActionText()
			: mMainWindow->toolManager().customizer()->expandExplosionActionText());
	connect(action, SIGNAL(triggered()), SLOT(expandExplosionActionTriggered()));

	action->setData(element->id().toVariant());
}

void ExploserView::createConnectionSubmenus(QMenu &contextMenu, Element const * const element) const
{
	QList<Explosion> const explosions = mMainWindow->editorManager().explosions(element->id().type());
	if (explosions.isEmpty() || (explosions.count() == 1 && explosions[0].requiresImmediateLinkage())) {
		return;
	}

	// menu items "connect to"
	// TODO: move to elements, they can call the model and API themselves
	createAddExplosionMenu(element, contextMenu, explosions
			, mLogicalApi->logicalRepoApi().outgoingExplosion(element->logicalId()));

	createRemoveExplosionMenu(element, contextMenu
			, mLogicalApi->logicalRepoApi().outgoingExplosion(element->logicalId()));

	createExpandAction(element, contextMenu
			, mLogicalApi->logicalRepoApi().outgoingExplosion(element->logicalId()));

	if (mMainWindow->editorManager().isInterpretationMode()) {
		contextMenu.addSeparator();
		QAction * const changePropertiesAction = contextMenu.addAction(tr("Change Properties"));
		connect(changePropertiesAction, SIGNAL(triggered()), SLOT(changePropertiesActionTriggered()));
		changePropertiesAction->setData(element->id().toVariant());
		QAction * const changeAppearancePaletteAction = contextMenu.addAction(tr("Change Appearance"));
		connect(changeAppearancePaletteAction, SIGNAL(triggered()), SLOT(changeAppearanceActionTriggered()));
		changeAppearancePaletteAction->setData(element->id().toVariant());
	}
}

void ExploserView::handleDoubleClick(Id const &id)
{
	Id outgoingLink = mLogicalApi->logicalRepoApi().outgoingExplosion(id);
	if (outgoingLink == Id()) {
		QList<Explosion> const explosions = mMainWindow->editorManager().explosions(id);
		if (!explosions.isEmpty()) {
			Id const diagramType = mMainWindow->editorManager().findElementByType(explosions[0].target().element());
			commands::AbstractCommand *createCommand =
					mLogicalApi->exploser().createElementWithIncomingExplosionCommand(
							id, diagramType, mGraphicalApi);
			mMainWindow->controller()->executeGlobal(createCommand);
			outgoingLink = static_cast<commands::CreateElementCommand *>(createCommand)->result();
		}
	}
	goTo(outgoingLink);
}

void ExploserView::handleCreationWithExplosion(commands::AbstractCommand *createCommand
		, Id const &source, Id const &target)
{
	if (target != Id()) {
		createCommand->addPostAction(mLogicalApi->exploser().addExplosionCommand(source, target, mGraphicalApi));
	} else {
		QList<Explosion> const explosions = mLogicalApi->editorManagerInterface().explosions(source);
		foreach (Explosion const &explosion, explosions) {
			if (explosion.source().type() == source.type() && explosion.requiresImmediateLinkage()) {
				createCommand->addPostAction(mLogicalApi->exploser().createElementWithIncomingExplosionCommand(
						source, explosion.target(), mGraphicalApi));
			}
		}
	}
}

void ExploserView::goTo(Id const &id)
{
	mMainWindow->activateItemOrDiagram(id);
}

void ExploserView::addExplosionActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> const connection = action->data().toList();
	Id const source = connection[0].value<Id>();
	Id const destination = connection[1].value<Id>();
	if (action->text().startsWith(tr("New "))) {
		mMainWindow->controller()->execute(mLogicalApi->exploser().createElementWithIncomingExplosionCommand(
				source, destination, mGraphicalApi));
	} else {
		mMainWindow->controller()->execute(mLogicalApi->exploser().addExplosionCommand(source, destination));
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
	mMainWindow->controller()->execute(mLogicalApi->exploser().removeExplosionCommand(source, destination));
}

void ExploserView::expandExplosionActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	Id elem = action->data().value<Id>();
	EditorViewScene *evScene = dynamic_cast<EditorViewScene *>(mMainWindow->getCurrentTab()->scene());
	NodeElement *node = evScene->getNodeById(elem);
	if (node) {
		mMainWindow->controller()->execute(new qReal::commands::ExpandCommand(node));
	}
}
