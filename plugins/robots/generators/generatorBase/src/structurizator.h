#pragma once

#include <qrrepo/repoApi.h>
#include <qrkernel/ids.h>


#include <QSet>
#include <QMap>

namespace generatorBase {

namespace utils {

class Node {};

class SimpleNode : Node {
public:
	SimpleNode(const qReal::Id &id);

	qReal::Id id() const;
private:
	const qReal::Id &mId;
};

class IfNode : Node {
public:
	IfNode(Node *condition, Node *thenBranch, Node *elseBranch);

	Node *condition() const;
	Node *thenBranch() const;
	Node *elseBranch() const;
private:

	Node *mCondition;
	Node *mThenBranch;
	Node *mElseBranch;
	bool mIsIfThenForm;
};

}

class Structurizator : public QObject
{
	Q_OBJECT

public:
	Structurizator(const qrRepo::RepoApi &repo
						, const qReal::IdList &vertecesIds);



private:

	enum AbstractNode {
		id
	};


	QMap<qReal::Id, int> mMapIdToInt;


	qrRepo::RepoApi &mRepo;
	qReal::IdList initialIds;
};

}
