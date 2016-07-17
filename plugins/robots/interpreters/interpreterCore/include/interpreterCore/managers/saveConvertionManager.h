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
	typedef std::function<bool(const qReal::Id &, qReal::LogicalModelAssistInterface &)> LogicalFilter;
	typedef std::function<bool(const qReal::Id &, qReal::GraphicalModelAssistInterface &)> GraphicalFilter;
	typedef std::function<qReal::Id(const qReal::Id &, qReal::GraphicalModelAssistInterface &)> GraphicalReplacer;
	typedef std::function<void(const qReal::Id &, const qReal::Id &
			, qReal::GraphicalModelAssistInterface &)> GraphicalConstructor;

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

	/// Replaces "interpreterBase" to "kitBase", "commonTwoDModel" to "twoDModel",
	/// "*kitInterpreter" to "*" in devices names, replaces robot model names (in NXT case adds "Usb" to robot model,
	/// in TRIK removes v6 from robot model name).
	static qReal::ProjectConverter from302to310Converter();

	/// Replaces scalar gyroscope and accelerometer variables to vector ones.
	static qReal::ProjectConverter from312to313Converter();

	static bool isRobotsDiagram(const qReal::Id &element);
	static bool isDiagramType(const qReal::Id &element);
	static bool isEdgeType(const qReal::Id &element);
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
			, const QList<LogicalFilter> &logicalFilters
			, const QList<GraphicalFilter> &graphicalFilters = {}
			, const std::function<bool(const qReal::Id &)> &condition
					= [] (const qReal::Id &block) { return !block.isNull(); }
			);

	/// Helper method, constructs property replace filter. Takes map in form { {<from>, <to>}, ... } and applies
	/// replacements coded in this map to every property of a block.
	static LogicalFilter replace(const QMap<QString, QString> &replacementRules);

	/// Helper method, constructs deleting filter. If block in old save was removed from metamodel of a new version,
	/// it is safer to delete it completely.
	static LogicalFilter deleteBlocks(const QStringList &blocks);

	/// Helper method, constructs quoting filter. Puts '"' around value of given property of all blocks with given type.
	static LogicalFilter quote(const QString &blockType, const QString &property);

	/// Helper method, constructs filter for graphical model recreation. For each graphical block in model
	/// \a constructor will be called with the id of that block. If constructor returns non-null type id
	/// current block in model will be replaced with the new instance of returned type.
	/// Links will be automaticly reconnected to a new nodes even if recreated themselves.
	/// @param replacer A function that would tell if we need to replace the given block in graphical model.
	/// If null id is returned the block stays without modification. Otherwise returned type id will be used
	/// to replace current block with the returned one.
	/// @param constructor A fucntion that will initialize new element right after its creation.
	/// The id of the new block, old block and graphical model will be passed there in this exact order.
	static GraphicalFilter graphicalRecreate(const GraphicalReplacer &replacer
			, const GraphicalConstructor &constructor);
};

}
