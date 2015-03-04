#pragma once

#include <functional>
#include <QtCore/QList>

#include <qrgui/plugins/toolPluginInterface/projectConverter.h>

namespace interpreterCore {

/// Provides a number of converters that transform some incompatible features in old save versions.
class SaveConvertionManager
{
public:
	/// Returns a list of converters for robots editor.
	static QList<qReal::ProjectConverter> converters();

private:
	/// Returns a converter that restricts all saves made by editors till 3.0.0 alpha1.
	static qReal::ProjectConverter before300Alpha1Converter();

	/// Returns a converter that transforms JA1-form ports to the A1-form ones and
	/// sensor1- and digitSensor1-form variables into sensorA1- and sensorD1-form ones.
	static qReal::ProjectConverter from300Alpha4to300Alpha5Converter();

	/// Returns a converter that apppends quotes to text blocks (like PrintText or TrikSay).
	static qReal::ProjectConverter from300Beta2to300rc1Converter();

	/// Returns a converter that changes button sensor variables and button blocks.
	static qReal::ProjectConverter from300to301Converter();

	/// Returns a converter that adds quotes for "System" block.
	static qReal::ProjectConverter from301to302Converter();

	/// Replaces "interpreterBase" to "kitBase", "commonTwoDModel" to "twoDModel" and
	/// "*kitInterpreter" to "*" in devices names.
	static qReal::ProjectConverter from302to310Converter();

	static bool isRobotsDiagram(const qReal::Id &diagram);
	static qReal::IdList elementsOfRobotsDiagrams(const qReal::LogicalModelAssistInterface &logicalApi);
	static QString editor();

	/// Helper method, creates "typical" that applies a list of filters to a block if block satisfies given condition.
	/// @param oldVersion - version from which converter can convert.
	/// @param newVersion - version to which converter converts save.
	/// @param filters - a list of elementary conversion steps. Each filter is a function that takes Id of a block
	///        and logical model API and shall return true if it has changed something or false if it did nothing.
	///        Every filter will be called for every logical block in a save.
	/// @param condition - logical predicate that shall be true for a block to be processed.
	static qReal::ProjectConverter constructConverter(const QString &oldVersion, const QString &newVersion
			, const QList<std::function<bool(const qReal::Id &
					, qReal::LogicalModelAssistInterface &)> > &filters
			, const std::function<bool(const qReal::Id &)> &condition
					= [] (const qReal::Id &block) { return !block.isNull(); }
			);

	/// Helper method, constructs property replace filter. Takes map in form { {<from>, <to>}, ... } and applies
	/// replacements coded in this map to every property of a block.
	static std::function<bool(const qReal::Id &, qReal::LogicalModelAssistInterface &)> replace(
			const QMap<QString, QString> &replacementRules);

	/// Helper method, constructs deleting filter. If block in old save was removed from metamodel of a new version,
	/// it is safer to delete it completely.
	static std::function<bool(const qReal::Id &, qReal::LogicalModelAssistInterface &)> deleteBlocks(
			const QStringList &blocks);

	/// Helper method, constructs quoting filter. Puts '"' around value of given property of all blocks with given type.
	static std::function<bool(const qReal::Id &, qReal::LogicalModelAssistInterface &)> quote(
			const QString &blockType, const QString &property);
};

}
