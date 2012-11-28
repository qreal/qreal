#include "qrmcLauncher.h"
#include "../../qrmc/metaCompiler.h"
#include "defs.h"

using namespace qReal;
using namespace qrmc;
using namespace qrRepo;

QrmcLauncher::QrmcLauncher()
{
}

void QrmcLauncher::launchQrmc(QString &fileName, QString const &pathToQrmc)
{
	if (!fileName.contains(".qrs")) {
		fileName += ".qrs";
	}

	QString const &destDir = "qrtest/binaries/plugins/qrmc";
	// DESTDIR = pathToQRealRoot + destDir

	qrRepo::RepoApi *mRepoApi = new qrRepo::RepoApi(fileName);
	MetaCompiler metaCompiler(pathToQrmc, mRepoApi);
	IdList const metamodels = mRepoApi->children(Id::rootId());

	foreach (Id const &key, metamodels) {
		QString const &objectType = key.element();
		if (objectType == "MetamodelDiagram" && mRepoApi->isLogicalElement(key)) {
			QString const &nameOfMetamodel = mRepoApi->stringProperty(key, "name");

			if (!metaCompiler.compile(nameOfMetamodel, pathToQRealRootFromQrmc, pathToQrmcGeneratedCode, destDir)) {
				qDebug() << "compilation failed";
			}
		}
	}
}
