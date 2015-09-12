/* Copyright 2015 QReal Research Group
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

#include "dirtree.h"

using namespace testCoverage;

DirTree::DirTree(const QString &name, int localTesting
		, int localDocumented, int localTests, bool isIgnored, Bgcolors color)
{
	root = DirTree::createNode(name, localTesting, localDocumented, localTests, isIgnored, color);
}

void DirTree::addChild(DirNode *child, DirNode *parent)
{
	if (!parent->childNode) {
		parent->childNode = child;
	} else {
		DirNode *node = parent->childNode;
		while (node->rightNode != nullptr) {
			node = node->rightNode;
		}

		node->rightNode = child;
	}

	child->parentNode = parent;
}

DirNode * DirTree::getRoot() const
{
	return root;
}

DirTree::~DirTree()
{
	delNode(root);
}

void DirTree::delNode(DirNode *node)
{
	if (node->childNode) {
		delNode(node->childNode);
	}

	if (node->rightNode){
		delNode(node->rightNode);
	}

	delete node;
}

DirNode *DirTree::createNode(const QString &name, int localTesting
		, int localDocumented, int localTests, bool isIgnored, Bgcolors color)
{
	DirNode *node = new DirNode;
	node->name = name;
	node->localDocumented = localDocumented;
	node->localTesting = localTesting;
	node->parentNode = nullptr;
	node->rightNode = nullptr;
	node->childNode = nullptr;
	node->totalTesting = 0;
	node->totalDocumented = 0;
	node->totalTests = 0;
	node->localTests = localTests;
	node->isIgnored = isIgnored;
	node->color = color;
	return node;
}

void DirTree::calculateTotalData()
{
	calculateTotalDataForNode(root);
}

void DirTree::calculateTotalDataForNode(DirNode *node)
{
	if (node->childNode) {
		calculateTotalDataForNode(node->childNode);
	}

	if (node->rightNode) {
		calculateTotalDataForNode(node->rightNode);
	}

	node->totalDocumented += node->localDocumented;
	node->totalTesting += node->localTesting;
	node->totalTests += node->localTests;

	if (node != root) {
		node->parentNode->totalDocumented += node->totalDocumented;
		node->parentNode->totalTesting += node->totalTesting;
		node->parentNode->totalTests += node->totalTests;
	}
}
