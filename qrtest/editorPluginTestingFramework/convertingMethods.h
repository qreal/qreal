#pragma once

#include <QtCore>
#include <qrkernel/ids.h>
#include <qrgui/plugins/pluginManager/explosion.h>
#include <qrgui/plugins/editorPluginInterface/editorInterface.h>

namespace editorPluginTestingFramework {

class ConvertingMethods
{
public:
	/// converters for methods output converting into string list
	/// from QListExplosionData
	static QStringList convertQListExplosionDataIntoStringList(const QList<qReal::EditorInterface::ExplosionData> &explDataList);

	/// from QPairList
	static QStringList convertingQPairListIntoStringList(const QList<QPair<QString, QString>> &qPairList);

	/// from IdList
	static QStringList convertIdListIntoStringList(const qReal::IdList &idList);

	/// from String
	static QStringList convertStringIntoStringList(const QString &string);

	/// from Bool
	static QStringList convertBoolIntoStringList(const bool &boolValue);

	/// from Id
	static QStringList convertIdIntoStringList(const qReal::Id &id);

	/// from Int
	static QStringList convertIntIntoStringList(const int &integer);

	/// from ExplosionList
	static QStringList convertExplosionListIntoStringList(const QList<qReal::Explosion> &explosion);

	/// converter to string with special symbols to parse in html
	static QString transformateOutput(const QStringList &output
			, const qReal::Id &id = qReal::Id::rootId()
			, const QString &name = ""
			);

	/// methods for parsing (common for htmlMaker and methodsCheckerForTravis)
	/// parses result for one method - because results can be equal, but in different order
	static QSet<QString> resultToCompare(const QString &method);
};

}
