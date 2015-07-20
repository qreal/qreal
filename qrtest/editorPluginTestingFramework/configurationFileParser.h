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

#include <QtCore/QString>
#include <QtXml/QDomDocument>

namespace editorPluginTestingFramework {

/// Class for parse file with configuration.
class ConfigurationFileParser
{
public:
	/// parses configuration file
	void parseConfigurationFile(const QString &configurationFile);

	/// returns qmake parameter from configuration file
	QString qmakeParameter() const;

	/// returns make parameter from configuration file
	QString makeParameter() const;

	/// returns configuration parameter (release or debug)
	QString configurationParameter() const;

	/// returns generated plugin extension (dll, so...)
	QString pluginExtension() const;

	/// returns prefix for generated plugin name (lib)
	QString prefix() const;

	/// returns path to QReal root
	QString qRealRootPath() const;

	/// returns "yes" if we want to generate html with testing results and "no" otherwise
	QString htmlGenerationParameter() const;

	/// returns path to generated binaries and plugins for qrxc
	QString generatedCodeDirQrxc() const;

	/// returns path to generated binaries and plugins for qrmc
	QString generatedCodeDirQrmc() const;

	/// returns path to generated result
	QString generatedDirHtml() const;

private:
	/// given tag, returns tag value from configuration file
	QString valueByTag(const QString &tag) const;

	QString mQmakeParameter;
	QString mMakeParameter;
	QString mConfigurationParameter;
	QString mPluginExtension;
	QString mPrefix;
	QString mQRealRootPath;
	QString mGenerateHtml;
	QString mGeneratedCodeDirQrxc;
	QString mGeneratedCodeDirQrmc;
	QString mGeneratedDirHtml;

	QDomDocument mXml;
};

}
