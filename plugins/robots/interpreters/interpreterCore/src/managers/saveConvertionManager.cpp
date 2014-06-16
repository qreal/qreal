#include "saveConvertionManager.h"

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
	return ProjectConverter(editor(), Version(), Version::fromString("3.0.0-a1")
			, [=](GraphicalModelAssistInterface const &graphicalApi, LogicalModelAssistInterface &logicalApi)
	{
		Q_UNUSED(graphicalApi)
		for (Id const &block : elementsOfRobotsDiagrams(logicalApi)) {
			QMapIterator<QString, QVariant> iterator = logicalApi.logicalRepoApi().propertiesIterator(block);
			for (; iterator.hasNext(); iterator.next()) {
				QString const name = iterator.key();
				QString value = iterator.value().toString();
				value
						.replace("JA1", "A1")
						.replace("JA2", "A2")
						.replace("JA3", "A3")
						.replace("JA4", "A4")
						.replace("JA5", "A5")
						.replace("JA6", "A6")

						.replace("JD1", "D1")
						.replace("JD2", "D2")

						.replace("JF1", "F1")

						.replace("JM1", "M1")
						.replace("JM2", "M2")
						.replace("JM3", "M3")
						.replace("JM4", "M4")

						.replace("JB1", "B1")
						.replace("JB2", "B2")
						.replace("JB3", "B3")
						.replace("JB4", "B4")

						.replace("JE1", "E1")
						.replace("JE2", "E2")
						.replace("JE3", "E3")
						.replace("JE4", "E4")
						.replace("JC1", "C1")
						.replace("JC2", "C2")
						.replace("JC3", "C3")

						.replace("sensor1", "sensorA1")
						.replace("sensor2", "sensorA2")
						.replace("sensor3", "sensorA3")
						.replace("sensor4", "sensorA4")
						.replace("sensor5", "sensorA5")
						.replace("sensor6", "sensorA6")

						.replace("digitSensor1", "sensorD1")
						.replace("digitSensor2", "sensorD2");

				logicalApi.setPropertyByRoleName(block, name, value);
			}
		}

		return ProjectConverter::Success;
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
