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
	};
}

ProjectConverter SaveConvertionManager::before300Alpha1Converter()
{
	return ProjectConverter(editor(), Version(), Version::fromString("3.0.0-a1")
			, [=](GraphicalModelAssistInterface const &graphicalApi, LogicalModelAssistInterface &logicalApi)
	{
		Q_UNUSED(graphicalApi)
		Q_UNUSED(logicalApi)
		return ProjectConverter::VersionTooOld;
	});
}

ProjectConverter SaveConvertionManager::from300Alpha4to300Alpha5Converter()
{
	QMap<QString, QString> replacementRules;
	replacementRules.insert("JA1", "A1");
	replacementRules.insert("JA2", "A2");
	replacementRules.insert("JA3", "A3");
	replacementRules.insert("JA4", "A4");
	replacementRules.insert("JA5", "A5");
	replacementRules.insert("JA6", "A6");

	replacementRules.insert("JD1", "D1");
	replacementRules.insert("JD2", "D2");

	replacementRules.insert("JF1", "F1");

	replacementRules.insert("JM1", "M1");
	replacementRules.insert("JM2", "M2");
	replacementRules.insert("JM3", "M3");
	replacementRules.insert("JM4", "M4");

	replacementRules.insert("JB1", "B1");
	replacementRules.insert("JB2", "B2");
	replacementRules.insert("JB3", "B3");
	replacementRules.insert("JB4", "B4");

	replacementRules.insert("JE1", "E1");
	replacementRules.insert("JE2", "E2");
	replacementRules.insert("JE3", "E3");
	replacementRules.insert("JE4", "E4");

	replacementRules.insert("JC1", "C1");
	replacementRules.insert("JC2", "C2");
	replacementRules.insert("JC3", "C3");

	replacementRules.insert("sensor1", "sensorA1");
	replacementRules.insert("sensor2", "sensorA2");
	replacementRules.insert("sensor3", "sensorA3");
	replacementRules.insert("sensor4", "sensorA4");
	replacementRules.insert("sensor5", "sensorA5");
	replacementRules.insert("sensor6", "sensorA6");

	replacementRules.insert("digitSensor1", "sensorD1");
	replacementRules.insert("digitSensor2", "sensorD2");


	return ProjectConverter(editor(), Version::fromString("3.0.0-a4"), Version::fromString("3.0.0-a5")
			, [=](GraphicalModelAssistInterface const &graphicalApi, LogicalModelAssistInterface &logicalApi)
	{
		Q_UNUSED(graphicalApi)
		bool modificationsMade = false;

		for (Id const &graphicalBlock : elementsOfRobotsDiagrams(logicalApi)) {
			Id const block = graphicalApi.logicalId(graphicalBlock);
			if (!block.element().startsWith("Trik")) {
				continue;
			}

			QMapIterator<QString, QVariant> iterator = logicalApi.logicalRepoApi().propertiesIterator(block);
			for (iterator.next(); iterator.hasNext(); iterator.next()) {
				QString const name = iterator.key();
				QString value = iterator.value().toString();
				bool replacementOccured = false;
				for (QString const &toReplace : replacementRules.keys()) {
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
		}

		return modificationsMade ? ProjectConverter::Success : ProjectConverter::NoModificationsMade;
	});
}

ProjectConverter SaveConvertionManager::from300Beta2to300rc1Converter()
{
	return ProjectConverter(editor(), Version::fromString("3.0.0-b2"), Version::fromString("3.0.0-rc1")
			, [=](GraphicalModelAssistInterface const &graphicalApi, LogicalModelAssistInterface &logicalApi)
	{
		Q_UNUSED(graphicalApi)
		bool modificationsMade = false;

		for (Id const &graphicalBlock : elementsOfRobotsDiagrams(logicalApi)) {
			Id const block = graphicalApi.logicalId(graphicalBlock);
			if (block.element() == "TrikSay" || block.element() == "PrintText") {
				QString const propertyName = block.element() == "TrikSay" ? "Text" : "PrintText";
				QString const oldValue = logicalApi.logicalRepoApi().property(block, propertyName).toString();
				if (!oldValue.startsWith("\"")) {
					modificationsMade = true;
					logicalApi.mutableLogicalRepoApi().setProperty(block, propertyName, "\"" + oldValue + "\"");
				}
			}
		}

		return modificationsMade ? ProjectConverter::Success : ProjectConverter::NoModificationsMade;
	});
}

bool SaveConvertionManager::isRobotsDiagram(Id const &diagram)
{
	QStringList const robotsDiagrams = { "RobotsDiagram", "SubprogramDiagram" };
	return diagram.editor() == editor() && robotsDiagrams.contains(diagram.diagram());
}

IdList SaveConvertionManager::elementsOfRobotsDiagrams(LogicalModelAssistInterface const &logicalApi)
{
	IdList result;
	for (Id const &diagramId : logicalApi.children(Id::rootId())) {
		if (isRobotsDiagram(diagramId)) {
			result += logicalApi.children(diagramId);
		}
	}

	return result;
}
