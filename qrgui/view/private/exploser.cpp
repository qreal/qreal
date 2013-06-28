#include "exploser.h"
#include "../../mainwindow/mainWindow.h"
#include "../../umllib/element.h"

using namespace qReal;

Exploser::Exploser(MainWindow * const mainWindow
		, LogicalModelAssistInterface * const logicalApi
		, GraphicalModelAssistInterface * const graphicalApi
		, QObject *parent)
	: QObject(parent), mMainWindow(mainWindow)
	, mLogicalApi(logicalApi), mGraphicalApi(graphicalApi)
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
		action->setData(QVariantList() << element->logicalId().toVariant() << diagramType.toVariant());
	}
	contextMenu.addMenu(addExplosionMenu);

	if (alreadyConnectedElement != Id()) {
		QAction * const gotoAction = contextMenu.addAction(tr("Go to connected element")
				, this, SLOT(goToActionTriggered()));
		gotoAction->setData(alreadyConnectedElement.toVariant());
	}
}

void Exploser::createRemoveExplosionMenu(Element const * const element, QMenu &contextMenu
		, Id const &outgoingConnection, const char *slot) const
{
	if (outgoingConnection == Id()) {
		return;
	}

	// TODO: customize it from plugins
	QAction *action = contextMenu.addAction(tr("Disconnect"));
	connect(action, SIGNAL(triggered()), slot);
	action->setData(QVariantList() << element->logicalId().toVariant() << outgoingConnection.toVariant());
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

	createRemoveExplosionMenu(element, contextMenu
			, mLogicalApi->logicalRepoApi().outgoingExplosion(element->logicalId())
			, SLOT(removeExplosionActionTriggered()));

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

void Exploser::handleDoubleClick(Id const &id)
{
	Id outgoingLink = mLogicalApi->logicalRepoApi().outgoingExplosion(id);
	if (outgoingLink == Id()) {
		QList<Explosion> const explosions = mMainWindow->editorManager().explosions(id);
		if (!explosions.isEmpty()) {
			Id const diagramType = mMainWindow->editorManager().findElementByType(explosions[0].target().element());
			outgoingLink = createElementWithIncommingExplosion(id, diagramType);
		}
	}
	goTo(outgoingLink);
}

void Exploser::goTo(Id const &id)
{
	mMainWindow->activateItemOrDiagram(id);
}

Id Exploser::createElementWithIncommingExplosion(Id const &source, Id const &targetType)
{
	Id const targetElement = mGraphicalApi->createElement(Id::rootId(), targetType);
	Id const logicalId = mGraphicalApi->logicalId(targetElement);
	QString const sourceName = mLogicalApi->property(source, "name").toString();
	// TODO: customize suffix from plugins
	mLogicalApi->setProperty(logicalId, "name", sourceName + tr(" - inside"));
	mLogicalApi->setProperty(targetElement, "name", sourceName + tr(" - inside"));
	mLogicalApi->addExplosion(source, logicalId);
	return logicalId;
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
		createElementWithIncommingExplosion(source, destination);
	}
}

void Exploser::goToActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	Id const target = action->data().value<Id>();
	goTo(target);
}

void Exploser::removeExplosionActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> const connection = action->data().toList();
	Id const source = connection[0].value<Id>();
	Id const destination = connection[1].value<Id>();
	mLogicalApi->removeExplosion(source, destination);
}
