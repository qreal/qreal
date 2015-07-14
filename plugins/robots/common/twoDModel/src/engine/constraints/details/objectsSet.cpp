/* Copyright 2015 QReal Research Group, Dmitry Mordvinov
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

#include "objectsSet.h"

using namespace twoDModel::constraints::details;

ObjectsSet::ObjectsSet(QObject *parent)
	: QObject(parent)
{
}

QVariant ObjectsSet::first() const
{
	return mList.isEmpty() ? QVariant() : mList.first();
}

QVariant ObjectsSet::last() const
{
	return mList.isEmpty() ? QVariant() : mList.last();
}

bool ObjectsSet::isEmpty() const
{
	return mList.isEmpty();
}

int ObjectsSet::size() const
{
	return mList.size();
}

void ObjectsSet::clear()
{
	mList.clear();
}

void ObjectsSet::add(const QVariant &item)
{
	mList << item;
}
