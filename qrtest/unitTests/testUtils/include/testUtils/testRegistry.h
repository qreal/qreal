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

#pragma once

#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QVariant>

#include <QtCore/qglobal.h>

#include "testUtils/declSpec.h"

namespace qrTest {

/// Helper class that provides RAII-style storage for registry (or SettingsManager) settings. Settings may be altered
/// during test, but will be reverted to previous values when TestRegistry is destroyed.
class TEST_UTILS_EXPORT TestRegistry
{
public:
	~TestRegistry();

	/// Temporarily set registry setting with given key to a given value.
	void set(const QString &key, const QVariant &value);

private:
	/// Hash containing initial registry settings to be restored when test is done.
	QHash<QString, QVariant> mPreviousValues;
};

}
