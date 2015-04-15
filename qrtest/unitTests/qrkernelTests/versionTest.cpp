/* Copyright 2007-2015 QReal Research Group
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

#include <qrkernel/version.h>

#include "gtest/gtest.h"

using namespace qReal;

TEST(VersionTest, loadFromStringTest)
{
	Version version = Version::fromString("3");
	ASSERT_EQ(version, Version(3));

	version = Version::fromString("3.14");
	ASSERT_EQ(version, Version(3, 14));

	version = Version::fromString("3.14.15");
	ASSERT_EQ(version, Version(3, 14, 15));

	version = Version::fromString("3 a");
	ASSERT_EQ(version, Version(3, 0, 0, Version::alpha));

	version = Version::fromString("3.1-Alpha1124");
	ASSERT_EQ(version, Version(3, 1, 0, Version::alpha, 1124));

	version = Version::fromString("3.0.0-B1");
	ASSERT_EQ(version, Version(3, 0, 0, Version::beta, 1));

	version = Version::fromString("3.14.0 BETA");
	ASSERT_EQ(version, Version(3, 14, 0, Version::beta));

	version = Version::fromString("3.14.0-RC1");
	ASSERT_EQ(version, Version(3, 14, 0, Version::releaseCandidate, 1));

	version = Version::fromString("");
	ASSERT_EQ(version, Version());

	version = Version::fromString("incorrect");
	ASSERT_EQ(version, Version());

	version = Version::fromString("3.1.4.15");
	ASSERT_EQ(version, Version());

	version = Version::fromString("3.1-wrong1");
	ASSERT_EQ(version, Version());

	version = Version::fromString("3.1-alpha1a");
	ASSERT_EQ(version, Version());
}

TEST(VersionTest, compareTest)
{
	ASSERT_TRUE(Version::fromString("3") == Version::fromString("3.0.0"));
	ASSERT_TRUE(Version::fromString("3.0.0-a") < Version::fromString("3.0.0-rc1"));
	ASSERT_TRUE(Version::fromString("3.0.0-a") < Version::fromString("3.0.0-b1"));
	ASSERT_TRUE(Version::fromString("3.0.0-b1") < Version::fromString("3.0.0-rc1"));
	ASSERT_TRUE(Version::fromString("3.0.0-rc1") < Version::fromString("3.0.0-rc2"));
	ASSERT_TRUE(Version::fromString("3.0.0-rc2") < Version::fromString("3.0.0"));
}

TEST(VersionTest, toStringTest)
{
	Version version = Version::fromString("3");
	ASSERT_EQ(version, Version::fromString(version.toString()));

	version = Version::fromString("3.0.0");
	ASSERT_EQ(version, Version::fromString(version.toString()));

	version = Version::fromString("3.0.0-a");
	ASSERT_EQ(version, Version::fromString(version.toString()));

	version = Version::fromString("3.0.0-b1");
	ASSERT_EQ(version, Version::fromString(version.toString()));

	version = Version::fromString("3.0.0-rc1");
	ASSERT_EQ(version, Version::fromString(version.toString()));
}
