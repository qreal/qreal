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

#include "configurationFileParser.h"
#include "defs.h"

#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include "qrutils/xmlUtils.h"

using namespace editorPluginTestingFramework;

void ConfigurationFileParser::parseConfigurationFile(const QString &configurationFile)
{
	mXml = utils::xmlUtils::loadDocument(configurationFile);

	mQmakeParameter = valueByTag("qmake");
	mMakeParameter = valueByTag("make");
	mConfigurationParameter = valueByTag("configuration");
	mPluginExtension = valueByTag("pluginExtension");
	mPrefix = valueByTag("prefix");
	mQRealRootPath = valueByTag("qrealRoot");
	mGenerateHtml = valueByTag("generateHtml");
	mGeneratedCodeDirQrxc = valueByTag("generatedCodeDirQrxc");
	mGeneratedCodeDirQrmc = valueByTag("generatedCodeDirQrmc");
	mGeneratedDirHtml = valueByTag("generatedDirHtml");
}

QString ConfigurationFileParser::valueByTag(const QString &tag) const
{
	QDomNode node = mXml.elementsByTagName(tag).at(0);
	return node.toElement().text();
}

QString ConfigurationFileParser::qmakeParameter() const
{
	return mQmakeParameter;
}

QString ConfigurationFileParser::makeParameter() const
{
	return mMakeParameter;
}

QString ConfigurationFileParser::configurationParameter() const
{
	return mConfigurationParameter;
}

QString ConfigurationFileParser::pluginExtension() const
{
	return mPluginExtension;
}

QString ConfigurationFileParser::prefix() const
{
	return mPrefix;
}

QString ConfigurationFileParser::qRealRootPath() const
{
	return mQRealRootPath;
}

QString ConfigurationFileParser::htmlGenerationParameter() const
{
	return mGenerateHtml;
}

QString ConfigurationFileParser::generatedCodeDirQrxc() const
{
	return mGeneratedCodeDirQrxc;
}
QString ConfigurationFileParser::generatedCodeDirQrmc() const
{
	return mGeneratedCodeDirQrmc;
}

QString ConfigurationFileParser::generatedDirHtml() const
{
	return mGeneratedDirHtml;
}
