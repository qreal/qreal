#pragma once

#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtCore/QPair>

#include "type.h"

class Label;
class Diagram;

const int maxLineLength = 80;

class GraphicType : public Type
{
public:
	GraphicType(Diagram *diagram, qrRepo::RepoApi *api, qReal::Id const &id);
	virtual ~GraphicType();
	virtual bool init();
	virtual bool resolve();
	bool isResolving() const;

protected:
	typedef QPair<QPair<QString,QString>,QPair<bool,QString> > PossibleEdge;

	struct ContainerProperties {
		ContainerProperties();

		bool isSortContainer;
		int sizeOfForestalling;
		int sizeOfChildrenForestalling;
		bool isChildrenMovable;
		bool isMinimizingToChildren;
		bool isClass;
		bool isMaximizingChildren;
	};

	QStringList mParents;
	bool mVisible;
	int mWidth;
	int mHeight;
	QList<Label*> mLabels;
	QStringList mContains;
	ContainerProperties mContainerProperties;
	QStringList mConnections;
	QStringList mUsages;
	QList<PossibleEdge> mPossibleEdges;
	QStringList mBonusContextMenuFields;

	void copyFields(GraphicType *type) const;
	QString resourceName(QString const &resourceType) const;

private:
	class ResolvingHelper {
	public:
		ResolvingHelper(bool &resolvingFlag);
		~ResolvingHelper();
	private:
		bool &mResolvingFlag;
	};

	bool addProperty(Property *property);

	bool mResolving;
};
