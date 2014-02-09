#include "exploser.h"

#include "mainwindow/palette/paletteTreeWidget.h"
#include "models/logicalModelAssistApi.h"
#include "controller/commands/doNothingCommand.h"
#include "controller/commands/explosionCommand.h"
#include "controller/commands/renameCommand.h"
#include "controller/commands/createElementCommand.h"
#include "controller/commands/renameExplosionCommand.h"

using namespace qReal;
using namespace models;
using namespace commands;

Exploser::Exploser(LogicalModelAssistApi &api)
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
	if (mApi.editorManagerInterface().isInterpretationMode()) {
		Id const interperterDiagram = mUserPalettes.keys().last();
		refreshPalette(mUserPalettes[interperterDiagram], interperterDiagram);
	}
	else
	{
		foreach (Id const &diagram, mUserPalettes.keys()) {
			refreshPalette(mUserPalettes[diagram], diagram);
		}
	}
}

void Exploser::refreshPalette(gui::PaletteTreeWidget * const tree, Id const &diagram)
{
	QMap<QString, QList<gui::PaletteElement> > groups;
	QMap<QString, QString> descriptions;
	descriptions[mUserGroupTitle] = mUserGroupDescription;

	IdList const childTypes = mApi.editorManagerInterface().elements(diagram);
	foreach (Id const &child, childTypes) {
		QList<Explosion> const explosions = mApi.editorManagerInterface().explosions(child);
		foreach (Explosion const &explosion, explosions) {
			if (!explosion.isReusable()) {
				continue;
			}
			Id const target = explosion.target();
			IdList const allTargets = mApi.logicalRepoApi().elementsByType(target.element(), true);
			foreach (Id const &targetInstance, allTargets) {
				if (mApi.isLogicalId(targetInstance) &&
						!mApi.logicalRepoApi().incomingExplosions(targetInstance).isEmpty())
				{
					groups[mUserGroupTitle] << gui::PaletteElement(child
							, mApi.logicalRepoApi().name(targetInstance)
							, QString(), mApi.editorManagerInterface().icon(child)
							, mApi.editorManagerInterface().iconSize(child)
							, targetInstance);
				}
			}
		}
	}

	tree->addGroups(groups, descriptions, true, mApi.editorManagerInterface().friendlyName(diagram));
}

void Exploser::customizeExplosionTitles(QString const &userGroupTitle, QString const &userGroupDescription)
{
	mUserGroupTitle = userGroupTitle;
	mUserGroupDescription = userGroupDescription;
}

IdList Exploser::elementsWithHardDependencyFrom(Id const &id) const
{
	IdList result;
	Id const targetType = id.type();
	IdList const incomingExplosions = mApi.logicalRepoApi().incomingExplosions(id);
	foreach (Id const &incoming, incomingExplosions) {
		QList<Explosion> const explosions = mApi.editorManagerInterface().explosions(incoming.type());
		foreach (Explosion const &explosion, explosions) {
			if (explosion.target() == targetType && explosion.requiresImmediateLinkage()) {
				result << incoming;
			}
		}
	}

	return result;
}

void Exploser::handleRemoveCommand(Id const &logicalId, AbstractCommand * const command)
{
	Id const outgoing = mApi.logicalRepoApi().outgoingExplosion(logicalId);
	if (!outgoing.isNull()) {
		command->addPreAction(new ExplosionCommand(mApi, NULL, logicalId, outgoing, false));
	}

	Id const targetType = logicalId.type();
	IdList const incomingExplosions = mApi.logicalRepoApi().incomingExplosions(logicalId);
	foreach (Id const &incoming, incomingExplosions) {
		QList<Explosion> const explosions = mApi.editorManagerInterface().explosions(incoming.type());
		foreach (Explosion const &explosion, explosions) {
			if (explosion.target() == targetType && !explosion.requiresImmediateLinkage()) {
				command->addPreAction(new ExplosionCommand(mApi, NULL, incoming, logicalId, false));
			}
		}
	}
}

AbstractCommand *Exploser::createElementWithIncomingExplosionCommand(Id const &source
		, Id const &targetType, GraphicalModelAssistApi *graphicalApi)
{
	QString const friendlyTargetName = mApi.editorManagerInterface().friendlyName(targetType);
	Id const newElementId(targetType, QUuid::createUuid().toString());
	AbstractCommand *result = new CreateElementCommand(mApi, *graphicalApi, Id::rootId()
			, Id::rootId(), newElementId, false, friendlyTargetName, QPointF());

	result->addPostAction(addExplosionCommand(source, newElementId, graphicalApi));
	result->addPostAction(new RenameExplosionCommand(mApi, graphicalApi, newElementId));
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
	QList<Explosion> const explosions = mApi.editorManagerInterface().explosions(id.type());
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

	IdList const idsToRename = explosionsHierarchy(oneOfIds);
	foreach (Id const &id, idsToRename) {
		result->addPostAction(new RenameCommand(mApi, id, newNames));
	}

	if (!idsToRename.isEmpty()) {
		connect(result, SIGNAL(undoComplete(bool)), this, SLOT(refreshAllPalettes()));
		connect(result, SIGNAL(redoComplete(bool)), this, SLOT(refreshAllPalettes()));
	}
	return result;
}

void Exploser::explosionsHierarchyPrivate(Id const &currentId, IdList &targetIds) const
{
	targetIds << currentId;
	IdList const incomingExplosions = mApi.logicalRepoApi().incomingExplosions(currentId);
	foreach (Id const incoming, incomingExplosions) {
		explosionsHierarchyPrivate(incoming, targetIds);
	}
}

Id Exploser::explosionsRoot(Id const &id) const
{
	Id currentId = id, previousId;
	do {
		previousId = currentId;
		currentId = mApi.logicalRepoApi().outgoingExplosion(currentId);
	} while (currentId != Id());

	return previousId;
}
