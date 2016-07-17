/* Copyright 2007-2016 QReal Research Group, Yurii Litvinov
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

#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtCore/QPair>

#include "type.h"
#include "shape.h"

namespace qrmc {

class Label;
class Diagram;

/// Represents metatype that can have its graphical representation and corresponding entry in a palette.
class GraphicType : public Type
{
public:
	GraphicType(const Diagram &diagram, const qrRepo::LogicalRepoApi &api
			, const qReal::Id &id, const QString &targetDirectory);

	~GraphicType() override;
	bool init(const QString &context) override;
	bool resolve() override;

	bool isGraphicalType() const override;
	QString generateProperties(const QString &lineTemplate) const override;
	QString generatePropertyDefaults(const QString &namesTemplate) const override;
	QString generatePropertyDisplayedNames(const QString &lineTemplate) const override;
	QString generateElementDescription(const QString &lineTemplate) const override;
	QString generateReferenceProperties(const QString &lineTemplate) const override;
	QString generatePortTypes(const QString &lineTemplate) const override;
	QString generatePropertyName(const QString &lineTemplate) const override;
	QString generateParents(const QString &lineTemplate) const override;
	QString generateContainers(const QString &lineTemplate) const override;
	QString generateConnections(const QString &lineTemplate) const override;
	QString generateUsages(const QString &lineTemplate) const override;
	QString generateEnums(const QString &lineTemplate) const override;
	QString generatePossibleEdges(const QString &lineTemplate) const override;

	bool isResolving() const override;

	QString generateElementDescriptionLine(const QString &lineTemplate) const;

protected:
	typedef QPair<QPair<QString, QString>, QPair<bool, QString>> PossibleEdge;

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

	void copyFields(GraphicType *type) const;
	QString resourceName(const QString &resourceType) const;
	QString targetDirectory() const;

	QStringList mParents;
	QStringList mChildren;
	bool mIsVisible = false;
	int mWidth = 0;
	int mHeight = 0;
	QStringList mContains;
	ContainerProperties mContainerProperties;
	QStringList mConnections;
	QStringList mUsages;
	QStringList mContextMenuItems;
	QList<PossibleEdge> mPossibleEdges;
	QStringList mBonusContextMenuFields;
	Shape mShape;

private:
	class ResolvingHelper {
	public:
		ResolvingHelper(bool &resolvingFlag);
		~ResolvingHelper();
	private:
		bool &mResolvingFlag;
	};

	bool addProperty(Property *property);
	bool initPossibleEdges();
	void initShape();

	bool mResolving = false;
	QString mDescription;
	QString mTargetDirectory;
};

}
