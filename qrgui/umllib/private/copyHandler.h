/** @file copyHandler.h
 * 	@brief Class for handling copyring of NodeElement.
 * */

#pragma once

class NodeElement;
// class qReal::models::GraphicalModelAssistApi; because warning

class CopyHandler {
public:
	/**
	 * Constructs a CopyHandler.
	 * @param copyringNode Node that is actually dealt with.
	 * @param graphicalAssistApi GraphicalModelAssistApi that used by node.
	 */
	CopyHandler(NodeElement * const copyringElement, qReal::models::GraphicalModelAssistApi * const graphicalAssistApi);

	/**
	 * Makes copy of NodeElement that was used in constructor of CopyHandler.
	 * @param toCursorPos Indicates if need to place new element at cursor position.
	 * @param searchForParents Parameter of createElement method in EditorViewScene.
	 * @return Copy of NodeElement that was used in constructor of CopyHandler.
	 */
	NodeElement *clone(bool toCursorPos = false, bool searchForParents = true);

private:
	/**
	 * Copies children of source to destination using GraphicalModelAssistApi.
	 * @param destination Node to that childrens of source will be copied.
	 * @param source Node that children will be copied.
	 */
	void copyChildren(const NodeElement * const destination, const NodeElement * const source) const;

	/**
	 * TODO: make realization
	 * Copies edges of source to destination using GraphicalModelAssistApi.
	 * @param destination Node to that edges will be copied from source.
	 * @param source Node that edges will be copied.
	 */
	void copyEdges(const NodeElement * const destination, const NodeElement * const source) const;

	/**
	 * Copies properties of source to destination using GraphicalModelAssistApi.
	 * @param destination Node to that properties will be copied from source.
	 * @param source Node that properties will be copied.
	 */
	void copyProperties(const NodeElement * const destination, const NodeElement * const source) const;

	/// Node that is actually dealt with.
	NodeElement * const mNode;
	
	/// GraphicalModelAssistApi that used to creating new NodeElement.
	qReal::models::GraphicalModelAssistApi * const mGraphicalAssistApi;
};
