/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	void copyChildren(const NodeElement &destination, const NodeElement &source) const;

	/**
	 * TODO: make realization
	 * Copies edges of source to destination using GraphicalModelAssistApi.
	 * @param destination Node to that edges will be copied from source.
	 * @param source Node that edges will be copied.
	 */
	void copyEdges(const NodeElement &destination, const NodeElement &source) const;

	/**
	 * Copies properties of source to destination using GraphicalModelAssistApi.
	 * @param destination Node to that properties will be copied from source.
	 * @param source Node that properties will be copied.
	 */
	void copyProperties(const NodeElement &destination, const NodeElement &source) const;

	/// Node that is actually dealt with.
	NodeElement &mNode;

	/// GraphicalModelAssistApi that is used for creating new NodeElement.
	qReal::models::GraphicalModelAssistApi &mGraphicalAssistApi;
};

}
