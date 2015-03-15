#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore>
#include "qrkernel/ids.h"
#include "qrgui/plugins/pluginManager/explosion.h"
#include "qrgui/plugins/editorPluginInterface/editorInterface.h"

using namespace qReal;

namespace editorPluginTestingFramework {

class ConvertingMethods
{
public:

	/// converters for methods output converting into string list
// QList<EditorInterface::ExplosionData>
	static QStringList convertQListExplosionDataIntoStringList(QList<EditorInterface::ExplosionData> const &explDataList);
	static QStringList convertingQPairListIntoStringList(QList<QPair<QString, QString>>  const &qPairList);
	static QStringList convertIdListIntoStringList(qReal::IdList const idList);
	static QStringList convertStringIntoStringList(QString const &string);
	static QStringList convertBoolIntoStringList(bool const &boolValue);
	static QStringList convertIdIntoStringList(qReal::Id const &id);
	static QStringList convertIntIntoStringList(int const &integer);
	static QStringList convertExplosionListIntoStringList(QList<qReal::Explosion> explosion);

	/// converter to string with special symbols to parse in html
	static QString transformateOutput(const QStringList &output
			, qReal::Id const &id = qReal::Id::rootId()
			, QString const &name = ""
			);
		/// converter to string with special symbols to parse in html   ololo

	/// methods for parsing (common for htmlMaker and methodsCheckerForTravis)
	/// parses result for one method - because results can be equal, but in different order
	static QSet<QString> resultToCompare(QString const &method);
};

}
