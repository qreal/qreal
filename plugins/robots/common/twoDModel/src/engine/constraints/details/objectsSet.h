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

#pragma once

#include <QtCore/QVariant>

namespace twoDModel {
namespace constraints {
namespace details {

/// A helper trait for storing and accessing a group of objects via Qt reflection.
/// Just QObject wrapper above the QVariantList.
class ObjectsSet : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QVariant first READ first)
	Q_PROPERTY(QVariant last READ last)
	Q_PROPERTY(bool isEmpty READ isEmpty)
	Q_PROPERTY(int size READ size)

public:
	explicit ObjectsSet(QObject *parent = 0);

	/// Returns an object that was added to this collection first of all.
	/// If the collection is empty returns invalid QVariant.
	QVariant first() const;

	/// Returns an object that was added to this collection at the last turn.
	/// If the collection is empty returns invalid QVariant.
	QVariant last() const;

	/// Returns true if this collection has no objects. Otherwise returns false.
	bool isEmpty() const;

	/// Returns the count of objects in this collection.
	int size() const;

	/// Removes all object from this collection without deletting objects itself.
	void clear();

	/// Appends the given \a item to the end of this collection.
	void add(const QVariant &item);

private:
	QVariantList mList;
};

}
}
}
