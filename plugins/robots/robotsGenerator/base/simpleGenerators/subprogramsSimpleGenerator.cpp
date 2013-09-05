#include "subprogramsSimpleGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

SubprogramsSimpleGenerator::SubprogramsSimpleGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "subprograms/subprogramCall.t", QList<Binding *>()
			<< Binding::createConverting("@@NAME@@", "name", customizer.factory()->nameNormalizerConverter())
			, parent)
{
}

//QList<SmartLine_old> SubprogramsSimpleGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
//		, qReal::Id const &elementId, qReal::Id const &logicElementId)
//{
//	nxtGen->subprogramsGenerator().usageFound(logicElementId);
//	return QList<SmartLine_old>() << SmartLine_old(identifier(nxtGen, elementId) + "();", elementId);
//}

//QString SubprogramsSimpleGenerator::identifier(NxtOSEKRobotGenerator const *nxtGen, qReal::Id const &id)
//{
//	QString const rawName = nxtGen->api()->name(id);
//	return utils::NameNormalizer::normalizeStrongly(rawName, false);
//}
