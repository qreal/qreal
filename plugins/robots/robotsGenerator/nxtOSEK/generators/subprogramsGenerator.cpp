#include "subprogramsGenerator.h"
#include "../nxtOSEKRobotGenerator.h"

using namespace qReal;
using namespace robots::generator;

SubprogramsGenerator::SubprogramsGenerator(NxtOSEKRobotGenerator * const nxtGenerator)
	: mNxtGen(nxtGenerator)
{
}

void SubprogramsGenerator::usageFound(Id const &logicalId)
{
	Id const diagram = mNxtGen->api()->outgoingExplosion(logicalId);
	if (diagram != Id() && !mDiscoveredSubprograms.contains(diagram)) {
		mDiscoveredSubprograms[diagram] = false;
	}
}

QList<SmartLine> SubprogramsGenerator::generateCode()
{
	Id toGen = firstToGenerate();
	while (toGen != Id()) {
		mDiscoveredSubprograms[toGen] = true;

		toGen = firstToGenerate();
	}
}

Id SubprogramsGenerator::firstToGenerate() const
{
	foreach (Id const &id, mDiscoveredSubprograms.keys()) {
		if (!mDiscoveredSubprograms[id]) {
			return id;
		}
	}

	return Id();
}
