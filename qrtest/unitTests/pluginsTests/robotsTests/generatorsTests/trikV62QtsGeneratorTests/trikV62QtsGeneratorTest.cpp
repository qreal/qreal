/* Copyright 2016 CyberTech Labs Ltd.
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

#include "trikV62QtsGeneratorTest.h"

#include <trikV62QtsGeneratorPlugin.h>

#include <testUtils/qrguiFacade.h>

using namespace qrTest;
using namespace qrTest::robotsTests::trikV62QtsGeneratorTests;
using namespace trik::qts;

void TrikV62QtsGeneratorTest::SetUp()
{
}

TEST_F(TrikV62QtsGeneratorTest, runProgramTest)
{
	QrguiFacade facade("unittests/basicTest.qrs");
}
