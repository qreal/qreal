#include "generator.h"
#include "../../../qrutils/nameNormalizer.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace qUbiq::qUbiqGenerator;
using namespace qReal;
using namespace utils;

QString const templateDir = "./templates";
QString const templateFileNameForms = "qUbiqForms.cs";
QString const templateFileNameVariables = "qUbiqVariables.cs";
QString const templateFileNameCSProj = "qUbiqCSProject.csproj";
QString const templateFileNameAppConfig = "qUbiqApp.config";

/// Generation target file

Generator::Generator(QString const &outputDirPath
		, QString const &programName
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
	: AbstractGenerator(templateDir, QString(outputDirPath + "/" + programName).replace("\\", "/"), logicalModel, errorReporter)
	, mProgramName(programName)
{
}

Generator::~Generator()
{
}

bool Generator::isCorrectedName(QString const &name)
{
	QRegExp patten;
	patten.setPattern("[A-Za-z]+([A-Za-z_0-9]*)");
	return patten.exactMatch(name);
}

void Generator::initGeneratingFiles()
{
	loadTemplateFromFile(templateFileNameForms, mResultForms);
	loadTemplateFromFile(templateFileNameVariables, mResultVariables);
}

void Generator::saveGeneratedFiles()
{
	QString formsFileName = QString(mProgramName + "Forms.cs");
	QString variablesFileName = QString(mProgramName + "Variables.cs");

	saveOutputFile(formsFileName, mResultForms);
	saveOutputFile(variablesFileName, mResultVariables);

	mCompileIncludeFiles.append(formsFileName);
	mCompileIncludeFiles.append(variablesFileName);
}

void Generator::generate()
{
	mErrorReporter.clear();
	mErrorReporter.clearErrors();

	if (!isCorrectedName(mProgramName)) {
		mErrorReporter.addError("Program name is not correct");
		return;
	}

	initGeneratingFiles();
	generateVariables();
	generatePresentationDiagrams();
	generateLogicDiagrams();
	saveGeneratedFiles();
	generateAndSaveCSProject();
	saveAppConfig();
}

void Generator::generatePresentationDiagrams()
{
	QString startFormName = "";
	QString formsDescription = "";
	QString onButtonClickedDescriptions = "";

	foreach (Id const &diagram, mApi.elementsByType("qUbiqPresentationDiagram")) {
		if (!mApi.isLogicalElement(diagram) || mApi.parent(diagram) != Id::rootId()) {
			continue;
		}
		NeededStringsForPresentationDiagram currentForms = generateMainForms(diagram);
		startFormName = currentForms.startFormName;
		formsDescription += currentForms.formsDescription;
		onButtonClickedDescriptions += currentForms.onButtonClickedDescriptions;
	}

	mResultForms.replace("@@programName@@", mProgramName);
	mResultForms.replace("@@startFormName@@", startFormName);
	mResultForms.replace("@@createFormDescriptions@@", formsDescription);
	mResultForms.replace("@@onButtonClickedDescriptions@@", onButtonClickedDescriptions);
}

void Generator::generateLogicDiagrams()
{
	QList<Id> logicElementList= mApi.elementsByType("qUbiqLogicDiagram");
	logicElementList.append(mApi.elementsByType("qUbiqConditionDiagram"));

	QString handlerDescriptions = "";
	foreach (Id const &diagram, logicElementList) {
		if (!mApi.isLogicalElement(diagram)) {
			continue;
		}
		handlerDescriptions += generateHandlers(diagram);
	}
	mResultForms.replace("@@handlerDescriptions@@", handlerDescriptions);
}

void Generator::generateVariables()
{
	QString variablesDeclaration = "";

	foreach (Id const &variable, mApi.elementsByType("variable")) {
		if (!mApi.isLogicalElement(variable)) {
			continue;
		}
		QString currentVariable = mTemplateUtils["@@oneVariableDeclaration@@"];
		QString variableType = mApi.property(variable, "type").toString();

		QString realVariableType = "";
		if (variableType == "text") {
			realVariableType = "string";
		} else if (variableType == "list") {
			realVariableType = "List<VisualElement>";
		} else if (variableType == "image") {
			realVariableType = "Image";
		} else if (variableType == "grid") {
			realVariableType = "Grid";
		} else if (variableType == "number") {
			realVariableType = "int";
		}

		currentVariable.replace("@@variableName@@", mApi.name(variable));
		currentVariable.replace("@@variableType@@", realVariableType);
		variablesDeclaration += currentVariable;
	}

	mResultVariables.replace("@@programName@@", mProgramName);
	mResultVariables.replace("@@variableDeclarations@@", variablesDeclaration);
}

Generator::NeededStringsForPresentationDiagram Generator::generateMainForms(Id const &diagram)
{
	NeededStringsForPresentationDiagram result = NeededStringsForPresentationDiagram("", "", "");
	result.startFormName = countStartFormName(diagram);

	foreach (Id const &form, mApi.elementsByType("slide")) {
		if (!mApi.isLogicalElement(form) || mApi.parent(form) != diagram) {
			continue;
		}
		NeededStringsForOneSlideDescription oneSlideWithButton = countFormWithButtonDescription(form);
		result.formsDescription += oneSlideWithButton.oneFormDescription;
		result.onButtonClickedDescriptions += oneSlideWithButton.onButtonDescription;
	}

	return result;
}

QString Generator::countStartFormName(Id const &diagram)
{
	QString startForm = "";
	bool existsBeginForm = false;
	foreach (Id const &begin, mApi.elementsByType("beginNode")) {
		if (existsBeginForm || !mApi.isLogicalElement(begin) || mApi.parent(begin) != diagram) {
			continue;
		}

		foreach (Id const &beginNode, mApi.outgoingNodes(begin)) {
			if (existsBeginForm || !mApi.isLogicalElement(beginNode)) {
				continue;
			}
			startForm = mApi.name(beginNode);
			existsBeginForm = true;
		}
	}
	return startForm;
}

Generator::NeededStringsForOneSlideDescription Generator::countFormWithButtonDescription(Id const &form)
{
	NeededStringsForOneSlideDescription result;

	QList<Id> buttonList = mApi.elementsByType("button");
	foreach (Id const &button, buttonList) {
		if (!mApi.isLogicalElement(button) || mApi.parent(button) != form) {
			continue;
		}
		result.onButtonDescription += countOnButtonDescription(button);
	}

	QStringList slideSize = mApi.property(form, "slideSize").toString().split('x');
	QString oneFormDescription = mTemplateUtils["@@oneCreateFormDescription@@"];
	oneFormDescription.replace("@@formName@@", mApi.name(form));
	oneFormDescription.replace("@@width@@", slideSize.at(1));
	oneFormDescription.replace("@@height@@", slideSize.at(0));
	oneFormDescription.replace("@@mainGridFilling@@", countMainGridFilling(form));

	result.oneFormDescription = oneFormDescription;
	return result;
}

QString Generator::countMainGridFilling(Id const &form)
{
	QString mainGridFilling = "";
	QList<Id> elementList= mApi.elementsByType("list");
	elementList.append(mApi.elementsByType("text"));
	elementList.append(mApi.elementsByType("image"));
	elementList.append(mApi.elementsByType("grid"));
	elementList.append(mApi.elementsByType("button"));
	int i = 0;
	foreach (Id const &element, elementList) {
		if (!mApi.isLogicalElement(element) || mApi.parent(element) != form) {
			continue;
		}

		QString elementCreation = "";
		QString elementName = "";
		if ((element.element().compare("text", Qt::CaseInsensitive)) == 0) {
			elementCreation = countTextDeclaration(element);
			elementName = "text";
		} else if ((element.element().compare("image", Qt::CaseInsensitive)) == 0) {
			elementCreation = countImageDeclaration(element);
			elementName = "imageBlock";
		} else if ((element.element().compare("list", Qt::CaseInsensitive)) == 0) {
			elementCreation = countListDeclaration(element);
			elementName = "list";
		} else if ((element.element().compare("grid", Qt::CaseInsensitive)) == 0) {
			elementCreation = countGridDeclaration(element);
			elementName = "grid";
		} else { // i.e. 'button' and 'exitButton'
			elementCreation = countButtonDeclaration(element);
			elementName = "button";
		}

		QString currentElement = mTemplateUtils["@@elementDeclaration@@"];
		currentElement.replace("@@elemetCreation@@", elementCreation);
		currentElement.replace("@@elementName@@", elementName);
		currentElement.replace("@@tail@@", QString::number(i));
		QStringList position = mApi.property(element, "position").toString().split(':');
		currentElement.replace("@@x@@", position.at(1));
		currentElement.replace("@@y@@", position.at(0));

		mainGridFilling += currentElement;
		i++;
	}
	return mainGridFilling;
}

QString Generator::countButtonDeclaration(Id const &button)
{
	QString result = mTemplateUtils["@@buttonDeclaration@@"];
	result.replace("@@buttonName@@", mApi.name(button));
	return result;
}

QString Generator::countButtonDeclarationWithHandler(Id const &button)
{
	QString result = mTemplateUtils["@@buttonDeclarationWithHandler@@"];
	result.replace("@@handlerName@@", mApi.property(button, "handler").toString());
	return result;
}

QString Generator::countTextDeclaration(Id const &element)
{
	QString result;
	QString text = mApi.property(element, "text").toString();
	if (text.contains('%')) {
		QStringList textList = text.split('%');
		for (int i = 1; i < textList.size(); i += 2) {
			QString variableName = textList.at(i);
			QString newText = "\" + " + mProgramName + "Variables." + variableName + ".ToString() + \" ";
			text.replace("%" + variableName + "%", newText);
		}
	}

	result = mTemplateUtils["@@textDeclaration@@"];
	result.replace("@@text@@", text);
	return result;
}

QString Generator::countImageDeclaration(Id const &element)
{
	QString result = mTemplateUtils["@@imageDeclaration@@"];
	result.replace("@@pathToImage@@", mApi.property(element, "pathToImage").toString());
	return result;
}

Generator::NeededStringsForOneElementDeclaration Generator::countOneElementDeclarationForLists(Id const &element, int tail)
{
	NeededStringsForOneElementDeclaration result;
	QString elementCreation = "";
	QString elementName = "";
	if ((element.element().compare("text", Qt::CaseInsensitive)) == 0) {
		elementCreation = countTextDeclaration(element);
		elementName = "text";
	} else if ((element.element().compare("image", Qt::CaseInsensitive)) == 0) {
		elementCreation = countImageDeclaration(element);
		elementName = "imageBlock";
	} else if ((element.element().compare("button", Qt::CaseInsensitive)) == 0){
		elementCreation = countButtonDeclarationWithHandler(element);
		elementName = "button";
	} else {
		mErrorReporter.addError("Invalid item in the List or Grid", element);
	}
	elementCreation.replace("@@tail@@", "@@tail@@_" + QString::number(tail));
	result.elemetCreation = elementCreation;
	result.elementName = elementName;
	return result;
}

QString Generator::countListDeclaration(Id const &element)
{
	QString result = mTemplateUtils["@@listDeclaration@@"];

	QString listFillings;
	int elementsCount = mApi.property(element, "elementsCount").toInt();
	IdList children = mApi.children(element);

	if (children.size() > elementsCount) {
		mErrorReporter.addError("Count of List elements is less than count of List children", element);
	} else {
		int i = 0;
		while (i < elementsCount) {
			foreach (Id const &child, children) {
				if (i >= elementsCount) {
					break;
				}
				NeededStringsForOneElementDeclaration currentElement = countOneElementDeclarationForLists(child, i);
				QString currentListFilling = mTemplateUtils["@@listFilling@@"];
				currentListFilling.replace("@@listItemCreation@@", currentElement.elemetCreation);
				currentListFilling.replace("@@listItemName@@", currentElement.elementName);
				currentListFilling.replace("@@itemTail@@", QString::number(i));
				listFillings += currentListFilling;
				i++;
			}
		}
	}

	result.replace("@@listFillings@@", listFillings);
	return result;
}

QString Generator::countGridDeclaration(Id const &element)
{
	QString result = mTemplateUtils["@@gridDeclaration@@"];
	IdList children = mApi.children(element);

	if (children.size() != 1) {
		mErrorReporter.addError("Incorrect number of Grid children", element);
	} else {
		Id child = children.at(0);
		NeededStringsForOneElementDeclaration currentElement = countOneElementDeclarationForLists(child, 0);
		result.replace("@@gridItemCreation@@", currentElement.elemetCreation);
		result.replace("@@gridItemName@@", currentElement.elementName);
		result.replace("@@itemTail@@", QString::number(0));
	}

	QStringList gridSize = mApi.property(element, "size").toString().split('x');
	result.replace("@@w@@", gridSize.at(1));
	result.replace("@@h@@", gridSize.at(0));
	return result;
}

QString Generator::countOnButtonDescription(Id const &button)
{
	QString result;

	if ((button.element().compare("ExitButton", Qt::CaseInsensitive)) == 0) {
		result = mTemplateUtils["@@oneOnExitButtonClickedDescription@@"];
	} else {
		QString toFormName = "";
		bool existsToForm = false;
		foreach (Id const &toNode, mApi.outgoingNodes(button)) {
			if (existsToForm || !mApi.isLogicalElement(toNode)) {
				continue;
			}
			toFormName = mApi.name(toNode);
			existsToForm = true;
		}

		result = mTemplateUtils["@@oneOnButtonClickedDescription@@"];
		result.replace("@@toFormName@@", toFormName);
	}

	result.replace("@@buttonName@@", mApi.name(button));
	return result;
}

QString Generator::generateHandlers(Id const &diagram)
{
	//qwerty_TODO_for future
	QString result = "";
	if ((diagram.element().compare("qUbiqLogicDiagram", Qt::CaseInsensitive)) == 0) {
		result = mTemplateUtils["@@oneHandlerDescription@@"];
		result.replace("@@handlerName@@", mApi.name(diagram));
	}
	return result;
}

void Generator::generateAndSaveCSProject()
{
	QString resultCSProject;
	loadTemplateFromFile(templateFileNameCSProj, resultCSProject);

	QString compileIncludes = "";
	foreach (QString fileName, mCompileIncludeFiles)
	{
		QString oneCompileIncludeTemplate = mTemplateUtils["@@oneCompileInclude@@"];
		oneCompileIncludeTemplate.replace("@@fileName@@", fileName);
		compileIncludes += oneCompileIncludeTemplate;
	}

	resultCSProject.replace("@@programName@@", mProgramName);
	resultCSProject.replace("@@compileIncludes@@", compileIncludes);

	saveOutputFile(QString(mProgramName + ".csproj"), resultCSProject);
}

void Generator::saveAppConfig()
{
	QString resultAppConfig;
	loadTemplateFromFile(templateFileNameAppConfig, resultAppConfig);
	saveOutputFile("App.config", resultAppConfig);
}

