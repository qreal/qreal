/* Copyright 2015 CyberTech Labs Ltd.
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

#include "utils/objectsSet.h"

using namespace utils;

ObjectsSetBase::ObjectsSetBase(QObject *parent)
	: QObject(parent)
{
}

VariantSet::VariantSet(QObject *parent)
	: ObjectsSetBase(parent)
{
}

VariantSet::VariantSet(const QVariantList &list, QObject *parent)
	: ObjectsSetBase(parent)
	, mList(list)
{
}

const QVariantList &VariantSet::data() const
{
	return mList;
}

QVariant VariantSet::first() const
{
	return mList.isEmpty() ? QVariant() : mList.first();
}

QVariant VariantSet::last() const
{
	return mList.isEmpty() ? QVariant() : mList.last();
}

bool VariantSet::isEmpty() const
{
	return mList.isEmpty();
}

int VariantSet::size() const
{
	return mList.size();
}

void VariantSet::iterate(const ObjectsSetBase::Visitor &visitor) const
{
	for (const QVariant &item : mList) {
		visitor(item);
	}
}

void VariantSet::add(const QVariant &item)
{
	mList << item;
}

void VariantSet::addRange(const QVariantList &items)
{
	mList += items;
}

void VariantSet::clear()
{
	mList.clear();
}
