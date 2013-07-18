#include "exploser.h"
#include "../../mainwindow/palette/paletteTreeWidget.h"
#include "../logicalModelAssistApi.h"

using namespace qReal::models::details;

Exploser::Exploser(LogicalModelAssistApi * const api)
	: mApi(api)
{
}

void Exploser::addUserPalette(qReal::gui::PaletteTreeWidget * const tree, Id const &diagram)
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
			if (!allTargets.isEmpty()) {
				groups[groupName] << mApi->logicalRepoApi().name(allTargets[0]);
				allElements << allTargets[0];
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

qReal::IdList Exploser::elementsWithHardDependencyFrom(qReal::Id const &id) const
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

void Exploser::explosionsHierarchy(Id const &oneOfIds, IdList &hierarchy) const
{
	// Infinite cycle may happen here in case of cyclic explosions
	Id const root = explosionsRoot(oneOfIds);
	explosionsHierarchyPrivate(root, hierarchy);
}

qReal::Id Exploser::explosionTarget(qReal::Id const &id)
{
	QList<Explosion> const explosions = mApi->editorManagerInterface().explosions(id.type());
	if (explosions.size() == 1 && explosions[0].requiresImmediateLinkage()) {
		return explosions[0].target();
	}
	return Id();
}

void Exploser::addExplosion(qReal::Id const &source, qReal::Id const &target)
{
	QString const sourceName = mApi->property(source, "name").toString();
	mApi->setProperty(target, "name", sourceName + tr(" - inside"));
	mApi->addExplosion(source, target);
	QList<Explosion> const explosions = mApi->editorManagerInterface().explosions(source);
	foreach (Explosion const &explosion, explosions) {
		if (explosion.target() == target.type() && explosion.isReusable()) {
			break;
		}
	}
}

void Exploser::explosionsHierarchyPrivate(qReal::Id const &currentId, qReal::IdList &targetIds) const
{
	targetIds << currentId;
	IdList const incommingExplosions = mApi->logicalRepoApi().incomingExplosions(currentId);
	foreach (Id const incomming, incommingExplosions) {
		explosionsHierarchyPrivate(incomming, targetIds);
	}
}

void Exploser::reuseExplosion(qReal::Id const &source, bool invokeEditTrigger)
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

QTreeWidgetItem *Exploser::userGroup(qReal::Id const &diagram) const
{
	for (int i = 0 ; i < mUserPalettes[diagram]->topLevelItemCount(); ++i) {
		QTreeWidgetItem *item = mUserPalettes[diagram]->topLevelItem(i);
		if (item && item->text(0) == userGroupTitle()) {
			return item;
		}
	}
	return NULL;
}

qReal::Id Exploser::explosionsRoot(qReal::Id const &id) const
{
	Id currentId = id, previousId;
	do {
		previousId = currentId;
		currentId = mApi->logicalRepoApi().outgoingExplosion(currentId);
	} while (currentId != Id());
	return previousId;
}
