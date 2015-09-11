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

#pragma once

#include <QtCore/QString>

namespace testCoverage {

enum class Bgcolors;

struct DirNode {
	DirNode *parentNode;
	DirNode *rightNode;
	DirNode *childNode;
	QString name;
	int localTesting;
	int localDocumented;
	int totalTesting;
	int totalDocumented;
	int localTests;
	int totalTests;
	bool isIgnored;
	Bgcolors color;
};

/// Special directory tree for some counting inside folders
/// @warning Should use this class carefully, without nullptr-s as parameters in some methods etc.
/// Check it before DirTree methods calling.
class DirTree
{
public:
	/// Creates tree with one node-root
	explicit DirTree(const QString &name, int localTesting
			, int localDocumented, int localTests, bool isIgnored, Bgcolors color);

	/// Deletes tree recursively
	virtual ~DirTree();

	/// Adds \a child to \a parent with relative pointers to each other
	void addChild(DirNode *child, DirNode *parent);

	/// @returns the root of the tree
	DirNode *getRoot() const;

	/// Creates the free node
	/// @returns the pointer to this node
	static DirNode *createNode(const QString &name, int localTesting
			, int localDocumented, int localTests,  bool isIgnored, Bgcolors color);

	/// Calculates the data for the tree recursively
	void calculateTotalData();

private:
	/// Calculates the data for all children of this node and this node recursively
	void calculateTotalDataForNode(DirNode *node);

	/// Deletes node and his childrens recursively
	void delNode(DirNode *node);

	// Has ownership
	DirNode *root;
};

}
