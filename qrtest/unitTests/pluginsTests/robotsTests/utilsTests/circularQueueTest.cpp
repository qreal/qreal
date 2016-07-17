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

#include "circularQueueTest.h"

using namespace qrTest::robotsTests::utilsTests;

TEST_F(CircularQueueTests, smokeTest)
{
	utils::CircularQueue<int> queue;
	ASSERT_TRUE(queue.isEmpty());
	ASSERT_EQ(queue.size(), 0);

	queue.enqueue(1);
	queue.enqueue(2);
	ASSERT_FALSE(queue.isEmpty());
	ASSERT_EQ(queue.size(), 2);

	ASSERT_EQ(queue.dequeue(), 1);
	ASSERT_FALSE(queue.isEmpty());
	ASSERT_EQ(queue.size(), 1);

	ASSERT_EQ(queue.dequeue(), 2);
	ASSERT_TRUE(queue.isEmpty());
	ASSERT_EQ(queue.size(), 0);
}

TEST_F(CircularQueueTests, clearTest)
{
	utils::CircularQueue<QString> queue;
	ASSERT_TRUE(queue.isEmpty());
	ASSERT_EQ(queue.size(), 0);

	queue.enqueue("1");
	queue.enqueue("2");
	queue.enqueue("3");
	queue.enqueue("4");
	ASSERT_FALSE(queue.isEmpty());
	ASSERT_EQ(queue.size(), 4);

	queue.clear();
	ASSERT_TRUE(queue.isEmpty());
	ASSERT_EQ(queue.size(), 0);

	queue.enqueue("5");
	queue.enqueue("6");
	queue.enqueue("7");
	queue.enqueue("8");
	ASSERT_FALSE(queue.isEmpty());
	ASSERT_EQ(queue.size(), 4);
}

TEST_F(CircularQueueTests, nearlyOverflowTest)
{
	const int halfSize = 5;
	const int size = halfSize + halfSize;
	utils::CircularQueue<int> queue(size);
	ASSERT_TRUE(queue.isEmpty());
	ASSERT_EQ(queue.size(), 0);

	for (int i = 0; i < size; ++i) {
		queue.enqueue(i);
		ASSERT_FALSE(queue.isEmpty());
		ASSERT_EQ(queue.size(), i + 1);
		ASSERT_EQ(queue.tail(), i);
	}

	ASSERT_EQ(queue.nthFromHead(3), 3);
	ASSERT_EQ(queue.nthFromTail(3), size - 4);

	ASSERT_FALSE(queue.isEmpty());
	ASSERT_EQ(queue.size(), size);
	ASSERT_EQ(queue.head(), 0);
	ASSERT_EQ(queue.tail(), size - 1);

	for (int i = 0; i < halfSize; ++i) {
		ASSERT_EQ(queue.dequeue(), i);
		ASSERT_EQ(queue.size(), size - i - 1);
	}

	for (int i = 0; i < halfSize; ++i) {
		queue.enqueue(size + i);
		ASSERT_EQ(queue.tail(), size + i);
		ASSERT_EQ(queue.size(), halfSize + i + 1);
	}

	ASSERT_EQ(queue.nthFromHead(halfSize), size);
	ASSERT_EQ(queue.nthFromTail(halfSize), size - 1);

	for (int i = 0; i < size; ++i) {
		ASSERT_EQ(queue.dequeue(), halfSize + i);
		ASSERT_EQ(queue.size(), size - i - 1);
	}

	ASSERT_TRUE(queue.isEmpty());
	ASSERT_EQ(queue.size(), 0);
}
