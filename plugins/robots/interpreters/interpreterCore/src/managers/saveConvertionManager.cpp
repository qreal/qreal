#include "interpreterCore/managers/saveConvertionManager.h"

using namespace interpreterCore;
using namespace qReal;

QString SaveConvertionManager::editor()
{
	return "RobotsMetamodel";
}

QList<ProjectConverter> SaveConvertionManager::converters()
{
	return { before300Alpha1Converter()
		, from300Alpha4to300Alpha5Converter()
		, from300Beta2to300rc1Converter()
		, from300to301Converter()
	};
}

ProjectConverter SaveConvertionManager::before300Alpha1Converter()
{
	return ProjectConverter(editor(), Version(), Version::fromString("3.0.0-a1")
			, [=](const GraphicalModelAssistInterface &graphicalApi, LogicalModelAssistInterface &logicalApi)
	{
		Q_UNUSED(graphicalApi)
		Q_UNUSED(logicalApi)
		return ProjectConverter::VersionTooOld;
	});
}

ProjectConverter SaveConvertionManager::from300Alpha4to300Alpha5Converter()
{
	return constructConverter("3.0.0-a4", "3.0.0-a5"
			, {
				replace({
						{"JA1", "A1"}
						, {"JA2", "A2"}
						, {"JA3", "A3"}
						, {"JA4", "A4"}
						, {"JA5", "A5"}
						, {"JA6", "A6"}

						, {"JD1", "D1"}
						, {"JD2", "D2"}

						, {"JF1", "F1"}

						, {"JM1", "M1"}
						, {"JM2", "M2"}
						, {"JM3", "M3"}
						, {"JM4", "M4"}

						, {"JB1", "B1"}
						, {"JB2", "B2"}
						, {"JB3", "B3"}
						, {"JB4", "B4"}

						, {"JE1", "E1"}
						, {"JE2", "E2"}
						, {"JE3", "E3"}
						, {"JE4", "E4"}

						, {"JC1", "C1"}
						, {"JC2", "C2"}
						, {"JC3", "C3"}

						, {"sensor1", "sensorA1"}
						, {"sensor2", "sensorA2"}
						, {"sensor3", "sensorA3"}
						, {"sensor4", "sensorA4"}
						, {"sensor5", "sensorA5"}
						, {"sensor6", "sensorA6"}

						, {"digitSensor1", "sensorD1"}
						, {"digitSensor2", "sensorD2"}
				})
			}
			, [] (const Id &block) { return block.element().startsWith("Trik"); }
			);

}

ProjectConverter SaveConvertionManager::from300Beta2to300rc1Converter()
{
	return constructConverter("3.0.0-b2", "3.0.0-rc1"
			, { quote("TrikSay", "Text"), quote("PrintText", "PrintText") }
			);
}

qReal::ProjectConverter SaveConvertionManager::from300to301Converter()
{
	return constructConverter("3.0.0", "3.0.1"
			, {
				replace({
						{"enterButton", "buttonEnter"}
						, {"escapeButton", "buttonEscape"}
						, {"leftButton", "buttonLeft"}
						, {"rightButton", "buttonRight"}
						, {"backButton", "buttonBack"}
						, {"downButton", "buttonDown"}
						, {"enterButton", "buttonEnter"}
						, {"upButton", "buttonUp"}
						, {"powerButton", "buttonEsc"}
				  })
				, [=] (const Id &block, LogicalModelAssistInterface &logicalApi) {
						if (block.element().startsWith("Trik")) {
							return replace({{"buttonEscape", "buttonEsc"}})(block, logicalApi);
						}

						return false;
					}
				, deleteBlocks({"Ev3WaitForUp"
						, "Ev3WaitForEnter"
						, "Ev3WaitForDown"
						, "Ev3WaitForRight"
						, "Ev3WaitForLeft"
						, "Ev3WaitForBack"

						, "NxtWaitForEnter"
						, "NxtWaitForEscape"
						, "NxtWaitForLeft"
						, "NxtWaitForRight"

						, "TrikWaitForEnter"
						, "TrikWaitForLeft"
						, "TrikWaitForRight"
						, "TrikWaitForDown"
						, "TrikWaitForUp"
						, "TrikWaitForPower"
						})
			}
			);
}

qReal::ProjectConverter SaveConvertionManager::from301to302Converter()
{
	return constructConverter("3.0.1", "3.0.2", { quote("TrikSystem", "Command") } );
}

bool SaveConvertionManager::isRobotsDiagram(const Id &diagram)
{
	const QStringList robotsDiagrams = { "RobotsDiagram", "SubprogramDiagram" };
	return diagram.editor() == editor() && robotsDiagrams.contains(diagram.diagram());
}

IdList SaveConvertionManager::elementsOfRobotsDiagrams(const LogicalModelAssistInterface &logicalApi)
{
	IdList result;
	for (const Id &diagramId : logicalApi.children(Id::rootId())) {
		if (isRobotsDiagram(diagramId)) {
			result += logicalApi.children(diagramId);
		}
	}

	return result;
}

qReal::ProjectConverter SaveConvertionManager::constructConverter(const QString &oldVersion
		, const QString &newVersion
		, const QList<std::function<bool(const Id &, LogicalModelAssistInterface &)>> &filters
		, const std::function<bool(const qReal::Id &)> &condition
		)
{
	return ProjectConverter(editor(), Version::fromString(oldVersion), Version::fromString(newVersion)
			, [=](const GraphicalModelAssistInterface &graphicalApi, LogicalModelAssistInterface &logicalApi)
	{
		Q_UNUSED(graphicalApi);

		bool modificationsMade = false;

		for (const Id &graphicalBlock : elementsOfRobotsDiagrams(logicalApi)) {
			const Id block = graphicalApi.logicalId(graphicalBlock);

			if (!condition(block)) {
				continue;
			}

			for (const auto &filter : filters) {
				modificationsMade |= filter(block, logicalApi);
			}
		}

		return modificationsMade ? ProjectConverter::Success : ProjectConverter::NoModificationsMade;
	});
}

std::function<bool(const qReal::Id &, qReal::LogicalModelAssistInterface &)> SaveConvertionManager::replace(
		const QMap<QString, QString> &replacementRules)
{
	return [=] (const Id &block, LogicalModelAssistInterface &logicalApi) {
		bool modificationsMade = false;
		QMapIterator<QString, QVariant> iterator = logicalApi.logicalRepoApi().propertiesIterator(block);
		for (iterator.next(); iterator.hasNext(); iterator.next()) {
			const QString name = iterator.key();
			QString value = iterator.value().toString();
			bool replacementOccured = false;
			for (const QString &toReplace : replacementRules.keys()) {
				if (value.contains(toReplace)) {
					replacementOccured = true;
					modificationsMade = true;

					value.replace(toReplace, replacementRules[toReplace]);
				}
			}

			if (replacementOccured) {
				logicalApi.setPropertyByRoleName(block, value, name);
			}
		}

		 return modificationsMade;
	};
}

std::function<bool(const qReal::Id &, qReal::LogicalModelAssistInterface &)>
		SaveConvertionManager::deleteBlocks(const QStringList &blocks)
{
	return [=] (const Id &block, LogicalModelAssistInterface &logicalApi) {
		if (blocks.contains(block.element())) {
			logicalApi.removeElement(block);
			return true;
		}

		return false;
	};
}

std::function<bool(const qReal::Id &, qReal::LogicalModelAssistInterface &)> SaveConvertionManager::quote(
		const QString &blockType, const QString &property)
{
	return [blockType, property] (const qReal::Id &block, qReal::LogicalModelAssistInterface &logicalApi) {
		if (block.element() == blockType) {
			const QString oldValue = logicalApi.logicalRepoApi().property(block, property).toString();
			if (!oldValue.startsWith("\"")) {
				logicalApi.mutableLogicalRepoApi().setProperty(block, property, "\"" + oldValue + "\"");
				return true;
			}
		}

		return false;
	};
}
