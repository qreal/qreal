#include "exploser.h"
#include "../../mainwindow/mainWindow.h"
#include "../../umllib/element.h"

using namespace qReal;

Exploser::Exploser(MainWindow * const mainWindow
		, LogicalModelAssistInterface * const logicalApi
		, QObject *parent)
	: QObject(parent), mMainWindow(mainWindow), mLogicalApi(logicalApi)
{
}

void Exploser::createAddExplosionMenu(Element const * const element
		, QMenu &contextMenu, QList<Explosion> const &explosions
		, Id const &alreadyConnectedElement, char const *slot) const
{
	bool hasAnyActions = false;
	QString const menuName = alreadyConnectedElement == Id()
			? tr("Add connection") : tr("Connect to other");
	QMenu *addExplosionMenu = new QMenu(menuName);

	foreach (Explosion const &explosion, explosions) {
		foreach (Id const &elementId, mLogicalApi->logicalRepoApi().logicalElements(explosion.target())) {
			if (alreadyConnectedElement == elementId) {
				continue;
			}
			QAction *action = addExplosionMenu->addAction(mLogicalApi->logicalRepoApi().name(elementId));
			hasAnyActions = true;
			connect(action, SIGNAL(triggered()), slot);
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
		QAction *action = addExplosionMenu->addAction("New " + editorName + "/" + name);
		hasAnyActions = true;
		connect(action, SIGNAL(triggered()), slot);
		QList<QVariant> tag;
		tag << element->logicalId().toVariant() << diagramType.toVariant();
		action->setData(tag);
	}
	contextMenu.addMenu(addExplosionMenu);
}

void Exploser::createRemoveExplosionMenu(Element const * const element, QMenu &contextMenu, QString const &menuName
		, Id const &outgoingConnection, IdList const &incomingConnections, const char *slot) const
{
	QMenu *disconnectMenu = new QMenu(menuName);
	IdList list = incomingConnections;
	if (outgoingConnection != Id()) {
		list.append(outgoingConnection);
	}

	foreach (Id const &elementId, list) {
		QAction *action = disconnectMenu->addAction(mLogicalApi->logicalRepoApi().name(elementId));
		connect(action, SIGNAL(triggered()), slot);
		QList<QVariant> tag;
		tag << element->logicalId().toVariant() << elementId.toVariant();
		action->setData(tag);
	}
	if (!list.empty()) {
		contextMenu.addMenu(disconnectMenu);
	}
}

void Exploser::createConnectionSubmenus(QMenu &contextMenu, Element const * const element) const
{
	QList<Explosion> const explosions = mMainWindow->editorManager().explosions(element->id().type());
	if (explosions.isEmpty() || (explosions.count() == 1 && explosions[0].requiresImmediateLinkage())) {
		return;
	}

	// menu items "connect to"
	// TODO: move to elements, they can call the model and API themselves
	createAddExplosionMenu(element, contextMenu, explosions
			, mLogicalApi->logicalRepoApi().outgoingExplosion(element->logicalId())
			, SLOT(addExplosionActionTriggered()));

	createRemoveExplosionMenu(element, contextMenu, tr("Disconnect")
			, mLogicalApi->logicalRepoApi().outgoingExplosion(element->logicalId())
			, mLogicalApi->logicalRepoApi().incomingExplosions(element->logicalId())
			, SLOT(removeExplosionActionTriggered()));

//	QMenu * const goToMenu = contextMenu.addMenu(tr("Go to"));
//	createGoToSubmenu(goToMenu, tr("Forward connection"), mLogicalApi->logicalRepoApi().outgoingExplosions(element->logicalId()));
//	createGoToSubmenu(goToMenu, tr("Backward connection"), mLogicalApi->logicalRepoApi().incomingExplosions(element->logicalId()));

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

void Exploser::addExplosionActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> const connection = action->data().toList();
	Id const source = connection[0].value<Id>();
	Id const destination = connection[1].value<Id>();
	if (!action->text().startsWith("New ")) {
		mLogicalApi->addExplosion(source, destination);
	} else {
		mLogicalApi->createWithExplosion(source, destination);
	}
}

void Exploser::goToActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	Id const target = action->data().value<Id>();
	mMainWindow->activateItemOrDiagram(target);
	return;
}

void Exploser::removeExplosionActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> const connection = action->data().toList();
	Id const source = connection[0].value<Id>();
	Id const destination = connection[1].value<Id>();
	mLogicalApi->removeExplosion(source, destination);
}
