/* Copyright 2007-2016 QReal Research Group, Yurii Litvinov
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

#include <QtCore/QString>
#include <QtCore/QFileInfo>

namespace editorPluginTestingFramework {

/// Helper class that builds generated code for plugins using qmake and common plugins build system. It actually
/// calls QRXC and QRMC via build scripts. Note that it will modify generated build scripts to facilitate testing
/// (to be able to get two versions of the same plugin).
class PluginCompiler
{
public:
	PluginCompiler() = delete;

	/// Enum with available metamodel compilers to use --- QRXC or QRMC.
	enum class MetamodelCompiler
	{
		qrxc
		, qrmc
	};

	/// Compiles code that is generated for QRXC and QRMC using qmake and common plugins build system.
	/// Note that it will compile only the first metamodel in .qrs file.
	/// @returns file with compiled plugin binary.
	/// @throws qReal::Exception if somwthing went wrong.
	static QFileInfo compilePlugin(const QString &fileWithMetamodel
		, const QString &directoryToCompile
		, const QString &pathToQmake
		, const QString &pathToMake
		, const QString &configurationParameter
		, MetamodelCompiler metamodelCompiler
		);

private:
	/// Determines name of a generated plugin by looking on metamodel contents.
	static QString getPluginName(const QString &fileWithMetamodel);

	/// Changes DESTDIR for QRXC metamodel compiler to compile binaries to separate folder.
	static QString patchProFileForQrxc(const QString &pluginName, const QString &directoryToCompile);

	/// Changes DESTDIR for QRMC metamodel compiler to compile binaries to separate folder.
	static QString patchProFileForQrmc(const QString &pluginName, const QString &directoryToCompile);
};

}
