#include "multipleRemoveCommand.h"

#include "models/models.h"

#include "models/commands/removeElementCommand.h"

using namespace qReal::commands;

MultipleRemoveCommand::MultipleRemoveCommand(models::Models const &models, IdList &itemsToDelete)
	: mLogicalApi(models.logicalModelAssistApi())
	, mGraphicalApi(models.graphicalModelAssistApi())
	, mExploser(models.exploser())
{
	IdList itemsToUpdate;

	addEdgesToBeDeleted(itemsToDelete);
	// QGraphicsScene::selectedItems() returns items in no particular order,
	// so we should handle parent-child relationships manually
	while (!itemsToDelete.isEmpty()) {
		Id const currentItem = itemsToDelete.at(0);
		IdList const children = mGraphicalApi.children(currentItem);
		foreach (Id const &child, children) {
			itemsToDelete.removeAll(child);
			// Child remove commands will be added in currentItem delete command
		}

		bool const isEdge = !mLogicalApi.editorManagerInterface().isGraphicalElementNode(currentItem);
		if (isEdge) {
			Id const src = mGraphicalApi.from(currentItem);
			if (src != Id() && !itemsToUpdate.contains(src)) {
				itemsToUpdate.append(src);
			}
			Id const dst = mGraphicalApi.to(currentItem);
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
		Id const currentElement = elementsToDelete.at(i);
		IdList const children = mGraphicalApi.children(currentElement);
		elementsToDelete.append(children);
		i++;
	}

	for (Id const &currentElement : elementsToDelete) {
		IdList const linksOfCurrentElement = mLogicalApi.logicalRepoApi().links(currentElement);
		for (Id const &link : linksOfCurrentElement) {
			Id const otherEntityOfCurrentLink
					= mLogicalApi.logicalRepoApi().otherEntityFromLink(link, currentElement);
			if (otherEntityOfCurrentLink == Id::rootId() || elementsToDelete.contains(otherEntityOfCurrentLink)) {
				itemsToDelete.append(link);
			}
		}
	}
}

AbstractCommand *MultipleRemoveCommand::logicalDeleteCommand(QModelIndex const &index)
{
	Id const id = mLogicalApi.idByIndex(index);
	return logicalDeleteCommand(id);
}

AbstractCommand *MultipleRemoveCommand::graphicalDeleteCommand(QModelIndex const &index)
{
	Id const id = mGraphicalApi.idByIndex(index);
	return graphicalDeleteCommand(id);
}

AbstractCommand *MultipleRemoveCommand::logicalDeleteCommand(Id const &id)
{
	// Logical deletion is equal to all its graphical parts deletion
	IdList const graphicalIds = mGraphicalApi.graphicalIdsByLogicalId(id);

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
	for (Id const &graphicalId : graphicalIds) {
		result->addPreAction(graphicalDeleteCommand(graphicalId));
	}

	if (graphicalIds.size() != 1) { // else it was done in graphicalDeleteCommand()
		appendExplosionsCommands(result, id);
	}

	result->removeDuplicates();
	return result;
}

AbstractCommand *MultipleRemoveCommand::graphicalDeleteCommand(Id const &id)
{
	Id const logicalId = mGraphicalApi.logicalId(id);
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

	connect(result, SIGNAL(redoComplete(bool)), this, SLOT(closeTabsWithRemovedRootElements()));
	IdList const children = mGraphicalApi.children(id);
	for (Id const &child : children) {
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

void MultipleRemoveCommand::appendExplosionsCommands(AbstractCommand *parentCommand, Id const &logicalId)
{
	IdList const toDelete = mExploser.elementsWithHardDependencyFrom(logicalId);
	for (Id const &logicalChild : toDelete) {
		parentCommand->addPreAction(logicalDeleteCommand(logicalChild));
	}

	mExploser.handleRemoveCommand(logicalId, parentCommand);
}
