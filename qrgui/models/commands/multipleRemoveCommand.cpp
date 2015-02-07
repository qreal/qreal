#include "multipleRemoveCommand.h"

#include "models/models.h"

#include "models/commands/removeElementCommand.h"

using namespace qReal::commands;

MultipleRemoveCommand::MultipleRemoveCommand(const models::Models &models)
	: mLogicalApi(models.logicalModelAssistApi())
	, mGraphicalApi(models.graphicalModelAssistApi())
	, mExploser(models.exploser())
{
}

void MultipleRemoveCommand::setItemsToDelete(IdList &itemsToDelete)
{
	IdList itemsToUpdate;

	addEdgesToBeDeleted(itemsToDelete);
	// QGraphicsScene::selectedItems() returns items in no particular order,
	// so we should handle parent-child relationships manually
	while (!itemsToDelete.isEmpty()) {
		const Id currentItem = itemsToDelete.at(0);
		const IdList children = mGraphicalApi.children(currentItem);
		foreach (const Id &child, children) {
			itemsToDelete.removeAll(child);
			// Child remove commands will be added in currentItem delete command
		}

		const bool isEdge = !mLogicalApi.editorManagerInterface().isGraphicalElementNode(currentItem);
		if (isEdge) {
			const Id src = mGraphicalApi.from(currentItem);
			if (src != Id() && !itemsToUpdate.contains(src)) {
				itemsToUpdate.append(src);
			}
			const Id dst = mGraphicalApi.to(currentItem);
			if (dst != Id() && !itemsToUpdate.contains(dst)) {
				itemsToUpdate.append(dst);
			}

			insertPreAction(graphicalDeleteCommand(currentItem), 0);
		} else {
			addPreAction(graphicalDeleteCommand(currentItem));
		}

		itemsToDelete.removeAll(currentItem);
	}

	removeDuplicates();
}

void MultipleRemoveCommand::addEdgesToBeDeleted(IdList &itemsToDelete)
{
	IdList elementsToDelete = itemsToDelete;
	int i = 0;
	while (i < elementsToDelete.count()) {
		const Id currentElement = elementsToDelete.at(i);
		const IdList children = mGraphicalApi.children(currentElement);
		elementsToDelete.append(children);
		i++;
	}

	for (const Id &currentElement : elementsToDelete) {
		const IdList linksOfCurrentElement = mLogicalApi.logicalRepoApi().links(currentElement);
		for (const Id &link : linksOfCurrentElement) {
			const Id otherEntityOfCurrentLink
					= mLogicalApi.logicalRepoApi().otherEntityFromLink(link, currentElement);
			if (otherEntityOfCurrentLink == Id::rootId() || elementsToDelete.contains(otherEntityOfCurrentLink)) {
				itemsToDelete.append(link);
			}
		}
	}
}

AbstractCommand *MultipleRemoveCommand::logicalDeleteCommand(const QModelIndex &index)
{
	const Id id = mLogicalApi.idByIndex(index);
	return logicalDeleteCommand(id);
}

AbstractCommand *MultipleRemoveCommand::graphicalDeleteCommand(const QModelIndex &index)
{
	const Id id = mGraphicalApi.idByIndex(index);
	return graphicalDeleteCommand(id);
}

AbstractCommand *MultipleRemoveCommand::logicalDeleteCommand(const Id &id)
{
	// Logical deletion is equal to all its graphical parts deletion
	const IdList graphicalIds = mGraphicalApi.graphicalIdsByLogicalId(id);

	if (graphicalIds.isEmpty()) {
		return new RemoveElementCommand(
				mLogicalApi
				, mGraphicalApi
				, mExploser
				, mLogicalApi.logicalRepoApi().parent(id)
				, Id()
				, id
				, true
				, mGraphicalApi.name(id)
				, mGraphicalApi.position(id)
				);
	}

	DoNothingCommand *result = new DoNothingCommand;
	for (const Id &graphicalId : graphicalIds) {
		result->addPreAction(graphicalDeleteCommand(graphicalId));
	}

	if (graphicalIds.size() != 1) { // else it was done in graphicalDeleteCommand()
		appendExplosionsCommands(result, id);
	}

	result->removeDuplicates();
	return result;
}

AbstractCommand *MultipleRemoveCommand::graphicalDeleteCommand(const Id &id)
{
	const Id logicalId = mGraphicalApi.logicalId(id);
	AbstractCommand *result = new RemoveElementCommand(
				mLogicalApi
				, mGraphicalApi
				, mExploser
				, mLogicalApi.logicalRepoApi().parent(logicalId)
				, mGraphicalApi.graphicalRepoApi().parent(id)
				, id
				, false
				, mGraphicalApi.name(id)
				, mGraphicalApi.position(id)
				);

	const IdList children = mGraphicalApi.children(id);
	for (const Id &child : children) {
		if (mLogicalApi.editorManagerInterface().isGraphicalElementNode(child)) {
			result->addPreAction(graphicalDeleteCommand(child));
		} else {
			// Edges are deletted first
			result->insertPreAction(graphicalDeleteCommand(child), 0);
		}
	}

	if (mGraphicalApi.graphicalIdsByLogicalId(logicalId).size() == 1) {
		appendExplosionsCommands(result, logicalId);
	}

	return result;
}

void MultipleRemoveCommand::appendExplosionsCommands(AbstractCommand *parentCommand, const Id &logicalId)
{
	const IdList toDelete = mExploser.elementsWithHardDependencyFrom(logicalId);
	for (const Id &logicalChild : toDelete) {
		parentCommand->addPreAction(logicalDeleteCommand(logicalChild));
	}

	mExploser.handleRemoveCommand(logicalId, parentCommand);
}
