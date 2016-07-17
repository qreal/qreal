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

#include "type.h"

namespace qrmc {

/// Represents a type that does not have its own graphical representation and is used in definitions of other types
/// instead. For example, enum types can be used as types of properties of visual language nodes.
class NonGraphicType : public Type
{
public:
	bool resolve() override;

	void print() override;

	bool isGraphicalType() const override;

	QString generateProperties(const QString &namesTemplate) const override;
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
	QString generateIsNodeOrEdge(const QString &lineTemplate) const override;
	QString generatePossibleEdges(const QString &lineTemplate) const override;

	QString generateNodeClass(const QString &classTemplate) override;
	QString generateEdgeClass(const QString &classTemplate) const override;
	QString generateResourceLine(const QString &resourceTemplate) const override;

protected:
	NonGraphicType(const Diagram &diagram, const qrRepo::LogicalRepoApi &api, const qReal::Id &id);
};

}
