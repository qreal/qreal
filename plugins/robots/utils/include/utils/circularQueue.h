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

#pragma once

#include <QtCore/QVector>

namespace utils {

/// An implementation of circular queue of fixed size.
/// @warning: This implementation is not thread safe but reentrant.
template<typename T>
class CircularQueue
{
public:
	explicit CircularQueue(int maxSize = 256);

	/// Pushes the given value into the tail of the queue. Works O(1), without memory reallocation.
	void enqueue(const T &value);

	/// Pops the front value of the queue and returns it. Works O(1), without memory reallocation.
	T dequeue();

	/// Returns a const reference to the queue's head item. This function assumes that the queue isn't empty.
	const T &head() const;

	/// Returns a mutable reference to the queue's head item. This function assumes that the queue isn't empty.
	T &head();

	/// Returns a const reference to the queue's tail item. This function assumes that the queue isn't empty.
	const T &tail() const;

	/// Returns a mutable reference to the queue's tail item. This function assumes that the queue isn't empty.
	T &tail();

	/// Returns a const reference to the queue's item that was enqueued \a n items after head.
	/// This function assumes that the queue's size is greater or equal to (\a n + 1).
	/// If \a n is zero head item will be returned.
	const T &nthFromHead(int n) const;

	/// Returns a mutablereference to the queue's item that placed \a n items after head.
	/// This function assumes that the queue's size is greater or equal to (\a n + 1).
	/// If \a n is zero head item will be returned.
	T &nthFromHead(int n);

	/// Returns a const reference to the queue's item that was enqueued \a n items before tail.
	/// This function assumes that the queue's size is greater or equal to (\a n + 1).
	/// If \a n is zero tail item will be returned.
	const T &nthFromTail(int n) const;

	/// Returns a mutablereference to the queue's item that placed \a n items before tail.
	/// This function assumes that the queue's size is greater or equal to (\a n + 1).
	/// If \a n is zero tail item will be returned.
	T &nthFromTail(int n);

	/// Returnes a number of currently queued item.
	int size() const;

	/// Returns true if queue does not contain items.
	bool isEmpty() const;

	/// Resets a queue, removes all items from it. Works O(1), without memory reallocation.
	void clear();

private:
	void increment(int &indexer) const;
	int previous(int indexer, int steps = 1) const;
	int next(int indexer, int steps = 1) const;

	QVector<T> mBody;
	int mHead;
	int mTail;
	bool mIsEmpty;
};

}

// ------------------------------------------- Implementation -------------------------------------------

template<typename T>
utils::CircularQueue<T>::CircularQueue(int maxSize)
	: mBody(maxSize)
	, mHead(0)
	, mTail(0)
	, mIsEmpty(true)
{
}

template<typename T>
void utils::CircularQueue<T>::enqueue(const T &value)
{
	Q_ASSERT_X(mTail != mHead || mIsEmpty, "utils::CircularQueue<T>::enqueue", "Queue overflow");
	mBody[mTail] = value;
	increment(mTail);
	mIsEmpty = false;
}

template<typename T>
T utils::CircularQueue<T>::dequeue()
{
	Q_ASSERT_X(!isEmpty(), "utils::CircularQueue<T>::enqueue", "Queue is empty");
	const int oldHead = mHead;
	increment(mHead);
	mIsEmpty = (mHead == mTail);
	return mBody[oldHead];
}

template<typename T>
const T &utils::CircularQueue<T>::head() const
{
	Q_ASSERT(!isEmpty());
	return mBody[mHead];
}

template<typename T>
T &utils::CircularQueue<T>::head()
{
	Q_ASSERT(!isEmpty());
	return mBody[mHead];
}

template<typename T>
const T &utils::CircularQueue<T>::tail() const
{
	Q_ASSERT(!isEmpty());
	return mBody[previous(mTail)];
}

template<typename T>
T &utils::CircularQueue<T>::tail()
{
	Q_ASSERT(!isEmpty());
	return mBody[previous(mTail)];
}

template<typename T>
const T &utils::CircularQueue<T>::nthFromHead(int n) const
{
	Q_ASSERT(size() >= n + 1);
	return mBody[next(mHead, n)];
}

template<typename T>
T &utils::CircularQueue<T>::nthFromHead(int n)
{
	Q_ASSERT(size() >= n + 1);
	return mBody[next(mHead, n)];
}

template<typename T>
const T &utils::CircularQueue<T>::nthFromTail(int n) const
{
	Q_ASSERT(size() >= n + 1);
	Q_ASSERT(n < mBody.size());
	return mBody[previous(mTail, n)];
}

template<typename T>
T &utils::CircularQueue<T>::nthFromTail(int n)
{
	Q_ASSERT(size() >= n + 1);
	Q_ASSERT(n < mBody.size());
	return mBody[previous(mTail, n + 1)];
}

template<typename T>
int utils::CircularQueue<T>::size() const
{
	return mTail > mHead || mIsEmpty ? mTail - mHead : mBody.size() - mHead + mTail;
}

template<typename T>
bool utils::CircularQueue<T>::isEmpty() const
{
	return mIsEmpty;
}

template<typename T>
void utils::CircularQueue<T>::clear()
{
	mHead = mTail = 0;
	mIsEmpty = true;
}

template<typename T>
void utils::CircularQueue<T>::increment(int &indexer) const
{
	++indexer;
	indexer %= mBody.size();
}

template<typename T>
int utils::CircularQueue<T>::previous(int indexer, int steps) const
{
	return (mBody.size() + indexer - steps) % mBody.size();
}

template<typename T>
int utils::CircularQueue<T>::next(int indexer, int steps) const
{
	return (indexer + steps) % mBody.size();
}
