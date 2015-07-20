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

#include "type.h"

namespace qrmc {
	class NonGraphicType : public Type
	{
	public:
		virtual bool resolve();

		virtual void print();

		virtual bool isGraphicalType() const;

		virtual QString generateProperties(const QString &namesTemplate) const;
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
		virtual QString generateIsNodeOrEdge(const QString &lineTemplate) const;
		virtual QString generatePossibleEdges(const QString &lineTemplate) const;

		virtual QString generateNodeClass(const QString &classTemplate);
		virtual QString generateEdgeClass(const QString &classTemplate) const;
		virtual QString generateResourceLine(const QString &resourceTemplate) const;

	protected:
		NonGraphicType(Diagram *diagram, qrRepo::LogicalRepoApi *api, const qReal::Id &id);
	};
}
