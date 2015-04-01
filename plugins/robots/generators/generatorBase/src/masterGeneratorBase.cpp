#include <QtCore/QPair>
#include <QtCore/QStack>

#include "generatorBase/masterGeneratorBase.h"

#include <qrutils/outFile.h>
#include <qrutils/stringUtils.h>
#include <qrtext/languageToolboxInterface.h>

#include "readableControlFlowGenerator.h"
#include "gotoControlFlowGenerator.h"
#include "generatorBase/lua/luaProcessor.h"
#include "generatorBase/parts/variables.h"
#include "generatorBase/parts/subprograms.h"
#include "generatorBase/parts/threads.h"
#include "generatorBase/parts/sensors.h"
#include "generatorBase/parts/initTerminateCodeGenerator.h"

using namespace generatorBase;
using namespace qReal;

MasterGeneratorBase::MasterGeneratorBase(const qrRepo::RepoApi &repo
		, ErrorReporterInterface &errorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, const utils::ParserErrorReporter &parserErrorReporter
		, const Id &diagramId)
	: mRepo(repo)
	, mErrorReporter(errorReporter)
	, mRobotModelManager(robotModelManager)
	, mTextLanguage(textLanguage)
	, mDiagram(diagramId)
	, mParserErrorReporter(parserErrorReporter)
{
}

void MasterGeneratorBase::setProjectDir(const QFileInfo &fileInfo)
{
	mProjectName = fileInfo.baseName();
	mProjectDir = fileInfo.absolutePath();
}

void MasterGeneratorBase::initialize()
{
	mCustomizer = createCustomizer();
	mCustomizer->factory()->initialize();
	setPathToTemplates(mCustomizer->factory()->pathToTemplates());

	mValidator = createValidator();

	mReadableControlFlowGenerator = new ReadableControlFlowGenerator(mRepo
			, mErrorReporter, *mCustomizer, *mValidator, mDiagram, this);
	mGotoControlFlowGenerator = new GotoControlFlowGenerator(mRepo
			, mErrorReporter, *mCustomizer, *mValidator, mDiagram, this);
}

QString MasterGeneratorBase::generate(const QString &indentString)
{
	if (mDiagram.isNull()) {
		mErrorReporter.addCritical(QObject::tr("There is no opened diagram"));
		return QString();
	}

	beforeGeneration();
	if (!QDir(mProjectDir).exists()) {
		QDir().mkpath(mProjectDir);
	}

	mTextLanguage.clear();
	mCustomizer->factory()->setMainDiagramId(mDiagram);

	for (parts::InitTerminateCodeGenerator *generator : mCustomizer->factory()->initTerminateGenerators()) {
		generator->reinit();
	}

	QString mainCode;
	const semantics::SemanticTree *mainControlFlow = mReadableControlFlowGenerator->generate();
	if (mainControlFlow && !mReadableControlFlowGenerator->cantBeGeneratedIntoStructuredCode()) {
		mainCode = mainControlFlow->toString(1, indentString);
		const bool subprogramsResult = mCustomizer->factory()->subprograms()->generate(mReadableControlFlowGenerator
				, indentString);
		if (!subprogramsResult) {
			if (supportsGotoGeneration()) {
				mainCode = QString();
			} else {
				return QString();
			}
		}
	} else {
		if (mReadableControlFlowGenerator->errorsOccured()) {
			return QString();
		}
	}

	if (mainCode.isEmpty() && supportsGotoGeneration()) {
		mErrorReporter.addInformation(tr("This diagram cannot be generated into the structured code."\
				" Generating it into the code with 'goto' statements."));
		const semantics::SemanticTree *gotoMainControlFlow = mGotoControlFlowGenerator->generate();
		if (gotoMainControlFlow) {
			mainCode = gotoMainControlFlow->toString(1, indentString);
			const bool gotoSubprogramsResult = mCustomizer->factory()
					->subprograms()->generate(mGotoControlFlowGenerator, indentString);
			if (!gotoSubprogramsResult) {
				mainCode = QString();
			}
		}
	}

	if (mainCode.isEmpty()) {
		const QString errorMessage = supportsGotoGeneration()
				? tr("This diagram cannot be even generated into the code with 'goto'"\
						"statements. Please contact the developers (WTF did you do?)")
				: tr("This diagram cannot be generated into the structured code.");
		mErrorReporter.addError(errorMessage);
		return QString();
	}

	QString resultCode = readTemplate("main.t");
	resultCode.replace("@@SUBPROGRAMS_FORWARDING@@", mCustomizer->factory()->subprograms()->forwardDeclarations());
	resultCode.replace("@@SUBPROGRAMS@@", mCustomizer->factory()->subprograms()->implementations());
	resultCode.replace("@@THREADS_FORWARDING@@", mCustomizer->factory()->threads().generateDeclarations());
	resultCode.replace("@@THREADS@@", mCustomizer->factory()->threads().generateImplementations(indentString));
	resultCode.replace("@@MAIN_CODE@@", mainCode);
	resultCode.replace("@@INITHOOKS@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->initCode(), 1, indentString));
	resultCode.replace("@@TERMINATEHOOKS@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->terminateCode(), 1, indentString));
	resultCode.replace("@@USERISRHOOKS@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->isrHooksCode(), 1, indentString));
	resultCode.replace("@@VARIABLES@@", mCustomizer->factory()->variables()->generateVariableString());
	// This will remove too many empty lines
	resultCode.replace(QRegExp("\n(\n)+"), "\n\n");

	resultCode = generateLinkingInfo(resultCode);

	processGeneratedCode(resultCode);

	const QString pathToOutput = targetPath();
	outputCode(pathToOutput, resultCode);

	afterGeneration();

	return pathToOutput;
}

QString MasterGeneratorBase::generateLinkingInfo(QString &resultCode)
{
	QString const open = "@~(qrm:(/\\w+)+/\\{(\\w+-)+\\w+\\})~@";
	QString const close = "@#%1#@";
	QRegExp re;
	QStack< QPair<QString, int> > stack;
	QList< QPair<QString, QPair<int, int>> > results;
	int lineNumber = 1;

	for (QString const &line : resultCode.split("\n")){
		re.setPattern(open);
		int const pos = re.indexIn(line);
		if (pos > -1) {
			QString const id = re.cap(1);
			stack.push(QPair<QString, int>(id, lineNumber));
		}

		if (!stack.isEmpty()) {
			QString const id = stack.top().first;
			if (line.contains(close.arg(id))) {
				results.append(QPair<QString, QPair<int, int>>(id
						, QPair<int, int>(stack.top().second, lineNumber)));
				stack.pop();
			}
		}

		lineNumber++;
	}

	QString out;

	for (QPair<QString, QPair<int, int>> res : results) {
		out += res.first + "@" + QString::number(res.second.first) + "@" + QString::number(res.second.second)
				+ "\n";
	}

	outputCode(targetPath() + ".dbg", out);

	return resultCode.remove(QRegExp("@(~|#)qrm:(((/\\w+)+/\\{(\\w+-)+\\w+\\})|(/))(~|#)@"));
}

lua::LuaProcessor *MasterGeneratorBase::createLuaProcessor()
{
	return new lua::LuaProcessor(mErrorReporter, mTextLanguage, mParserErrorReporter, this);
}

PrimaryControlFlowValidator *MasterGeneratorBase::createValidator()
{
	return new PrimaryControlFlowValidator(mRepo, mErrorReporter, *mCustomizer, this);
}

void MasterGeneratorBase::beforeGeneration()
{
}

void MasterGeneratorBase::processGeneratedCode(QString &generatedCode)
{
	Q_UNUSED(generatedCode)
}

void MasterGeneratorBase::afterGeneration()
{
}

void MasterGeneratorBase::outputCode(const QString &path, const QString &code)
{
	// File must be removed to leave created and modified timestamps equal.
	QFile::remove(path);
	utils::OutFile out(path);
	out() << code;
}
