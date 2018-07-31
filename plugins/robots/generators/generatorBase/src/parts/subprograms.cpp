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

#include <QtXml/QDomDocument>

#include <qrtext/lua/luaToolbox.h>

#include "generatorBase/parts/subprograms.h"
#include "src/structuralControlFlowGenerator.h"

using namespace generatorBase::parts;
using namespace qReal;

Subprograms::Subprograms(const qrRepo::RepoApi &repo
		, ErrorReporterInterface &errorReporter
		, const QStringList &pathsToTemplates
		, qrtext::LanguageToolboxInterface &luaToolbox
		, const simple::Binding::ConverterInterface *nameNormalizer
		, const simple::Binding::ConverterInterface *typeConverter
		)
	: TemplateParametrizedEntity(pathsToTemplates)
	, mRepo(repo)
	, mErrorReporter(errorReporter)
	, mLuaToolbox(luaToolbox)
	, mNameNormalizer(nameNormalizer)
	, mTypeConverter(typeConverter)
{
}

Subprograms::~Subprograms()
{
	delete mNameNormalizer;
	delete mTypeConverter;
}

QString Subprograms::forwardDeclarations() const
{
	return mForwardDeclarationsCode.join('\n');
}

QString Subprograms::implementations() const
{
	return mImplementationsCode.join('\n');
}

void Subprograms::appendManualSubprogram(const QString &name, const QString &body)
{
	mManualDeclarations[name] = body;
}

void Subprograms::usageFound(const Id &logicalId)
{
	const Id diagram = mRepo.outgoingExplosion(logicalId);
	if (diagram != Id() && !mDiscoveredSubprograms.contains(diagram)) {
		mDiscoveredSubprograms[diagram] = false;
		mDiscoveredSubprogramsOrder.push_back(diagram);
	}
}

Subprograms::GenerationResult Subprograms::generate(ControlFlowGeneratorBase *mainGenerator
		, const QString &indentString)
{
	QMap<Id, QString> declarations;
	QMap<Id, QString> implementations;

	Id toGen = firstToGenerate();
	while (toGen != Id()) {
		mDiscoveredSubprograms[toGen] = true;

		const Id graphicalDiagramId = graphicalId(toGen);
		if (graphicalDiagramId.isNull()) {
			mErrorReporter.addError(QObject::tr("Graphical diagram instance not found"));
			return GenerationResult::fatalError;
		}

		const QString rawIdentifier = mRepo.name(toGen);
		const QString identifier = mNameNormalizer->convert(rawIdentifier);
		if (!checkIdentifier(identifier, rawIdentifier)) {
			return GenerationResult::fatalError;
		}

		const QString currentDynamicLabelsString = mRepo.stringProperty(toGen, "labels");
		if (!currentDynamicLabelsString.isEmpty()) {
			QDomDocument currentDynamicLabels;
			currentDynamicLabels.setContent(currentDynamicLabelsString);

			for (QDomElement element
					= currentDynamicLabels.firstChildElement("labels").firstChildElement("label")
					; !element.isNull()
					; element = element.nextSiblingElement("label"))
			{
				mLuaToolbox.interpret(QString("%1=%2")
						.arg(mNameNormalizer->convert(element.attribute("text"))).arg(element.attribute("value")));
			}
		}

		ControlFlowGeneratorBase *generator = mainGenerator->cloneFor(graphicalDiagramId, true);
		auto readableGenerator = dynamic_cast<StructuralControlFlowGenerator *>(generator);
		semantics::SemanticTree *controlFlow = generator->generate(Id(), "@@unknown@@");
		if (!controlFlow || (readableGenerator && readableGenerator->cantBeGeneratedIntoStructuredCode())) {
			return GenerationResult::error;
		}

		implementations[toGen] = controlFlow->toString(1, indentString);

		const QString forwardDeclaration = readSubprogramSignature(toGen, "subprograms/forwardDeclaration.t");
		declarations[toGen] = forwardDeclaration;

		toGen = firstToGenerate();
	}

	obtainCode(declarations, implementations);

	return GenerationResult::success;
}

void Subprograms::obtainCode(QMap<Id, QString> const &declarations
		, QMap<Id, QString> const &implementations)
{
	if (!declarations.keys().isEmpty()) {
		mForwardDeclarationsCode << readTemplate("subprograms/declarationsSectionHeader.t");
	}

	for (const Id &id : declarations.keys()) {
		mForwardDeclarationsCode << declarations[id];
	}

	if (!implementations.keys().isEmpty()) {
		mImplementationsCode << readTemplate("subprograms/implementationsSectionHeader.t");
	}

	for (const Id &id : implementations.keys()) {
		const QString signature = readSubprogramSignature(id, "subprograms/implementation.t");
		QString subprogramCode = signature;
		subprogramCode.replace("@@BODY@@", implementations[id]);
		mImplementationsCode << subprogramCode;
	}

	mImplementationsCode.insert(0, generateManualDeclarations() + "\n\n");
}

QString Subprograms::generateManualDeclarations() const
{
	return QStringList(mManualDeclarations.values()).join("\n\n");
}

QString Subprograms::readSubprogramSignature(const Id &id, const QString &pathToTemplate)
{
	const QString rawName = mRepo.name(id);
	QString signatureWithName = readTemplate(pathToTemplate).replace("@@NAME@@", mNameNormalizer->convert(rawName));

	QList<QString> argumentsList;
	const QString argumentFormat = readTemplate("subprograms/subprogramArgument.t");
	const QString currentDynamicLabelsString = mRepo.stringProperty(id, "labels");
	if (!currentDynamicLabelsString.isEmpty()) {
		QDomDocument currentDynamicLabels;
		currentDynamicLabels.setContent(currentDynamicLabelsString);

		for (QDomElement element
				= currentDynamicLabels.firstChildElement("labels").firstChildElement("label")
				; !element.isNull()
				; element = element.nextSiblingElement("label"))
		{
			const QString type = element.attribute("type");
			QString argument = readTemplateIfExists(QString("subprograms/%1SubprogramArgument.t").arg(type)
					, argumentFormat);
			argument.replace("@@TYPE@@", mTypeConverter->convert(type));
			argument.replace("@@NAME@@", mNameNormalizer->convert(element.attribute("text")));
			argumentsList << argument;
		}
	}

	const QString fallBackResult = "FileNotExist";
	QString argumentsSeparator = readTemplateIfExists("subprograms/declarationArgumentsSeparator.t", fallBackResult);
	argumentsSeparator = argumentsSeparator == fallBackResult
			? readTemplate("luaPrinting/argumentsSeparator.t")
			: argumentsSeparator;
	QString arguments = argumentsList.join(argumentsSeparator);
	return signatureWithName.replace("@@ARGUMENTS@@", arguments);
}

Id Subprograms::graphicalId(const Id &logicalId) const
{
	const IdList graphicalIds = mRepo.graphicalElements(logicalId.type());
	foreach (const Id &id, graphicalIds) {
		if (mRepo.logicalId(id) == logicalId) {
			return id;
		}
	}

	return Id();
}

bool Subprograms::checkIdentifier(const QString &identifier, const QString &rawName)
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
	while (!mDiscoveredSubprogramsOrder.isEmpty()) {
		const Id &id = mDiscoveredSubprogramsOrder.first();
		mDiscoveredSubprogramsOrder.pop_front();
		if (!mDiscoveredSubprograms[id]) {
			return id;
		}
	}

	return Id();
}
