/* Copyright 2016 Dmitry Mordvinov
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

#include "metaMetaModel/metamodel.h"

using namespace qReal;

QList<ElementType *> Metamodel::elements(const QString &diagram) const
{
	return mElements[diagram].values();
}

ElementType &Metamodel::elementType(const QString &diagram, const QString &element) const
{
	Q_ASSERT_X(mElements.contains(diagram) && mElements[diagram].contains(element)
			, Q_FUNC_INFO, "No such entity in metamodel!");
	ElementType * const result = mElements[diagram][element];
	Q_ASSERT_X(result, Q_FUNC_INFO, "No such entity in metamodel!");
	return *result;
}

void Metamodel::addNode(qrgraph::Node &entity)
{
	ElementType * const type = dynamic_cast<ElementType *>(&entity);
	Q_ASSERT_X(type, Q_FUNC_INFO, "Attempt to add non-metamodel enitity!");
	Q_ASSERT_X(&type->metamodel() == this, Q_FUNC_INFO, "Attempt to add entity from other metamodel!");

	const QString diagram = type->diagram();
	const QString element = type->name();
	Q_ASSERT_X(!mElements[diagram][element], Q_FUNC_INFO, "Duplicate enitity in metamodel");

	mElements[diagram][element] = type;
	Multigraph::addNode(entity);
}
