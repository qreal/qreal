#include "subprogramsGenerator.h"

#include "../nxtOSEKRobotGenerator.h"
#include "controlFlowGenerator.h"
#include "simpleElements/subprogramsSimpleGenerator.h"

using namespace qReal;
using namespace robots::generator;

SubprogramsGenerator::SubprogramsGenerator(NxtOSEKRobotGenerator * const nxtGenerator)
	: mMainGenerator(nxtGenerator)
{
}

QList<SmartLine> &SubprogramsGenerator::generatedCode()
{
	return mGeneratedCode;
}

void SubprogramsGenerator::usageFound(Id const &logicalId)
{
	Id const diagram = mMainGenerator->api()->outgoingExplosion(logicalId);
	if (diagram != Id() && !mDiscoveredSubprograms.contains(diagram)) {
		mDiscoveredSubprograms[diagram] = false;
	}
}

bool SubprogramsGenerator::generate()
{
	QMap<Id, QList<SmartLine> > declarations;
	QMap<Id, QList<SmartLine> > implementations;

	Id toGen = firstToGenerate();
	while (toGen != Id()) {
		mDiscoveredSubprograms[toGen] = true;

		Id const graphicalDiagramId = graphicalId(toGen);
		if (graphicalDiagramId == Id()) {
			mMainGenerator->errorReporter().addError(QObject::tr("Graphical diagram instance not found"));
			return false;
		}

		QString const identifier = SubprogramsSimpleGenerator::identifier(mMainGenerator, toGen);
		if (!checkIdentifier(identifier, mMainGenerator->api()->name(toGen))) {
			return false;
		}

		ControlFlowGenerator generator(mMainGenerator, graphicalDiagramId);
		mMainGenerator->beforeSubprogramGeneration(&generator);
		if (!generator.generate()) {
			return false;
		}
		implementations[toGen] = generator.generatedCode();

		QString const forwardDeclaration = QString("void %1();").arg(identifier);
		declarations[toGen] = QList<SmartLine>() << SmartLine(forwardDeclaration, toGen);

		toGen = firstToGenerate();
	}

	mergeCode(declarations, implementations);

	return true;
}

void SubprogramsGenerator::mergeCode(QMap<Id, QList<SmartLine> > const &declarations
		, QMap<Id, QList<SmartLine> > const &implementations)
{
	if (!declarations.keys().isEmpty()) {
		mGeneratedCode << SmartLine("/* Subprograms declarations */", Id())
				<< SmartLine("", Id());
	}
	foreach (Id const &id, declarations.keys()) {
		mGeneratedCode += declarations[id];
	}

	mGeneratedCode << SmartLine("", Id());

	if (!implementations.keys().isEmpty()) {
		mGeneratedCode << SmartLine("/* Subprograms implementations */", Id())
				 << SmartLine("", Id());
	}
	foreach (Id const &id, implementations.keys()) {
		QString const signature = QString("void %1()").arg(
				SubprogramsSimpleGenerator::identifier(mMainGenerator, id));
		mGeneratedCode << SmartLine(signature, id) << SmartLine("{", id, SmartLine::increase);
		mGeneratedCode += implementations[id];
		mGeneratedCode << SmartLine("}", id, SmartLine::decrease)
				<< SmartLine("", id);
	}
}

Id SubprogramsGenerator::graphicalId(Id const &logicalId) const
{
	IdList const graphicalIds = mMainGenerator->api()->graphicalElements(logicalId.type());
	foreach (Id const &id, graphicalIds) {
		if (mMainGenerator->api()->logicalId(id) == logicalId) {
			return id;
		}
	}

	return Id();
}

bool SubprogramsGenerator::checkIdentifier(QString const &identifier, QString const &rawName)
{
	if (identifier.isEmpty()) {
		mMainGenerator->errorReporter().addError(
				QObject::tr("Please enter valid c-style name for subprogram \"") + rawName + "\"");
		return false;
	}

	if (mUsedNames.contains(identifier)) {
		mMainGenerator->errorReporter().addError(QObject::tr("Duplicate identifier: ") + identifier);
		return false;
	}

	mUsedNames << identifier;

	return true;
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
