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

namespace editorPluginTestingFramework {

/// Helper class that calls editor generator from MetaEditorSupport and produces .xml file containing metamodel
/// description ready for QRXC from .qrs with metamodel.
class MetamodelXmlGeneratorLauncher
{
public:
	MetamodelXmlGeneratorLauncher() = delete;

	/// Launches editor generator using given metamodel file name (in .qrs format) and produces .xml file ready for
	/// QRXC and .pro file which directs build (and actually calls QRXC from build scripts).
	static void launchEditorGenerator(const QString &fileName, const QString &pathToQRealSources
			, const QString &pathToGeneratedCode);
};

}
