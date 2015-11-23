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

#pragma once

#include <qrkernel/kernelDeclSpec.h>

namespace qReal {

/// Provides some useful methods for getting information about external environment.
class QRKERNEL_EXPORT PlatformInfo
{
public:
	/// Returns printable information about operating system which runs this application.
	static QString prettyOsVersion();

	/// Returns QCoreApplication::applicationDirPath on every platform besides Mac OS X
	/// and path to directory containing application bundle on Mac
	/// (3 levels up from QCoreApplication::applicationDirPath()) if current executable is bundled.
	static QString applicationDirPath();

	/// Returns a path to file that contains additional settings for concrete platform.
	/// Such config will be loaded when application starts if nessesary. This config can be overrided
	/// with --config option of main executable. Usually such config will contain paths to application
	/// resources (such as plugins, translations and docs), that may differ in different installers
	/// (.deb package will put translations to /usr/share, while QtIFW will put it to home dir).
	static QString defaultPlatformConfigPath();

	/// Replaces ./ prefix of the string to application dir path, returns resulting string.
	static QString invariantPath(const QString &path);

	/// Useful alias for invariantPath(qReal::SettingsManager::value(\a settingsKey).toString());
	static QString invariantSettingsPath(const QString &settingsKey);
};

}
