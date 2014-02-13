#pragma once
#include <QtCore/QString>

namespace editorPluginTestingFramework {

QString const pathToQRealRoot = "../../";

QString const pathToSourceCode = "../qrtest/editorPluginTestingFramework/";

QString const binariesDir = "../qrtest/binaries";
QString const sourcesDir = "../qrtest/binaries/sources";
QString const pluginsDir = "../qrtest/binaries/plugins";

QString const pathToQrmcGeneratedCode = "../qrtest/binaries/sources/qrmc";
QString const pathToQrxcGeneratedCode = "../qrtest/binaries/sources/qrxc";

QString const pathToQrmcGeneratedPlugin = "../qrtest/binaries/plugins/qrmc";
QString const pathToQrxcGeneratedPlugin = "../qrtest/binaries/plugins/qrxc";

QString const pathToQRealRootFromQrmc = "../../../../../";
QString const pathToQRealRootFromQrxc = "../../../../";

QString const destDirForQrmc = "qrtest/binaries/plugins/qrmc";
QString const destDirForQrxc = "qrtest/binaries/plugins/qrxc";

QString const pathToQrxcQrmcMethodsToTest = "methodsToTest/methodsToTestQrxcQrmc.txt";
QString const pathToQrxcInterpreterMethodsToTest = "methodsToTest/methodsToTestInterpQrxc.txt";

QString const configurationFileName = "configurationParameters.xml";
QString const travisConfigurationFileName = "travisConfigurationParameters.xml";

QString const pathToTestMetamodel = "../qrtest/editorPluginTestingFramework/fileToTestWithTravis";

QString const tempValueForSettingsManager = "/unsaved";

QString const stringSeparator = "-----------------------------------";
}
