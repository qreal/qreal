#include "exploser.h"
#include "../../mainwindow/palette/paletteTreeWidget.h"
#include "../logicalModelAssistApi.h"
#include "../controller/commands/doNothingCommand.h"
#include "../controller/commands/explosionCommand.h"
#include "../controller/commands/renameCommand.h"
#include "../controller/commands/createElementCommand.h"

using namespace qReal;
using namespace models::details;
using namespace commands;

Exploser::Exploser(LogicalModelAssistApi * const api)
	: mApi(api)
{
}

void Exploser::addUserPalette(gui::PaletteTreeWidget * const tree, Id const &diagram)
{
	mUserPalettes[diagram] = tree;
	refreshPalette(tree, diagram);
}

void Exploser::refreshAllPalettes()
{
	foreach (Id const &diagram, mUserPalettes.keys()) {
		refreshPalette(mUserPalettes[diagram], diagram);
	}
}

void Exploser::refreshPalette(gui::PaletteTreeWidget * const tree, Id const &diagram)
{
	QString const groupName = userGroupTitle();
	QString const groupDescription = userGroupDescription();
	QMap<QString, QStringList> groups;
	QMap<QString, QString> descriptions;
	descriptions[groupName] = groupDescription;
	IdList allElements;

	IdList const childTypes = mApi->editorManagerInterface().elements(diagram);
	foreach (Id const &child, childTypes) {
		QList<Explosion> const explosions = mApi->editorManagerInterface().explosions(child);
		foreach (Explosion const &explosion, explosions) {
			if (!explosion.isReusable()) {
				continue;
			}
			Id const target = explosion.target();
			IdList const allTargets = mApi->logicalRepoApi().elementsByType(target.element(), true);
			foreach (Id const &targetInstance, allTargets) {
				if (mApi->isLogicalId(targetInstance) &&
						!mApi->logicalRepoApi().incomingExplosions(targetInstance).isEmpty())
				{
					groups[groupName] << mApi->logicalRepoApi().name(targetInstance);
					allElements << targetInstance;
				}
			}
		}
	}

	tree->addGroups(groups, descriptions, allElements, true, mApi->editorManagerInterface().friendlyName(diagram));
}

QString Exploser::insideSuffix()
{
	// TODO: pluginize it
	return tr(" - inside");
}

QString Exploser::userGroupTitle()
{
	// TODO: pluginize it
	return tr("Existing connections");
}

QString Exploser::userGroupDescription()
{
	// TODO: pluginize it
	return tr("Elements from this group exist for reusing all created connections");
}

IdList Exploser::elementsWithHardDependencyFrom(Id const &id) const
{
	IdList result;
	Id const targetType = id.type();
	IdList const incommingExplosions = mApi->logicalRepoApi().incomingExplosions(id);
	foreach (Id const &incomming, incommingExplosions) {
		QList<Explosion> const explosions = mApi->editorManagerInterface().explosions(incomming.type());
		foreach (Explosion const &explosion, explosions) {
			if (explosion.target() == targetType && explosion.requiresImmediateLinkage()) {
				result << incomming;
			}
		}
	}
	return result;
}

AbstractCommand *Exploser::createElementWithIncommingExplosionCommand(Id const &source
		, Id const &targetType, GraphicalModelAssistApi *graphicalApi)
{
	QString const friendlyTargetName = mApi->editorManagerInterface().friendlyName(targetType);
	Id const newElementId(targetType, QUuid::createUuid().toString());
	AbstractCommand *result = new CreateElementCommand(mApi, graphicalApi, Id::rootId()
			, Id::rootId(), newElementId, false, friendlyTargetName, QPointF());

	result->addPostAction(addExplosionCommand(source, newElementId, graphicalApi));
	connect(result, SIGNAL(undoComplete(bool)), this, SIGNAL(explosionTargetRemoved()));
	return result;
}

IdList Exploser::explosionsHierarchy(Id const &oneOfIds) const
{
	// Infinite cycle may happen here in case of cyclic explosions
	Id const root = explosionsRoot(oneOfIds);
	IdList hierarchy;
	explosionsHierarchyPrivate(root, hierarchy);
	return hierarchy;
}

Id Exploser::immediateExplosionTarget(Id const &id)
{
	QList<Explosion> const explosions = mApi->editorManagerInterface().explosions(id.type());
	if (explosions.size() == 1 && explosions[0].requiresImmediateLinkage()) {
		return explosions[0].target();
	}
	return Id();
}

AbstractCommand *Exploser::addExplosionCommand(Id const &source, Id const &target
		, GraphicalModelAssistApi *graphicalApi)
{
	AbstractCommand *result = new ExplosionCommand(mApi, graphicalApi, source, target, true);
	connect(result, SIGNAL(undoComplete(bool)), this, SLOT(refreshAllPalettes()));
	connect(result, SIGNAL(redoComplete(bool)), this, SLOT(refreshAllPalettes()));
	return result;
}

AbstractCommand *Exploser::removeExplosionCommand(Id const &source, Id const &target)
{
	AbstractCommand *result = new ExplosionCommand(mApi, NULL, source, target, false);
	connect(result, SIGNAL(undoComplete(bool)), this, SLOT(refreshAllPalettes()));
	connect(result, SIGNAL(redoComplete(bool)), this, SLOT(refreshAllPalettes()));
	return result;
}

AbstractCommand *Exploser::renameCommands(Id const &oneOfIds, QString const &newNames)
{
	DoNothingCommand *result = new DoNothingCommand;
	connect(result, SIGNAL(undoComplete(bool)), this, SLOT(refreshAllPalettes()));
	connect(result, SIGNAL(redoComplete(bool)), this, SLOT(refreshAllPalettes()));

	IdList const idsToRename = explosionsHierarchy(oneOfIds);
	foreach (Id const &id, idsToRename) {
		result->addPostAction(new RenameCommand(mApi, id, newNames));
	}
	return result;
}

void Exploser::explosionsHierarchyPrivate(Id const &currentId, IdList &targetIds) const
{
	targetIds << currentId;
	IdList const incommingExplosions = mApi->logicalRepoApi().incomingExplosions(currentId);
	foreach (Id const incomming, incommingExplosions) {
		explosionsHierarchyPrivate(incomming, targetIds);
	}
}

void Exploser::reuseExplosion(Id const &source, bool invokeEditTrigger)
{
	QTreeWidgetItem *group = userGroup(Id(source.editor(), source.diagram()));
	QTreeWidgetItem *item = new QTreeWidgetItem;
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	item->setText(0, mApi->logicalRepoApi().name(source));
	group->addChild(item);
	if (invokeEditTrigger) {
		mUserPalettes[Id(source.editor(), source.diagram())]->editItem(item);
	}
}

QTreeWidgetItem *Exploser::userGroup(Id const &diagram) const
{
	for (int i = 0 ; i < mUserPalettes[diagram]->topLevelItemCount(); ++i) {
		QTreeWidgetItem *item = mUserPalettes[diagram]->topLevelItem(i);
		if (item && item->text(0) == userGroupTitle()) {
			return item;
		}
	}
	return NULL;
}

Id Exploser::explosionsRoot(Id const &id) const
{
	Id currentId = id, previousId;
	do {
		previousId = currentId;
		currentId = mApi->logicalRepoApi().outgoingExplosion(currentId);
	} while (currentId != Id());
	return previousId;
}
