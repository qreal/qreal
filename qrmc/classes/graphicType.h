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

#pragma once

#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtCore/QPair>

#include "type.h"
#include "shape.h"

namespace qrmc {
	class Label;
	class Diagram;

	const int maxLineLength = 80;

	class GraphicType : public Type
	{
	public:
		GraphicType(Diagram *diagram, qrRepo::LogicalRepoApi *api, const qReal::Id &id);
		virtual ~GraphicType();
		virtual bool init(const QString &context);
		virtual bool resolve();

		virtual bool isGraphicalType() const;
		virtual QString generateProperties(const QString &lineTemplate) const;
		virtual QString generatePropertyDefaults(const QString &namesTemplate) const;
		virtual QString generatePropertyDisplayedNames(const QString &lineTemplate) const;
		virtual QString generateElementDescription(const QString &lineTemplate) const;
		virtual QString generateReferenceProperties(const QString &lineTemplate) const;
		virtual QString generatePortTypes(const QString &lineTemplate) const;
		virtual QString generatePropertyName(const QString &lineTemplate) const;
		virtual QString generateParents(const QString &lineTemplate) const;
		virtual QString generateContainers(const QString &lineTemplate) const;
		virtual QString generateConnections(const QString &lineTemplate) const;
		virtual QString generateUsages(const QString &lineTemplate) const;
		virtual QString generateEnums(const QString &lineTemplate) const;
		virtual QString generatePossibleEdges(const QString &lineTemplate) const;

		bool isResolving() const;
		QString generateElementDescriptionLine(const QString &lineTemplate) const;

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
		QStringList mChildren;
		bool mIsVisible;
		int mWidth;
		int mHeight;
		QStringList mContains;
		ContainerProperties mContainerProperties;
		QStringList mConnections;
		QStringList mUsages;
		QStringList mContextMenuItems;
		QList<PossibleEdge> mPossibleEdges;
		QStringList mBonusContextMenuFields;
		Shape mShape;

		void copyFields(GraphicType *type) const;
		QString resourceName(const QString &resourceType) const;

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

		bool mResolving;
		QString mDescription;
	};
}
