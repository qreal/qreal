#include "structuralGenerator.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace generators::generator;
using namespace qReal;
using namespace utils;

/// Generation target file
QString const fileNamePRO = "generator.pro";
QString const fileNameGeneratorH = "generator.h";
QString const fileNameGeneratorCPP = "generator.cpp";
QString const fileNamePluginH = "generatorPlugin.h";
QString const fileNamePluginCPP = "generatorPlugin.cpp";

StructuralGenerator::StructuralGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, QString const &pathToQReal
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		, QString const &metamodelLanguageName
		, QString const &languageName
		, QString const &nodeName
		, QString const &toGeneratePropertyName
		, QString const &programNamePropertyName
		, QString const &generatorMetamodelName
	)
	: MyAbstractGenerator(templateDirPath, outputDirPath, pathToQReal
						  , logicalModel, errorReporter
						  , metamodelLanguageName, languageName, nodeName
						  , toGeneratePropertyName, programNamePropertyName
						  , generatorMetamodelName)
{
}

StructuralGenerator::~StructuralGenerator()
{
}

QString StructuralGenerator::generateForeachNode(Id const &element, QString const& count, QString const& parentCount)
{
	QString result = mTemplateUtils["@@beforeAllForeachGenerateBody@@"];
	QString resultMarks;

	int curCount = 1;
	IdList list = mApi.children(element);
	foreach (Id const &child, list) {
		if (!mApi.isLogicalElement(child) || child.element() != "SemanticNode") {
			continue;
		}
		QString textCode = mApi.property(child, "textCode").toString();
		resultMarks += textCode + "\n";
		result += generateSemanticNode(child, "oneForeachGenerateBody", count + "_" + QString("%1").arg(curCount), parentCount) + "\n";
		curCount ++;
	}

	result.replace("@@foreachMarkName@@", mApi.property(element, "markName").toString());
	generateTemplateUsingTextCode(element, resultMarks);

	return result;
}

QString StructuralGenerator::generateConverterNode(Id const &element, QString const& count)
{
	Q_UNUSED(element)
	Q_UNUSED(count)
	return "";
}

QStringList StructuralGenerator::marksList(QString const& text, QString const& mark)
{
	QStringList res;
	QRegExp pattern(QString(mark + "[A-Za-z]+([A-Za-z0-9_.]*)" + mark));

	int pos = 0;
	while ((pos = pattern.indexIn(text, pos)) != -1) {
		QString curRes = pattern.cap(0);
		if (!res.contains(curRes)) {
			res << curRes;
		}
		pos += pattern.matchedLength();
	 }

	return res;
}

QString StructuralGenerator::generateReplaceTemplateBodyForMark(QString const& textCode, QString const& elementMarkName, QString const& count)
{
	QString result = mTemplateUtils["@@oneReplaceTemplateGenerateBody@@"];
	QString replaceTemplates = "";

	QStringList list = marksList(textCode, "@@");
	foreach (QString var, list) {
		replaceTemplates += QString(mTemplateUtils["@@oneSimpleReplaceTemplate@@"])
				.replace("@@templateMarkName@@", var);
	}

	QStringList propertyList = marksList(textCode, "##");
	foreach (QString var, propertyList) {
		QString curReplaceTemplates = mTemplateUtils["@@oneWithPropertyReplaceTemplate@@"];
		replaceTemplates += doReplaceForPropertyMarks(curReplaceTemplates, var);
	}

	result.replace("@@replaceTemplates@@", replaceTemplates);
	result.replace("@@elementMarkName@@", elementMarkName);
	result.replace("@@count@@", count);

	return result;
}

QString StructuralGenerator::doReplaceForPropertyMarks(QString const& text, QString const& propertyMark)
{
	QString result = text;
	QString templatePropertyName = "";
	QString realElementForProperty = "";
	QString realPropertyName = propertyMark.split("##").at(1);

	QStringList list = realPropertyName.split(".");
	if (list.count() > 1) {// т.е. на самом деле метка вида "some1.some2"
		QString node = list.at(0);
		templatePropertyName = list.at(1);

		if (node == "dst") {
			realElementForProperty = "outElement";
		} else if (node == "src") {
			realElementForProperty = "element_@@count@@";
		}
	} else { // т.е. это обычная метка
		templatePropertyName = realPropertyName;
		realElementForProperty = "element_@@count@@";
	}

	result.replace("@@templateMarkName@@", propertyMark)
		.replace("@@templatePropertyName@@", templatePropertyName)
		.replace("@@realElementForProperty@@", realElementForProperty);
	return result;
}

QString StructuralGenerator::generateReplaceTemplateBodyForDiagramMark(QString const& textCode, QString const& elementMarkName)
{
	QString result = mTemplateUtils["@@onefillMarkProperty@@"];
	QString replaceTemplates = "";

	QStringList list = marksList(textCode, "@@"); // таких меток для данного "особого" шаблона быть не должно => ошибка
	if (!list.isEmpty()) {
		mErrorReporter.addError(QObject::tr("Root Template is not correct ('@@' is forbidden for textCode)"));
	}

	QStringList propertyList = marksList(textCode, "##");
	foreach (QString var, propertyList) {
		QString curReplaceTemplates = mTemplateUtils["@@oneWithPropertyReplaceTemplateForDiagram@@"];
		replaceTemplates += doReplaceForPropertyMarks(curReplaceTemplates, var);
	}

	result.replace("@@replaceTemplates@@", replaceTemplates);
	result.replace("@@realTemplateMarkName@@", elementMarkName);
	result.replace("@@textCode@@", QString(textCode).replace("\\n", " "));

	return result;
}

QString StructuralGenerator::generateReplaceTemplateBodyForFile(QString const& textCode, QString const& templateVariableName, QString const& templateFileName)
{
	QString result = mTemplateUtils["@@oneReplaceTemplateGenerateBodyForFile@@"];
	QString replaceTemplates = "";

	QStringList list = marksList(textCode, "@@");
	foreach (QString var, list) {
		replaceTemplates += QString(mTemplateUtils["@@oneSimpleReplaceTemplateForFile@@"])
				.replace("@@templateMarkName@@", var);
	}

	QStringList propertyList = marksList(textCode, "##");
	foreach (QString var, propertyList) {
		QString curReplaceTemplates = mTemplateUtils["@@oneWithPropertyReplaceTemplateForFile@@"];
		replaceTemplates += doReplaceForPropertyMarks(curReplaceTemplates, var);
	}

	result.replace("@@replaceTemplates@@", replaceTemplates);
	result.replace("@@templateVariableName@@", templateVariableName);
	result.replace("@@templateFileName@@", templateFileName);

	return result;
}

QPair<QString, StructuralGenerator::TemplateType> StructuralGenerator::generateReplaceTemplateBodyForTemplateNode(Id const &templateElement, QString const& count)
{
	QString markName = mApi.property(templateElement, "markName").toString();
	QString fileName = mApi.property(templateElement, "fileName").toString();
	QString textCode = mApi.property(templateElement, "textCode").toString();
	QPair<QString, StructuralGenerator::TemplateType> result;

	if (!fileName.isEmpty()) {
		saveOutputFile(QString(fileName), textCode, mTemplateDirName);
		QString variableName = QString(fileName).replace(".", "_");
		mTemplateVariableFilename[variableName] = fileName;
		result.first = generateReplaceTemplateBodyForFile(textCode, variableName, fileName);
		result.second = TemplateType::file;
	} else if (!markName.isEmpty()) {
		mMarksCode[markName] = textCode;

		if (!count.isEmpty()) {
			result.first = generateReplaceTemplateBodyForMark(textCode, markName, count);
			result.second = TemplateType::mark;
		} else { // особый случай
			result.first = generateReplaceTemplateBodyForDiagramMark(textCode, markName);
			result.second = TemplateType::diagramMark;
		}
	}

	return result;
}

QPair<QString, QString> StructuralGenerator::addTemplateString(QPair<QString, QString> sum, QPair<QString, StructuralGenerator::TemplateType> str)
{
	if (str.second == TemplateType::diagramMark) {
		sum.second += str.first + "\n";
	} else {
		sum.first += str.first + "\n";
	}
	return sum;
}

QPair<QString, QString> StructuralGenerator::generateReplaceTemplateBody(Id const &element, QString const& count)
{
	QString elementName = mApi.property(element, "elementName").toString();
	QString textCode = mApi.property(element, "textCode").toString();

	QPair<QString, QString>  result;

	if (!textCode.isEmpty()) {
		QString elementMarkName = QString(mTemplateUtils["@@realElementMarkName@@"])
				.replace("@@elementName@@", elementName)
				.replace("@@count@@", count);
		elementMarkName.chop(1);
		mMarksCode[elementMarkName] = textCode;

		if (!count.isEmpty()) {
			result.first += generateReplaceTemplateBodyForMark(textCode, elementMarkName, count) + "\n";
		} else { // особый случай (но тут на самом-то деле этого быть не может)
			result.second += generateReplaceTemplateBodyForDiagramMark(textCode, elementMarkName) + "\n";
		}
	}

	IdList list = mApi.children(element);
	foreach (Id const &child, list) {
		if (!mApi.isLogicalElement(child) || child.element() != "TemplateNode") {
			continue;
		}

		result = addTemplateString(result, generateReplaceTemplateBodyForTemplateNode(child, count));
	}

	return result;
}

QString StructuralGenerator::generateSemanticNode(Id const &element, QString const& templateName, QString const& count, QString const& parentCount)
{
	QString elementName = mApi.property(element, "elementName").toString();
	QString genBody = mTemplateUtils["@@" + templateName + "@@"];

	QString foreachBody = "";
	QString converterBody = "";

	int curCount = 1;
	IdList list = mApi.outgoingNodes(element); // qwerty_ToDo_need only "SimpleEdge"
	foreach (Id const &outElement, list) {
		if (!mApi.isLogicalElement(outElement)) {
			continue;
		}

		QString strCurCount = count + "_" + QString("%1").arg(curCount);
		if (outElement.element() == "ForeachNode") {
			QString oneForeachBody = generateForeachNode(outElement, strCurCount, count);
			foreachBody += oneForeachBody + "\n";
		} else if (outElement.element() == "ConverterNode") {
			QString oneConverterBody = generateConverterNode(outElement, strCurCount);
			converterBody += oneConverterBody  + "\n";
		} else {
			continue;
		}
		curCount ++;
	}

	genBody.replace("@@foreachGenerateBody@@", foreachBody);
	genBody.replace("@@converterGenerateBody@@", converterBody);
	genBody.replace("@@replaceTemplateGenerateBody@@", generateReplaceTemplateBody(element, count).first);

	genBody.replace("@@count@@", count);
	genBody.replace("@@elementName@@", elementName);
	if (!parentCount.isEmpty()) {
		genBody.replace("@@parentCount@@", parentCount);
	}

	return genBody;
}

QString StructuralGenerator::generateSemanticNodes()
{
	QString result = "";
	IdList list = mApi.elementsByType("SemanticNode");

	int curCount = 1;
	foreach (Id const &element, list) {
		if (!mApi.isLogicalElement(element) || mApi.parent(element).element() != "GeneratorDiagram") {
			continue;
		}
		result += generateSemanticNode(element, "oneSemanticGenerateBody", QString("%1").arg(curCount)) + "\n";
		curCount ++;
	}
	return result;
}

QString StructuralGenerator::makeStringFromList(QStringList const& list)
{
	QString result = "";
	foreach (QString str, list) {
		result += str;
	}
	return result;
}

QString StructuralGenerator::generateOneDstBody(Id const &element, QString const& count, QString const& dst)
{
	QString result = mTemplateUtils["@@oneDstGenerateBody@@"];
	if (dst == "ANY") {
		result = mTemplateUtils["@@oneDstGenerateBodyForANY@@"];
	}

	result.replace("@@replaceTemplateGenerateBodyForEdge@@", generateReplaceTemplateBody(element, count).first);
	result.replace("@@dstElementName@@", dst);
	return result;
}

QString StructuralGenerator::generateSemanticNodeForEdge(Id const &element, QString const& count)
{
	QString elementName = mApi.property(element, "elementName").toString();
	QString dstTypes = mApi.property(element, "dstTypes").toString();
	QString srcTypes = mApi.property(element, "srcTypes").toString();

	QStringList dstTypeList = dstTypes.split(",");
	QStringList srcTypeList = srcTypes.split(",");

	QString dstElementName = makeStringFromList(dstTypeList);
	QString srcElementName = makeStringFromList(srcTypeList);

	QString srcList = QString(mTemplateUtils["@@oneSrcList@@"]).replace("@@oneSrcElementName@@", srcTypeList.at(0));
	if (srcTypeList.count() > 1) {
		for (int i = 1; i < srcTypeList.count(); i++) {
			QString oneSrcList = QString(mTemplateUtils["@@oneSrcList@@"]).replace("@@oneSrcElementName@@", srcTypeList.at(i));
			oneSrcList.chop(1);
			srcList += " << " + oneSrcList;
		}
	}

	QString dstGenerateBody = "";
	foreach (QString dst, dstTypeList) {
		dstGenerateBody += generateOneDstBody(element, count, dst);
	}

	QString genBody = mTemplateUtils["@@oneSemanticEdgeGenerateBody@@"];
	genBody.replace("@@dstGenerateBody@@", dstGenerateBody);
	genBody.replace("@@count@@", count);
	genBody.replace("@@fullDstElementName@@", dstElementName);
	genBody.replace("@@fullSrcElementName@@", srcElementName);
	genBody.replace("@@srcList@@", srcList);
	genBody.replace("@@elementName@@", elementName);

	return genBody;
}

QString StructuralGenerator::generateSemanticNodesForEdge()
{
	QString result = "";
	IdList list = mApi.elementsByType("SemanticForEdge");

	int curCount = 1;
	foreach (Id const &element, list) {
		if (!mApi.isLogicalElement(element) || mApi.parent(element).element() != "GeneratorDiagram") {
			continue;
		}
		result += generateSemanticNodeForEdge(element, QString("%1").arg(curCount)) + "\n";
		curCount ++;
	}
	result.replace("@@tailName@@", "_Links");
	return result;
}

void StructuralGenerator::generate()
{
	QString generateBodyReplaceTemplates = "";
	QString fillMarksProperty = "";
	foreach (Id const &element, mApi.elementsByType("TemplateNode")) {
		if (!mApi.isLogicalElement(element)
				|| (mApi.parent(element).element() != "GeneratorDiagram"
					&& mApi.parent(element).element() != "ApplicationNode")) {
			continue;
		}
		QPair<QString, TemplateType> res = generateReplaceTemplateBodyForTemplateNode(element, "");
		if (res.second == TemplateType::diagramMark) {
			fillMarksProperty += res.first;
		} else {
			generateBodyReplaceTemplates += res.first;
		}
	}

	QString generateBodyForLinks = generateSemanticNodesForEdge();
	QString generateBody = generateSemanticNodes();

	saveTemplateUtils();

	QString resultPRO;
	QString resultPluginH;
	QString resultPluginCPP;
	QString resultGeneratorH;
	QString resultGeneratorCPP;

	QString fileBaseName = normalizerGeneratorModelName();
	QString generatorClassName = normalizerGeneratorClassModelName();
	QString pathToQReal = mPathToQReal;

	loadTemplateFromFile(fileNamePRO, resultPRO);
	loadTemplateFromFile(fileNamePluginH, resultPluginH);
	loadTemplateFromFile(fileNamePluginCPP, resultPluginCPP);
	loadTemplateFromFile(fileNameGeneratorH, resultGeneratorH);
	loadTemplateFromFile(fileNameGeneratorCPP, resultGeneratorCPP);

	resultPRO.replace("@@strangePathToQReal@@", pathToQReal.replace("/", "\\\\"));

	resultPRO.replace("@@pathToQReal@@", mPathToQReal);
	resultPluginH.replace("@@pathToQReal@@", mPathToQReal);
	resultPluginCPP.replace("@@pathToQReal@@", mPathToQReal);
	resultGeneratorH.replace("@@pathToQReal@@", mPathToQReal);
	resultGeneratorCPP.replace("@@pathToQReal@@", mPathToQReal);

	resultPRO.replace("@@generatorName@@", fileBaseName);
	resultPluginH.replace("@@generatorName@@", fileBaseName);
	resultPluginCPP.replace("@@generatorName@@", fileBaseName);
	resultGeneratorH.replace("@@generatorName@@", fileBaseName);
	resultGeneratorCPP.replace("@@generatorName@@", fileBaseName);

	resultPluginH.replace("@@generatorClassName@@", generatorClassName);
	resultPluginCPP.replace("@@generatorClassName@@", generatorClassName);

	resultGeneratorCPP.replace("@@templateConstStringNames@@", generateTemplateConstStringNames());
	resultGeneratorCPP.replace("@@generateBodyForLinks@@", generateBodyForLinks);
	resultGeneratorCPP.replace("@@generateBody@@", generateBody);
	resultGeneratorCPP.replace("@@generateBody_replaceTemplates@@", generateBodyReplaceTemplates);
	resultGeneratorCPP.replace("@@programNameProperty@@", mProgramNamePropertyName);
	resultGeneratorCPP.replace("@@tailName@@", "");

	resultPluginCPP.replace("@@languageNodeDiagram@@", mNodeName);
	resultPluginCPP.replace("@@programNameProperty@@", mProgramNamePropertyName);
	resultPluginCPP.replace("@@pathToGenerateProperty@@", mToGeneratePropertyName);
	resultPluginCPP.replace("@@fillMarksProperty@@", fillMarksProperty);
	resultPluginCPP.replace("@@tailName@@", "");

	saveOutputFile(QString(fileBaseName + ".pro"), resultPRO);
	saveOutputFile(QString(fileBaseName + "Plugin.h"), resultPluginH);
	saveOutputFile(QString(fileBaseName + "Plugin.cpp"), resultPluginCPP);
	saveOutputFile(QString(fileNameGeneratorH), resultGeneratorH);
	saveOutputFile(QString(fileNameGeneratorCPP), resultGeneratorCPP);
}

