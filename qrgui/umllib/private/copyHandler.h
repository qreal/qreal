#pragma once

class NodeElement;
class qReal::models::GraphicalModelAssistApi;

class CopyHandler {
public:
	CopyHandler(NodeElement * const copyringElement, qReal::models::GraphicalModelAssistApi * const graphicalAssistApi);

	NodeElement *clone(bool toCursorPos = false, bool searchForParents = true);
private:
	void copyChildren(const NodeElement * const destination, const NodeElement * const source) const;
	void copyEdges(const NodeElement * const destination, const NodeElement * const source) const;
	void copyProperties(const NodeElement * const destination, const NodeElement * const source) const;

	NodeElement * const mNode;
	qReal::models::GraphicalModelAssistApi * const mGraphicalAssistApi;
};
