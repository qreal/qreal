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

#include <gtest/gtest.h>
#include <qrgraph/multigraph.h>

namespace qrTest {

class MultigraphTest : public testing::Test
{
public:
	static void setUpCase1(qrgraph::Multigraph &graph);
	static void setUpCase2(qrgraph::Multigraph &graph);

protected:
	void checkCase1(qrgraph::Multigraph &graph);
	void checkCase2(qrgraph::Multigraph &graph);

	void checkVertex(qrgraph::Multigraph &graph, const qrgraph::Node &node
			, QList<const qrgraph::Node *> outgoingNodesType0
			, QList<const qrgraph::Node *> outgoingNodesType1
			, QList<const qrgraph::Node *> incomingNodesType0
			, QList<const qrgraph::Node *> incomingNodesType1);
};

}
