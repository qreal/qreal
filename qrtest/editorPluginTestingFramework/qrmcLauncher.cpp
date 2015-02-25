#include "qrmcLauncher.h"
#include "qrmc/metaCompiler.h"
#include "defs.h"

using namespace qReal;
using namespace qrmc;
using namespace qrRepo;
using namespace editorPluginTestingFramework;

void QrmcLauncher::launchQrmc(QString const &fileName, QString const &pathToQrmc, QString const &pathToGeneratedCode)
{
	qDebug() << "STARTING QRMC LAUNCHING";
	QString normalizedFileName = fileName;
	if (!fileName.contains(".qrs")) {
		normalizedFileName += ".qrs";
	}

	RepoApi *const mRepoApi = new RepoApi(normalizedFileName);
	MetaCompiler metaCompiler(pathToQrmc, mRepoApi);
	IdList const metamodels = mRepoApi->children(Id::rootId());

	foreach (Id const &key, metamodels) {
		QString const &objectType = key.element();
		if (objectType == "MetamodelDiagram" && mRepoApi->isLogicalElement(key)) {
			QString const &nameOfMetamodel = mRepoApi->stringProperty(key, "name");

			if (!metaCompiler.compile(nameOfMetamodel)) {
				qDebug() << "compilation failed";
			}
		}
	}
	qDebug() << stringSeparator;
}
