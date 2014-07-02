#include "generatorBase/parts/subprograms.h"

#include "generatorBase/controlFlowGeneratorBase.h"

using namespace generatorBase::parts;
using namespace qReal;

Subprograms::Subprograms(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter
		, QString const &pathToTemplates
		, simple::Binding::ConverterInterface const *nameNormalizer)
	: TemplateParametrizedEntity(pathToTemplates)
	, mRepo(repo)
	, mErrorReporter(errorReporter)
	, mNameNormalizer(nameNormalizer)
{
}

Subprograms::~Subprograms()
{
	delete mNameNormalizer;
}

QString Subprograms::generatedCode() const
{
	return mGeneratedCode.join('\n');
}

void Subprograms::appendManualSubprogram(QString const &name, QString const &body)
{
	mManualDeclarations[name] = body;
}

void Subprograms::usageFound(Id const &logicalId)
{
	Id const diagram = mRepo.outgoingExplosion(logicalId);
	if (diagram != Id() && !mDiscoveredSubprograms.contains(diagram)) {
		mDiscoveredSubprograms[diagram] = false;
	}
}

bool Subprograms::generate(ControlFlowGeneratorBase *mainGenerator)
{
	QMap<Id, QString> declarations;
	QMap<Id, QString> implementations;

	Id toGen = firstToGenerate();
	while (toGen != Id()) {
		mDiscoveredSubprograms[toGen] = true;

		Id const graphicalDiagramId = graphicalId(toGen);
		if (graphicalDiagramId.isNull()) {
			mErrorReporter.addError(QObject::tr("Graphical diagram instance not found"));
			return false;
		}

		QString const rawIdentifier = mRepo.name(toGen);
		QString const identifier = mNameNormalizer->convert(rawIdentifier);
		if (!checkIdentifier(identifier, rawIdentifier)) {
			return false;
		}

		ControlFlowGeneratorBase *generator = mainGenerator->cloneFor(graphicalDiagramId);
		semantics::SemanticTree *controlFlow = generator->generate();
		if (!controlFlow) {
			return false;
		}

		implementations[toGen] = controlFlow->toString(1);

		QString const forwardDeclaration = readSubprogramTemplate(toGen, "subprograms/forwardDeclaration.t");
		declarations[toGen] = forwardDeclaration;

		toGen = firstToGenerate();
	}

	mergeCode(declarations, implementations);

	return true;
}

void Subprograms::mergeCode(QMap<Id, QString> const &declarations
		, QMap<Id, QString> const &implementations)
{
	if (!declarations.keys().isEmpty()) {
		mGeneratedCode << readTemplate("subprograms/declarationsSectionHeader.t");
	}

	foreach (Id const &id, declarations.keys()) {
		mGeneratedCode << declarations[id];
	}

	mGeneratedCode << QString();

	if (!implementations.keys().isEmpty()) {
		mGeneratedCode << readTemplate("subprograms/implementationsSectionHeader.t");
	}

	foreach (Id const &id, implementations.keys()) {
		QString const signature = readSubprogramTemplate(id, "subprograms/implementation.t");
		QString subprogramCode = signature;
		subprogramCode.replace("@@BODY@@", implementations[id]);
		mGeneratedCode << subprogramCode;
	}

	mGeneratedCode.insert(0, generateManualDeclarations() + "\n\n");
}

QString Subprograms::generateManualDeclarations() const
{
	return QStringList(mManualDeclarations.values()).join("\n\n");
}

QString Subprograms::readSubprogramTemplate(Id const &id, QString const &pathToTemplate)
{
	QString const rawName = mRepo.name(id);
	return readTemplate(pathToTemplate).replace("@@NAME@@", mNameNormalizer->convert(rawName));
}

Id Subprograms::graphicalId(Id const &logicalId) const
{
	IdList const graphicalIds = mRepo.graphicalElements(logicalId.type());
	foreach (Id const &id, graphicalIds) {
		if (mRepo.logicalId(id) == logicalId) {
			return id;
		}
	}

	return Id();
}

bool Subprograms::checkIdentifier(QString const &identifier, QString const &rawName)
{
	if (identifier.isEmpty()) {
		mErrorReporter.addError(QObject::tr(
				"Please enter valid c-style name for subprogram \"") + rawName + "\"");
		return false;
	}

	if (mUsedNames.contains(identifier)) {
		mErrorReporter.addError(QObject::tr("Duplicate identifier: ") + identifier);
		return false;
	}

	mUsedNames << identifier;

	return true;
}

Id Subprograms::firstToGenerate() const
{
	foreach (Id const &id, mDiscoveredSubprograms.keys()) {
		if (!mDiscoveredSubprograms[id]) {
			return id;
		}
	}

	return Id();
}
