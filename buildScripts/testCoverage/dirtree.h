#pragma once

#include <QString>

enum bgcolors {
	yellow = 0,
	white,
	pink,
	orange,
	light1,
	light2,
	light3
};

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
	bgcolors color;
};

class DirTree
{
	public:
		explicit DirTree(QString name, int localTesting, int localDocumented, int localTests = 0, bool isIgnored = false, bgcolors color = white);
		virtual ~DirTree();
		void addChild(DirNode *child, DirNode *parent);
		DirNode *getRoot() const;
		static DirNode *createNode(QString name, int localTesting, int localDocumented, int localTests = 0,  bool isIgnored = false, bgcolors color = white);
		void calculateTotalData();

	private:
		void calculateTotalDataForNode(DirNode *node);
		void delNode(DirNode *node);
		DirNode *root;
};
