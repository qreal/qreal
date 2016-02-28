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

#include <QtCore>
#include <qrkernel/ids.h>
#include <qrgui/plugins/pluginManager/explosion.h>
#include <qrgui/plugins/metaMetaModel/metamodel.h>

namespace editorPluginTestingFramework {

class ConvertingMethods
{
public:
	/// converters for methods output converting into string list
	/// from QListExplosionData
	static QStringList convertQListExplosionDataIntoStringList(const QList<qReal::Metamodel::ExplosionData> &explDataList);

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
