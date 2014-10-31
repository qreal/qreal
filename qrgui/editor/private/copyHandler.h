/** @file copyHandler.h
* 	@brief Class for handling copyring of NodeElement.
**/

#pragma once

namespace qReal {

namespace models {
class GraphicalModelAssistApi;
}

class NodeElement;
// class qReal::models::GraphicalModelAssistApi; because warning

class CopyHandler
{
public:
	/**
	 * Constructs a CopyHandler.
	 * @param copyringNode Node that is actually dealt with.
	 * @param graphicalAssistApi GraphicalModelAssistApi that used by node.
	 */
	CopyHandler(NodeElement &copyingElement, qReal::models::GraphicalModelAssistApi &graphicalAssistApi);

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
	void copyChildren(NodeElement const &destination, NodeElement const &source) const;

	/**
	 * TODO: make realization
	 * Copies edges of source to destination using GraphicalModelAssistApi.
	 * @param destination Node to that edges will be copied from source.
	 * @param source Node that edges will be copied.
	 */
	void copyEdges(NodeElement const &destination, NodeElement const &source) const;

	/**
	 * Copies properties of source to destination using GraphicalModelAssistApi.
	 * @param destination Node to that properties will be copied from source.
	 * @param source Node that properties will be copied.
	 */
	void copyProperties(NodeElement const &destination, NodeElement const &source) const;

	/// Node that is actually dealt with.
	NodeElement &mNode;

	/// GraphicalModelAssistApi that is used for creating new NodeElement.
	qReal::models::GraphicalModelAssistApi &mGraphicalAssistApi;
};

}
