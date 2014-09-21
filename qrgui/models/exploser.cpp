#include "exploser.h"

#include "models/logicalModelAssistApi.h"
#include "models/commands/explosionCommand.h"
#include "models/commands/createElementCommand.h"
#include "models/commands/renameExplosionCommand.h"
#include "controller/commands/doNothingCommand.h"
#include "controller/commands/renameCommand.h"
#include "controller/commands/createGroupCommand.h"

using namespace qReal;
using namespace models;
using namespace commands;

Exploser::Exploser(LogicalModelAssistApi &api)
	: mApi(api)
{
}

//void Exploser::addUserPalette(gui::PaletteTreeWidget * const tree, Id const &diagram)
//{
//	mUserPalettes[diagram] = tree;
//	refreshPalette(tree, diagram);
//}

//void Exploser::refreshAllPalettes()
//{
//	if (mApi.editorManagerInterface().isInterpretationMode()) {
//		Id const interperterDiagram = mUserPalettes.keys().last();
//		refreshPalette(mUserPalettes[interperterDiagram], interperterDiagram);
//	} else {
//		foreach (Id const &diagram, mUserPalettes.keys()) {
//			refreshPalette(mUserPalettes[diagram], diagram);
//		}
//	}
//}

//void Exploser::refreshPalette(gui::PaletteTreeWidget * const tree, Id const &diagram)
//{
//	QList<QPair<QString, QList<gui::PaletteElement>>> groups;
//	QMap<QString, QString> descriptions;
//	descriptions[mUserGroupTitle] = mUserGroupDescription;

//	IdList const childTypes = mApi.editorManagerInterface().elements(diagram);

//	for (Id const &child : childTypes) {
//		QList<Explosion> const explosions = mApi.editorManagerInterface().explosions(child);

//		for (Explosion const &explosion : explosions) {
//			if (!explosion.isReusable()) {
//				continue;
//			}

//			Id const targetNodeOrGroup = explosion.target();
//			Id target;
//			if (mApi.editorManagerInterface().isNodeOrEdge(targetNodeOrGroup.editor(), targetNodeOrGroup.element())) {
//				target = targetNodeOrGroup;
//			} else {
//				Pattern const pattern = mApi.editorManagerInterface().getPatternByName(targetNodeOrGroup.element());
//				target = Id(targetNodeOrGroup.editor(), targetNodeOrGroup.diagram(), pattern.rootType());
//			}

//			IdList const allTargets = mApi.logicalRepoApi().elementsByType(target.element(), true);
//			QList<gui::PaletteElement> groupElements;
//			for (Id const &targetInstance : allTargets) {
//				if (mApi.isLogicalId(targetInstance)) {
//					groupElements << gui::PaletteElement(child
//							, mApi.logicalRepoApi().name(targetInstance)
//							, QString(), mApi.editorManagerInterface().icon(child)
//							, mApi.editorManagerInterface().iconSize(child)
//							, targetInstance);
//				}
//			}

//			if (!groupElements.isEmpty()) {
//				groups << qMakePair(mUserGroupTitle, groupElements);
//			}
//		}
//	}

//	tree->addGroups(groups, descriptions, true, mApi.editorManagerInterface().friendlyName(diagram), true);
//}

//void Exploser::customizeExplosionTitles(QString const &userGroupTitle, QString const &userGroupDescription)
//{
//	mUserGroupTitle = userGroupTitle;
//	mUserGroupDescription = userGroupDescription;
//}

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
		command->addPreAction(new ExplosionCommand(mApi, nullptr, logicalId, outgoing, false));
	}

	Id const targetType = logicalId.type();
	IdList const incomingExplosions = mApi.logicalRepoApi().incomingExplosions(logicalId);
	foreach (Id const &incoming, incomingExplosions) {
		QList<Explosion> const explosions = mApi.editorManagerInterface().explosions(incoming.type());
		foreach (Explosion const &explosion, explosions) {
			if (explosion.target() == targetType && !explosion.requiresImmediateLinkage()) {
				command->addPreAction(new ExplosionCommand(mApi, nullptr, incoming, logicalId, false));
			}
		}
	}
}

AbstractCommand *Exploser::createElementWithIncomingExplosionCommand(Id const &source
		, Id const &targetType, GraphicalModelAssistApi &graphicalApi)
{
	AbstractCommand *result = nullptr;
	Id newElementId;
	if (mApi.editorManagerInterface().isNodeOrEdge(targetType.editor(), targetType.element())) {
		QString const friendlyTargetName = mApi.editorManagerInterface().friendlyName(targetType);
		newElementId = Id(targetType, QUuid::createUuid().toString());
		result = new CreateElementCommand(mApi, graphicalApi, *this, Id::rootId()
				, Id::rootId(), newElementId, false, friendlyTargetName, QPointF());
	} else {
		result = new CreateGroupCommand(nullptr, mApi, graphicalApi, *this, Id::rootId()
				, Id::rootId(), targetType, false, QPointF());
		newElementId = static_cast<CreateGroupCommand *>(result)->rootId();
	}

	result->addPostAction(addExplosionCommand(source, newElementId, &graphicalApi));
	result->addPostAction(new RenameExplosionCommand(mApi, &graphicalApi, *this, newElementId));
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
		, GraphicalModelAssistApi * const graphicalApi)
{
	AbstractCommand *result = new ExplosionCommand(mApi, graphicalApi, source, target, true);
	// Do not remove Qt::QueuedConnection flag.
	// Immediate refreshing may cause segfault because of deletting drag source.
	connect(result, SIGNAL(undoComplete(bool)), this, SLOT(refreshAllPalettes()), Qt::QueuedConnection);
	connect(result, SIGNAL(redoComplete(bool)), this, SLOT(refreshAllPalettes()), Qt::QueuedConnection);
	return result;
}

AbstractCommand *Exploser::removeExplosionCommand(Id const &source, Id const &target)
{
	AbstractCommand *result = new ExplosionCommand(mApi, nullptr, source, target, false);
	// Do not remove Qt::QueuedConnection flag.
	// Immediate refreshing may cause segfault because of deletting drag source.
	connect(result, SIGNAL(undoComplete(bool)), this, SLOT(refreshAllPalettes()), Qt::QueuedConnection);
	connect(result, SIGNAL(redoComplete(bool)), this, SLOT(refreshAllPalettes()), Qt::QueuedConnection);
	return result;
}

AbstractCommand *Exploser::renameCommands(Id const &oneOfIds, QString const &newNames) const
{
	DoNothingCommand *result = new DoNothingCommand;

	IdList const idsToRename = explosionsHierarchy(oneOfIds);
	foreach (Id const &id, idsToRename) {
		result->addPostAction(new RenameCommand(mApi, id, newNames));
	}

	if (!idsToRename.isEmpty()) {
		// Do not remove Qt::QueuedConnection flag.
		// Immediate refreshing may cause segfault because of deletting drag source.
		connect(result, SIGNAL(undoComplete(bool)), this, SLOT(refreshAllPalettes()), Qt::QueuedConnection);
		connect(result, SIGNAL(redoComplete(bool)), this, SLOT(refreshAllPalettes()), Qt::QueuedConnection);
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
