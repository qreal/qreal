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
#include <functional>

#include "utilsDeclSpec.h"

namespace utils {

/// A base for QVariantList wrappers.
class ROBOTS_UTILS_EXPORT ObjectsSetBase : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QVariant first READ first)
	Q_PROPERTY(QVariant last READ last)
	Q_PROPERTY(bool isEmpty READ isEmpty)
	Q_PROPERTY(int size READ size)

public:
	typedef std::function<void(const QVariant &)> Visitor;

	explicit ObjectsSetBase(QObject *parent = 0);

	/// Returns an object that was added to this collection first of all.
	/// If the collection is empty returns invalid QVariant.
	virtual QVariant first() const = 0;

	/// Returns an object that was added to this collection at the last turn.
	/// If the collection is empty returns invalid QVariant.
	virtual QVariant last() const = 0;

	/// Returns true if this collection has no objects. Otherwise returns false.
	virtual bool isEmpty() const = 0;

	/// Returns the count of objects in this collection.
	virtual int size() const = 0;

	/// Sequentially calls the \a visitor for each element in the collection.
	virtual void iterate(const Visitor &visitor) const = 0;
};

/// A helper trait for storing and accessing an existing collection of objects via Qt reflection.
/// Just QObject wrapper arround the QList<T>.
template<typename T>
class ObjectsSet : public ObjectsSetBase
{
public:
	/// Constructor.
	/// Creates new collection wraps some existing data source.
	/// @param list A collection passed by reference. This list must not be temporal instance
	/// cause it is not copied.
	explicit ObjectsSet(const QList<T> &list, QObject *parent = 0)
		: ObjectsSetBase(parent)
		, mList(list)
	{
	}

	/// Returns reference to original data source.
	const QList<T> &data() const
	{
		return mList;
	}

	QVariant first() const override
	{
		return mList.isEmpty() ? QVariant() : QVariant::fromValue<T>(mList.first());
	}

	QVariant last() const override
	{
		return mList.isEmpty() ? QVariant() : QVariant::fromValue<T>(mList.last());
	}

	bool isEmpty() const override
	{
		return mList.isEmpty();
	}

	int size() const override
	{
		return mList.size();
	}

	void iterate(const Visitor &visitor) const override
	{
		for (const T &item : mList) {
			visitor(QVariant::fromValue<T>(item));
		}
	}

private:
	const QList<T> &mList;  // Doesn`t take ownership.
};

/// A wrapper arround the QVariantList for getting its properties and elements via Qt reflection.
class ROBOTS_UTILS_EXPORT VariantSet : public ObjectsSetBase
{
public:
	explicit VariantSet(QObject *parent = 0);
	explicit VariantSet(const QVariantList &list, QObject *parent = 0);

	QVariant first() const override;
	QVariant last() const override;
	bool isEmpty() const override;
	int size() const override;
	void iterate(const Visitor &visitor) const override;

	/// Returns reference to underlying QVariantList instance.
	const QVariantList &data() const;

	/// Appends the given item to the tail of this list.
	void add(const QVariant &item);

	/// Appends the given items collection to the tail of this list.
	void addRange(const QVariantList &item);

	/// Clears this collection without disposing items themselves.
	void clear();

private:
	QVariantList mList;
};

}
