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

namespace editorPluginTestingFramework {
const QString sourcesDir = "/sources";
const QString pluginsDir = "/plugins";

const QString pathToQrmcGeneratedCode = "";
const QString pathToQrxcGeneratedCode = "";

const QString pathToQrmcGeneratedPlugin = "/plugins";
const QString pathToQrxcGeneratedPlugin = "/plugins";

const QString pathToQRealRootFromQrmc = "../";
const QString pathToQRealRootFromQrxc = "../../";

const QString destDirForQrmc = "";
const QString destDirForQrxc = "";

const QString pathToQrxcQrmcMethodsToTest = "methodsToTestQrxcQrmc.txt";
const QString pathToQrxcInterpreterMethodsToTest = "methodsToTestInterpQrxc.txt";

const QString configurationFileName = "configurationParameters.xml";
const QString travisConfigurationFileName = "travisConfigurationParameters.xml";

const QString pathToTestMetamodel = "../qrtest/editorPluginTestingFramework/fileToTestWithTravis";

const QString tempValueForSettingsManager = "/unsaved";

const QString stringSeparator = "-----------------------------------";
}
