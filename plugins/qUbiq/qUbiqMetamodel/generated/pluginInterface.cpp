#include "pluginInterface.h"

#include "elements.h"


QUbiqMetamodelPlugin::QUbiqMetamodelPlugin()
{
	initPlugin();
}

void QUbiqMetamodelPlugin::initPlugin()
{
	initNameMap();
	initMouseGestureMap();
	initPropertyMap();
	initPropertyDefaultsMap();
	initDescriptionMap();
	initParentsMap();
	initPaletteGroupsMap();
	initPaletteGroupsDescriptionMap();
	initShallPaletteBeSortedMap();
	initExplosionsMap();
}

void QUbiqMetamodelPlugin::initNameMap()
{
	mDiagramNameMap["QUbiqConditionEditor"] = QString::fromUtf8("QUbiq Condition Editor");
	mDiagramNodeNameMap["QUbiqConditionEditor"] = "qUbiqConditionDiagram";

	mDiagramNameMap["QUbiqLogicEditor"] = QString::fromUtf8("QUbiq Logic Editor");
	mDiagramNodeNameMap["QUbiqLogicEditor"] = "qUbiqLogicDiagram";

	mDiagramNameMap["QUbiqPresentationEditor"] = QString::fromUtf8("QUbiq Presentation Editor");
	mDiagramNodeNameMap["QUbiqPresentationEditor"] = "qUbiqPresentationDiagram";

	mElementsNameMap["QUbiqConditionEditor"]["BeginNode"] = QString::fromUtf8("Begin node");
	mElementsNameMap["QUbiqConditionEditor"]["Change"] = QString::fromUtf8("Change element");
	mElementsNameMap["QUbiqConditionEditor"]["ConditionLink"] = QString::fromUtf8("Conditional Link");
	mElementsNameMap["QUbiqConditionEditor"]["CyclicLink"] = QString::fromUtf8("Cyclic link");
	mElementsNameMap["QUbiqConditionEditor"]["ForNode"] = QString::fromUtf8("For");
	mElementsNameMap["QUbiqConditionEditor"]["IfNode"] = QString::fromUtf8("IF");
	mElementsNameMap["QUbiqConditionEditor"]["Link"] = QString::fromUtf8("Link");
	mElementsNameMap["QUbiqConditionEditor"]["OutputSplitter"] = QString::fromUtf8("Output splitter");
	mElementsNameMap["QUbiqConditionEditor"]["QUbiqConditionDiagram"] = QString::fromUtf8("Condition Diagram");
	mElementsNameMap["QUbiqConditionEditor"]["Return"] = QString::fromUtf8("Return (bool value)");
	mElementsNameMap["QUbiqConditionEditor"]["TakeGridOrListElements"] = QString::fromUtf8("Take elements of grid or list");
	mElementsNameMap["QUbiqConditionEditor"]["Variable"] = QString::fromUtf8("Variable");
	mElementsNameMap["QUbiqConditionEditor"]["While"] = QString::fromUtf8("While");
	mElementsNameMap["QUbiqLogicEditor"]["BeginNodeInLogicEditor"] = QString::fromUtf8("Begin node");
	mElementsNameMap["QUbiqLogicEditor"]["ChangeInLogicEditor"] = QString::fromUtf8("Change element");
	mElementsNameMap["QUbiqLogicEditor"]["ConditionLinkInLogicEditor"] = QString::fromUtf8("Conditional Link");
	mElementsNameMap["QUbiqLogicEditor"]["CyclicLinkInLogicEditor"] = QString::fromUtf8("Cyclic Link");
	mElementsNameMap["QUbiqLogicEditor"]["ForNodeInLogicEditor"] = QString::fromUtf8("For");
	mElementsNameMap["QUbiqLogicEditor"]["Go"] = QString::fromUtf8("Go to slide");
	mElementsNameMap["QUbiqLogicEditor"]["IfNodeInLogicEditor"] = QString::fromUtf8("If");
	mElementsNameMap["QUbiqLogicEditor"]["LinkInLogicEditor"] = QString::fromUtf8("Link");
	mElementsNameMap["QUbiqLogicEditor"]["Message"] = QString::fromUtf8("Sent message");
	mElementsNameMap["QUbiqLogicEditor"]["OutputSplitterInLogicEditor"] = QString::fromUtf8("Output splitter");
	mElementsNameMap["QUbiqLogicEditor"]["QUbiqLogicDiagram"] = QString::fromUtf8("Logic Diagram");
	mElementsNameMap["QUbiqLogicEditor"]["ReturnInLogicEditor"] = QString::fromUtf8("Return (bool value)");
	mElementsNameMap["QUbiqLogicEditor"]["Take"] = QString::fromUtf8("Take");
	mElementsNameMap["QUbiqLogicEditor"]["TakeGridOrListElementsInLogicEditor"] = QString::fromUtf8("Take elements of grid or list");
	mElementsNameMap["QUbiqLogicEditor"]["VariableInLogicEditor"] = QString::fromUtf8("Variable");
	mElementsNameMap["QUbiqLogicEditor"]["WaitMessage"] = QString::fromUtf8("Wait message");
	mElementsNameMap["QUbiqLogicEditor"]["WhileInLogicEditor"] = QString::fromUtf8("While");
	mElementsNameMap["QUbiqPresentationEditor"]["BeginNodeInPresentationEditor"] = QString::fromUtf8("Begin node");
	mElementsNameMap["QUbiqPresentationEditor"]["Button"] = QString::fromUtf8("Button");
	mElementsNameMap["QUbiqPresentationEditor"]["ExitButton"] = QString::fromUtf8("Exit Button");
	mElementsNameMap["QUbiqPresentationEditor"]["Grid"] = QString::fromUtf8("Grid");
	mElementsNameMap["QUbiqPresentationEditor"]["Image"] = QString::fromUtf8("Image");
	mElementsNameMap["QUbiqPresentationEditor"]["List"] = QString::fromUtf8("List");
	mElementsNameMap["QUbiqPresentationEditor"]["QUbiqPresentationDiagram"] = QString::fromUtf8("PresentationDiagram");
	mElementsNameMap["QUbiqPresentationEditor"]["Slide"] = QString::fromUtf8("Slide");
	mElementsNameMap["QUbiqPresentationEditor"]["Text"] = QString::fromUtf8("Text");
	mElementsNameMap["QUbiqPresentationEditor"]["TransitionLink"] = QString::fromUtf8("Transition Link");
	mElementsNameMap["QUbiqPresentationEditor"]["VariableInPresentationEditor"] = QString::fromUtf8("Variable");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["Change"]["elementId"] = QString::fromUtf8("Element ID");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["Change"]["newValue"] = QString::fromUtf8("New property value");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["Change"]["propertyName"] = QString::fromUtf8("Name of property");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["ConditionLink"]["boolValueStereotype"] = QString::fromUtf8("Bool stereotype");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["CyclicLink"]["cyclicValueStereotype"] = QString::fromUtf8("Cyclic stereotype");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["ForNode"]["gridOrListId"] = QString::fromUtf8("ID of grid or list");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["ForNode"]["hCounterId"] = QString::fromUtf8("Counter ID by height");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["ForNode"]["wCounterId"] = QString::fromUtf8("Counter ID by width");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["IfNode"]["condition"] = QString::fromUtf8("Condition");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["QUbiqConditionDiagram"]["description"] = QString::fromUtf8("Description");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["Return"]["value"] = QString::fromUtf8("Return value");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["TakeGridOrListElements"]["condition"] = QString::fromUtf8("By condition");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["TakeGridOrListElements"]["gridOrListId"] = QString::fromUtf8("ID of grid or list");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["TakeGridOrListElements"]["variableId"] = QString::fromUtf8("Save in variable");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["Variable"]["currentValue"] = QString::fromUtf8("Current value");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["Variable"]["description"] = QString::fromUtf8("Description");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["Variable"]["type"] = QString::fromUtf8("Type of variable");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["While"]["condition"] = QString::fromUtf8("Condition");
	mPropertiesDisplayedNamesMap["QUbiqConditionEditor"]["While"]["counterId"] = QString::fromUtf8("Counter ID");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["ChangeInLogicEditor"]["elementId"] = QString::fromUtf8("Element ID");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["ChangeInLogicEditor"]["newValue"] = QString::fromUtf8("New property value");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["ChangeInLogicEditor"]["propertyName"] = QString::fromUtf8("Name of property");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["ConditionLinkInLogicEditor"]["boolValueStereotype"] = QString::fromUtf8("Bool stereotype");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["CyclicLinkInLogicEditor"]["cyclicValueStereotype"] = QString::fromUtf8("Cyclic stereotype");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["ForNodeInLogicEditor"]["gridOrListId"] = QString::fromUtf8("ID of grid or list");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["ForNodeInLogicEditor"]["hCounterId"] = QString::fromUtf8("Counter ID by height");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["ForNodeInLogicEditor"]["wCounterId"] = QString::fromUtf8("Counter ID by width");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["Go"]["goto"] = QString::fromUtf8("Gor to ID slide");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["IfNodeInLogicEditor"]["condition"] = QString::fromUtf8("Condition");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["Message"]["additionalText"] = QString::fromUtf8("Additional text");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["Message"]["messageType"] = QString::fromUtf8("Type of sent message");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["Message"]["sentTo"] = QString::fromUtf8("Reciever");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["QUbiqLogicDiagram"]["handlerType"] = QString::fromUtf8("Type of handler");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["ReturnInLogicEditor"]["value"] = QString::fromUtf8("Return value");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["Take"]["takeFrom"] = QString::fromUtf8("From element ID");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["Take"]["takeTo"] = QString::fromUtf8("To element ID");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["Take"]["takeType"] = QString::fromUtf8("Type of memory");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["TakeGridOrListElementsInLogicEditor"]["condition"] = QString::fromUtf8("By condition");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["TakeGridOrListElementsInLogicEditor"]["gridOrListId"] = QString::fromUtf8("ID of grid or list");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["TakeGridOrListElementsInLogicEditor"]["variableId"] = QString::fromUtf8("Save in variable");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["VariableInLogicEditor"]["currentValue"] = QString::fromUtf8("Current value");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["VariableInLogicEditor"]["description"] = QString::fromUtf8("Description");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["VariableInLogicEditor"]["type"] = QString::fromUtf8("Type of variable");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["WaitMessage"]["additionalText"] = QString::fromUtf8("Additional text");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["WaitMessage"]["waitFrom"] = QString::fromUtf8("Sender");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["WaitMessage"]["waitMessageType"] = QString::fromUtf8("Type of wait message");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["WhileInLogicEditor"]["condition"] = QString::fromUtf8("Condition");
	mPropertiesDisplayedNamesMap["QUbiqLogicEditor"]["WhileInLogicEditor"]["counterId"] = QString::fromUtf8("Counter ID");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Button"]["buttonText"] = QString::fromUtf8("Text on button");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Button"]["handler"] = QString::fromUtf8("Handler");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Button"]["x"] = QString::fromUtf8("X position on slide");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Button"]["y"] = QString::fromUtf8("Y position on slide");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["ExitButton"]["buttonText"] = QString::fromUtf8("Text on button");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["ExitButton"]["handler"] = QString::fromUtf8("Handler");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["ExitButton"]["x"] = QString::fromUtf8("X position on slide");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["ExitButton"]["y"] = QString::fromUtf8("Y position on slide");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Grid"]["elementId"] = QString::fromUtf8("Id of element");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Grid"]["h"] = QString::fromUtf8("Height dimension");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Grid"]["handler"] = QString::fromUtf8("Handler");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Grid"]["isNull"] = QString::fromUtf8("Is Null in begin");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Grid"]["w"] = QString::fromUtf8("Widht dimension");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Grid"]["x"] = QString::fromUtf8("X position on slide");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Grid"]["y"] = QString::fromUtf8("Y position on slide");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Image"]["isNull"] = QString::fromUtf8("Is Null in begin");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Image"]["pathToImage"] = QString::fromUtf8("Path to image");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Image"]["x"] = QString::fromUtf8("X position on slide");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Image"]["y"] = QString::fromUtf8("Y position on slide");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["List"]["elementId"] = QString::fromUtf8("Id of element");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["List"]["elementsCount"] = QString::fromUtf8("Count of elements");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["List"]["handler"] = QString::fromUtf8("Handler");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["List"]["isNull"] = QString::fromUtf8("Is Null in begin");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["List"]["x"] = QString::fromUtf8("X position on slide");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["List"]["y"] = QString::fromUtf8("Y position on slide");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["QUbiqPresentationDiagram"]["pathToGenerate"] = QString::fromUtf8("Path to generate");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["QUbiqPresentationDiagram"]["programName"] = QString::fromUtf8("Program name");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Slide"]["description"] = QString::fromUtf8("Description");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Slide"]["handler"] = QString::fromUtf8("Handler");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Slide"]["height"] = QString::fromUtf8("Height of slide");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Slide"]["width"] = QString::fromUtf8("Width of slide");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Text"]["isNull"] = QString::fromUtf8("Is Null in begin");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Text"]["text"] = QString::fromUtf8("Text");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Text"]["x"] = QString::fromUtf8("X position on slide");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["Text"]["y"] = QString::fromUtf8("Y position on slide");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["VariableInPresentationEditor"]["currentValue"] = QString::fromUtf8("Current value");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["VariableInPresentationEditor"]["description"] = QString::fromUtf8("Description");
	mPropertiesDisplayedNamesMap["QUbiqPresentationEditor"]["VariableInPresentationEditor"]["type"] = QString::fromUtf8("Type of variable");
}

void QUbiqMetamodelPlugin::initPaletteGroupsMap()
{
}

void QUbiqMetamodelPlugin::initPaletteGroupsDescriptionMap()
{
}

void QUbiqMetamodelPlugin::initMouseGestureMap()
{
}

void QUbiqMetamodelPlugin::initPropertyMap()
{
	mPropertyTypes["ConditionLink"]["boolValueStereotype"] = "BoolValue";
	mPropertyTypes["CyclicLink"]["cyclicValueStereotype"] = "CyclicValue";
	mPropertyTypes["Return"]["value"] = "BoolValue";
	mPropertyTypes["Variable"]["type"] = "ElementTypes";
	mPropertyTypes["ConditionLinkInLogicEditor"]["boolValueStereotype"] = "BoolValue";
	mPropertyTypes["CyclicLinkInLogicEditor"]["cyclicValueStereotype"] = "CyclicValueInLogicEditor";
	mPropertyTypes["Message"]["messageType"] = "SentMessageType";
	mPropertyTypes["Message"]["sentTo"] = "SentToType";
	mPropertyTypes["QUbiqLogicDiagram"]["handlerType"] = "HandlerType";
	mPropertyTypes["ReturnInLogicEditor"]["value"] = "BoolValue";
	mPropertyTypes["Take"]["takeType"] = "TakeType";
	mPropertyTypes["VariableInLogicEditor"]["type"] = "ElementTypes";
	mPropertyTypes["WaitMessage"]["waitFrom"] = "SentToType";
	mPropertyTypes["WaitMessage"]["waitMessageType"] = "WaitMessageType";
	mPropertyTypes["Button"]["x"] = "Int";
	mPropertyTypes["Button"]["y"] = "Int";
	mPropertyTypes["ExitButton"]["x"] = "Int";
	mPropertyTypes["ExitButton"]["y"] = "Int";
	mPropertyTypes["Grid"]["h"] = "Int";
	mPropertyTypes["Grid"]["isNull"] = "Bool";
	mPropertyTypes["Grid"]["w"] = "Int";
	mPropertyTypes["Grid"]["x"] = "Int";
	mPropertyTypes["Grid"]["y"] = "Int";
	mPropertyTypes["Image"]["isNull"] = "Bool";
	mPropertyTypes["Image"]["x"] = "Int";
	mPropertyTypes["Image"]["y"] = "Int";
	mPropertyTypes["List"]["elementsCount"] = "Int";
	mPropertyTypes["List"]["isNull"] = "Bool";
	mPropertyTypes["List"]["x"] = "Int";
	mPropertyTypes["List"]["y"] = "Int";
	mPropertyTypes["Slide"]["height"] = "Int";
	mPropertyTypes["Slide"]["width"] = "Int";
	mPropertyTypes["Text"]["isNull"] = "Bool";
	mPropertyTypes["Text"]["x"] = "Int";
	mPropertyTypes["Text"]["y"] = "Int";
	mPropertyTypes["VariableInPresentationEditor"]["type"] = "ElementTypes";
}

void QUbiqMetamodelPlugin::initPropertyDefaultsMap()
{
	mPropertyDefault["ConditionLink"]["boolValueStereotype"] = QString::fromUtf8("none");
	mPropertyDefault["CyclicLink"]["cyclicValueStereotype"] = QString::fromUtf8("next");
	mPropertyDefault["Return"]["value"] = QString::fromUtf8("none");
	mPropertyDefault["Variable"]["type"] = QString::fromUtf8("text");
	mPropertyDefault["ConditionLinkInLogicEditor"]["boolValueStereotype"] = QString::fromUtf8("none");
	mPropertyDefault["CyclicLinkInLogicEditor"]["cyclicValueStereotype"] = QString::fromUtf8("next");
	mPropertyDefault["ReturnInLogicEditor"]["value"] = QString::fromUtf8("none");
	mPropertyDefault["Take"]["takeType"] = QString::fromUtf8("all");
	mPropertyDefault["VariableInLogicEditor"]["type"] = QString::fromUtf8("text");
	mPropertyDefault["Button"]["x"] = QString::fromUtf8("1");
	mPropertyDefault["Button"]["y"] = QString::fromUtf8("1");
	mPropertyDefault["ExitButton"]["x"] = QString::fromUtf8("1");
	mPropertyDefault["ExitButton"]["y"] = QString::fromUtf8("1");
	mPropertyDefault["Grid"]["h"] = QString::fromUtf8("1");
	mPropertyDefault["Grid"]["isNull"] = QString::fromUtf8("false");
	mPropertyDefault["Grid"]["w"] = QString::fromUtf8("1");
	mPropertyDefault["Grid"]["x"] = QString::fromUtf8("1");
	mPropertyDefault["Grid"]["y"] = QString::fromUtf8("1");
	mPropertyDefault["Image"]["isNull"] = QString::fromUtf8("false");
	mPropertyDefault["Image"]["x"] = QString::fromUtf8("1");
	mPropertyDefault["Image"]["y"] = QString::fromUtf8("1");
	mPropertyDefault["List"]["isNull"] = QString::fromUtf8("false");
	mPropertyDefault["List"]["x"] = QString::fromUtf8("1");
	mPropertyDefault["List"]["y"] = QString::fromUtf8("1");
	mPropertyDefault["Slide"]["height"] = QString::fromUtf8("1");
	mPropertyDefault["Slide"]["width"] = QString::fromUtf8("1");
	mPropertyDefault["Text"]["isNull"] = QString::fromUtf8("false");
	mPropertyDefault["Text"]["x"] = QString::fromUtf8("1");
	mPropertyDefault["Text"]["y"] = QString::fromUtf8("1");
	mPropertyDefault["VariableInPresentationEditor"]["type"] = QString::fromUtf8("text");
}

void QUbiqMetamodelPlugin::initDescriptionMap()
{
}

void QUbiqMetamodelPlugin::initParentsMap()
{
	mParentsMap["QUbiqConditionEditor"]["ForNode"]
		<< qMakePair(QString("QUbiqConditionEditor"), QString("AbstractGridOrListNode"))
	;
	mParentsMap["QUbiqConditionEditor"]["IfNode"]
		<< qMakePair(QString("QUbiqConditionEditor"), QString("AbstractConditionalNode"))
	;
	mParentsMap["QUbiqConditionEditor"]["TakeGridOrListElements"]
		<< qMakePair(QString("QUbiqConditionEditor"), QString("AbstractGridOrListNode"))
	;
	mParentsMap["QUbiqConditionEditor"]["While"]
		<< qMakePair(QString("QUbiqConditionEditor"), QString("AbstractConditionalNode"))
	;
	mParentsMap["QUbiqLogicEditor"]["ForNodeInLogicEditor"]
		<< qMakePair(QString("QUbiqLogicEditor"), QString("AbstractGridOrListNode"))
	;
	mParentsMap["QUbiqLogicEditor"]["IfNodeInLogicEditor"]
		<< qMakePair(QString("QUbiqLogicEditor"), QString("AbstractConditionalNode"))
	;
	mParentsMap["QUbiqLogicEditor"]["Message"]
		<< qMakePair(QString("QUbiqLogicEditor"), QString("AbstractMessage"))
	;
	mParentsMap["QUbiqLogicEditor"]["TakeGridOrListElementsInLogicEditor"]
		<< qMakePair(QString("QUbiqLogicEditor"), QString("AbstractGridOrListNode"))
	;
	mParentsMap["QUbiqLogicEditor"]["WaitMessage"]
		<< qMakePair(QString("QUbiqLogicEditor"), QString("AbstractMessage"))
	;
	mParentsMap["QUbiqLogicEditor"]["WhileInLogicEditor"]
		<< qMakePair(QString("QUbiqLogicEditor"), QString("AbstractConditionalNode"))
	;
	mParentsMap["QUbiqPresentationEditor"]["AbstractIsNullNode"]
		<< qMakePair(QString("QUbiqPresentationEditor"), QString("AbstractSlideElement"))
	;
	mParentsMap["QUbiqPresentationEditor"]["AbstractListsNode"]
		<< qMakePair(QString("QUbiqPresentationEditor"), QString("AbstractHandlerNode"))
		<< qMakePair(QString("QUbiqPresentationEditor"), QString("AbstractIsNullNode"))
	;
	mParentsMap["QUbiqPresentationEditor"]["Button"]
		<< qMakePair(QString("QUbiqPresentationEditor"), QString("AbstractHandlerNode"))
		<< qMakePair(QString("QUbiqPresentationEditor"), QString("AbstractSlideElement"))
	;
	mParentsMap["QUbiqPresentationEditor"]["ExitButton"]
		<< qMakePair(QString("QUbiqPresentationEditor"), QString("Button"))
	;
	mParentsMap["QUbiqPresentationEditor"]["Grid"]
		<< qMakePair(QString("QUbiqPresentationEditor"), QString("AbstractListsNode"))
	;
	mParentsMap["QUbiqPresentationEditor"]["Image"]
		<< qMakePair(QString("QUbiqPresentationEditor"), QString("AbstractIsNullNode"))
	;
	mParentsMap["QUbiqPresentationEditor"]["List"]
		<< qMakePair(QString("QUbiqPresentationEditor"), QString("AbstractListsNode"))
	;
	mParentsMap["QUbiqPresentationEditor"]["Slide"]
		<< qMakePair(QString("QUbiqPresentationEditor"), QString("AbstractHandlerNode"))
	;
	mParentsMap["QUbiqPresentationEditor"]["Text"]
		<< qMakePair(QString("QUbiqPresentationEditor"), QString("AbstractIsNullNode"))
	;
}

void QUbiqMetamodelPlugin::initShallPaletteBeSortedMap()
{
	mShallPaletteBeSortedMap[QString::fromUtf8("QUbiqConditionEditor")] = true;
	mShallPaletteBeSortedMap[QString::fromUtf8("QUbiqLogicEditor")] = true;
	mShallPaletteBeSortedMap[QString::fromUtf8("QUbiqPresentationEditor")] = true;
}

void QUbiqMetamodelPlugin::initExplosionsMap()
{
}

QStringList QUbiqMetamodelPlugin::diagrams() const
{
	return mDiagramNameMap.keys();
}

QStringList QUbiqMetamodelPlugin::diagramPaletteGroups(QString const &diagram) const
{
	QStringList result;
	for (QPair<QString, QStringList> const &group : mPaletteGroupsMap[diagram]) {
		result << group.first;
	}

	return result;
}

QStringList QUbiqMetamodelPlugin::diagramPaletteGroupList(QString const &diagram, QString const &group) const
{
	for (QPair<QString, QStringList> const &ourGroup : mPaletteGroupsMap[diagram]) {
		if (ourGroup.first == group) {
			return ourGroup.second;
		}
	}

	return QStringList();
}

QString QUbiqMetamodelPlugin::diagramPaletteGroupDescription(QString const &diagram, QString const &group) const
{
	return mPaletteGroupsDescriptionMap[diagram][group];
}

bool QUbiqMetamodelPlugin::shallPaletteBeSorted(QString const &diagram) const
{
	return mShallPaletteBeSortedMap[diagram];
}

QStringList QUbiqMetamodelPlugin::elements(QString const &diagram) const
{
	return mElementsNameMap[diagram].keys();
}

QStringList QUbiqMetamodelPlugin::getPropertiesWithDefaultValues(QString const &element) const
{
	return mPropertyDefault[element].keys();
}

QString QUbiqMetamodelPlugin::editorName() const
{
	 return "QUbiqMetamodel Editor";
}

QString QUbiqMetamodelPlugin::diagramName(QString const &diagram) const
{
	return mDiagramNameMap[diagram];
}

QString QUbiqMetamodelPlugin::diagramNodeName(QString const &diagram) const
{
	return mDiagramNodeNameMap[diagram];
}

QString QUbiqMetamodelPlugin::elementName(QString const &diagram, QString const &element) const
{
	return mElementsNameMap[diagram][element];
}

QString QUbiqMetamodelPlugin::elementDescription(QString const &diagram, QString const &element)const
{
	return mElementsDescriptionMap[diagram][element];
}

QString QUbiqMetamodelPlugin::propertyDescription(QString const &diagram, QString const &element, QString const &property) const
{
	return mPropertiesDescriptionMap[diagram][element][property];
}

QString QUbiqMetamodelPlugin::propertyDisplayedName(QString const &diagram, QString const &element, QString const &property) const
{
	return mPropertiesDisplayedNamesMap[diagram][element][property];
}

QString QUbiqMetamodelPlugin::elementMouseGesture(QString const &diagram, QString const &element) const
{
	return mElementMouseGesturesMap[diagram][element];
}

QList<qReal::EditorInterface::ExplosionData>QUbiqMetamodelPlugin::explosions(QString const &diagram, QString const &element) const 
{
	return mExplosionsMap[diagram][element];
}

qReal::ElementImpl* QUbiqMetamodelPlugin::getGraphicalObject(QString const &/*diagram*/, QString const &element) const
{
	if (element == "BeginNode") {
		return new BeginNode();
	}
	else if (element == "Change") {
		return new Change();
	}
	else if (element == "ConditionLink") {
		return new ConditionLink();
	}
	else if (element == "CyclicLink") {
		return new CyclicLink();
	}
	else if (element == "ForNode") {
		return new ForNode();
	}
	else if (element == "IfNode") {
		return new IfNode();
	}
	else if (element == "Link") {
		return new Link();
	}
	else if (element == "OutputSplitter") {
		return new OutputSplitter();
	}
	else if (element == "QUbiqConditionDiagram") {
		return new QUbiqConditionDiagram();
	}
	else if (element == "Return") {
		return new Return();
	}
	else if (element == "TakeGridOrListElements") {
		return new TakeGridOrListElements();
	}
	else if (element == "Variable") {
		return new Variable();
	}
	else if (element == "While") {
		return new While();
	}
	else if (element == "BeginNodeInLogicEditor") {
		return new BeginNodeInLogicEditor();
	}
	else if (element == "ChangeInLogicEditor") {
		return new ChangeInLogicEditor();
	}
	else if (element == "ConditionLinkInLogicEditor") {
		return new ConditionLinkInLogicEditor();
	}
	else if (element == "CyclicLinkInLogicEditor") {
		return new CyclicLinkInLogicEditor();
	}
	else if (element == "ForNodeInLogicEditor") {
		return new ForNodeInLogicEditor();
	}
	else if (element == "Go") {
		return new Go();
	}
	else if (element == "IfNodeInLogicEditor") {
		return new IfNodeInLogicEditor();
	}
	else if (element == "LinkInLogicEditor") {
		return new LinkInLogicEditor();
	}
	else if (element == "Message") {
		return new Message();
	}
	else if (element == "OutputSplitterInLogicEditor") {
		return new OutputSplitterInLogicEditor();
	}
	else if (element == "QUbiqLogicDiagram") {
		return new QUbiqLogicDiagram();
	}
	else if (element == "ReturnInLogicEditor") {
		return new ReturnInLogicEditor();
	}
	else if (element == "Take") {
		return new Take();
	}
	else if (element == "TakeGridOrListElementsInLogicEditor") {
		return new TakeGridOrListElementsInLogicEditor();
	}
	else if (element == "VariableInLogicEditor") {
		return new VariableInLogicEditor();
	}
	else if (element == "WaitMessage") {
		return new WaitMessage();
	}
	else if (element == "WhileInLogicEditor") {
		return new WhileInLogicEditor();
	}
	else if (element == "BeginNodeInPresentationEditor") {
		return new BeginNodeInPresentationEditor();
	}
	else if (element == "Button") {
		return new Button();
	}
	else if (element == "ExitButton") {
		return new ExitButton();
	}
	else if (element == "Grid") {
		return new Grid();
	}
	else if (element == "Image") {
		return new Image();
	}
	else if (element == "List") {
		return new List();
	}
	else if (element == "QUbiqPresentationDiagram") {
		return new QUbiqPresentationDiagram();
	}
	else if (element == "Slide") {
		return new Slide();
	}
	else if (element == "Text") {
		return new Text();
	}
	else if (element == "TransitionLink") {
		return new TransitionLink();
	}
	else if (element == "VariableInPresentationEditor") {
		return new VariableInPresentationEditor();
	}
	else {
		Q_ASSERT(!"Request for creation of an element with unknown name");
		return NULL;
	}
}

bool QUbiqMetamodelPlugin::isParentOf(QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const
{
	if (childDiagram == parentDiagram && childElement == parentElement)
		return true;
	if (mParentsMap[childDiagram][childElement].contains(qMakePair(parentDiagram, parentElement)))
		return true;
	typedef QPair<QString, QString> StringPair;
	foreach (StringPair const &pair, mParentsMap[childDiagram][childElement])
		if (isParentOf(parentDiagram, parentElement, pair.first, pair.second))
			return true;
	return false;
}
QList<QPair<QString, QString> > QUbiqMetamodelPlugin::getParentsOf(QString const &diagram, QString const &element) const
{
	return mParentsMap[diagram][element];
}
QStringList QUbiqMetamodelPlugin::getPropertyNames(QString const &/*diagram*/, QString const &element) const
{
	QStringList result;
	if (element == "BeginNode") {
	}
	else if (element == "Change") {
		result  << "elementId" << "newValue" << "propertyName";
	}
	else if (element == "ConditionLink") {
		result  << "boolValueStereotype";
	}
	else if (element == "CyclicLink") {
		result  << "cyclicValueStereotype";
	}
	else if (element == "ForNode") {
		result  << "gridOrListId" << "hCounterId" << "wCounterId";
	}
	else if (element == "IfNode") {
		result  << "condition";
	}
	else if (element == "Link") {
	}
	else if (element == "OutputSplitter") {
	}
	else if (element == "QUbiqConditionDiagram") {
		result  << "description";
	}
	else if (element == "Return") {
		result  << "value";
	}
	else if (element == "TakeGridOrListElements") {
		result  << "condition" << "gridOrListId" << "variableId";
	}
	else if (element == "Variable") {
		result  << "currentValue" << "description" << "type";
	}
	else if (element == "While") {
		result  << "condition" << "counterId";
	}
	else if (element == "BeginNodeInLogicEditor") {
	}
	else if (element == "ChangeInLogicEditor") {
		result  << "elementId" << "newValue" << "propertyName";
	}
	else if (element == "ConditionLinkInLogicEditor") {
		result  << "boolValueStereotype";
	}
	else if (element == "CyclicLinkInLogicEditor") {
		result  << "cyclicValueStereotype";
	}
	else if (element == "ForNodeInLogicEditor") {
		result  << "gridOrListId" << "hCounterId" << "wCounterId";
	}
	else if (element == "Go") {
		result  << "goto";
	}
	else if (element == "IfNodeInLogicEditor") {
		result  << "condition";
	}
	else if (element == "LinkInLogicEditor") {
	}
	else if (element == "Message") {
		result  << "additionalText" << "messageType" << "sentTo";
	}
	else if (element == "OutputSplitterInLogicEditor") {
	}
	else if (element == "QUbiqLogicDiagram") {
		result  << "handlerType";
	}
	else if (element == "ReturnInLogicEditor") {
		result  << "value";
	}
	else if (element == "Take") {
		result  << "takeFrom" << "takeTo" << "takeType";
	}
	else if (element == "TakeGridOrListElementsInLogicEditor") {
		result  << "condition" << "gridOrListId" << "variableId";
	}
	else if (element == "VariableInLogicEditor") {
		result  << "currentValue" << "description" << "type";
	}
	else if (element == "WaitMessage") {
		result  << "additionalText" << "waitFrom" << "waitMessageType";
	}
	else if (element == "WhileInLogicEditor") {
		result  << "condition" << "counterId";
	}
	else if (element == "BeginNodeInPresentationEditor") {
	}
	else if (element == "Button") {
		result  << "buttonText" << "handler" << "x" << "y";
	}
	else if (element == "ExitButton") {
		result  << "buttonText" << "handler" << "x" << "y";
	}
	else if (element == "Grid") {
		result  << "elementId" << "h" << "handler" << "isNull" << "w" << "x" << "y";
	}
	else if (element == "Image") {
		result  << "isNull" << "pathToImage" << "x" << "y";
	}
	else if (element == "List") {
		result  << "elementId" << "elementsCount" << "handler" << "isNull" << "x" << "y";
	}
	else if (element == "QUbiqPresentationDiagram") {
		result  << "pathToGenerate" << "programName";
	}
	else if (element == "Slide") {
		result  << "description" << "handler" << "height" << "width";
	}
	else if (element == "Text") {
		result  << "isNull" << "text" << "x" << "y";
	}
	else if (element == "TransitionLink") {
	}
	else if (element == "VariableInPresentationEditor") {
		result  << "currentValue" << "description" << "type";
	}
	return result;
}

QStringList QUbiqMetamodelPlugin::getPortTypes(QString const &/*diagram*/, QString const &element) const
{
	QStringList result;
	if (element == "AbstractConditionalNode") {
	}
	else if (element == "AbstractGridOrListNode") {
	}
	else if (element == "BeginNode") {
		result << "NonTyped";
	}
	else if (element == "Change") {
		result << "NonTyped";
	}
	else if (element == "ForNode") {
		result << "NonTyped";
	}
	else if (element == "IfNode") {
		result << "NonTyped";
	}
	else if (element == "OutputSplitter") {
		result << "NonTyped";
	}
	else if (element == "QUbiqConditionDiagram") {
	}
	else if (element == "Return") {
		result << "NonTyped";
	}
	else if (element == "TakeGridOrListElements") {
		result << "NonTyped";
	}
	else if (element == "Variable") {
	}
	else if (element == "While") {
		result << "NonTyped";
	}
	else if (element == "AbstractMessage") {
	}
	else if (element == "BeginNodeInLogicEditor") {
		result << "NonTyped";
	}
	else if (element == "ChangeInLogicEditor") {
		result << "NonTyped";
	}
	else if (element == "ForNodeInLogicEditor") {
		result << "NonTyped";
	}
	else if (element == "Go") {
		result << "NonTyped";
	}
	else if (element == "IfNodeInLogicEditor") {
		result << "NonTyped";
	}
	else if (element == "Message") {
		result << "NonTyped";
	}
	else if (element == "OutputSplitterInLogicEditor") {
		result << "NonTyped";
	}
	else if (element == "QUbiqLogicDiagram") {
	}
	else if (element == "ReturnInLogicEditor") {
		result << "NonTyped";
	}
	else if (element == "Take") {
		result << "NonTyped";
	}
	else if (element == "TakeGridOrListElementsInLogicEditor") {
		result << "NonTyped";
	}
	else if (element == "VariableInLogicEditor") {
	}
	else if (element == "WaitMessage") {
		result << "NonTyped";
	}
	else if (element == "WhileInLogicEditor") {
		result << "NonTyped";
	}
	else if (element == "AbstractHandlerNode") {
	}
	else if (element == "AbstractIsNullNode") {
	}
	else if (element == "AbstractListsNode") {
	}
	else if (element == "AbstractSlideElement") {
	}
	else if (element == "BeginNodeInPresentationEditor") {
		result << "NonTyped";
	}
	else if (element == "Button") {
		result << "NonTyped";
	}
	else if (element == "ExitButton") {
		result << "NonTyped";
	}
	else if (element == "Grid") {
	}
	else if (element == "Image") {
	}
	else if (element == "List") {
	}
	else if (element == "QUbiqPresentationDiagram") {
	}
	else if (element == "Slide") {
		result << "NonTyped";
	}
	else if (element == "Text") {
	}
	else if (element == "VariableInPresentationEditor") {
	}
	return result;
}

QStringList QUbiqMetamodelPlugin::getReferenceProperties(QString const &/*diagram*/, QString const &element) const
{
	QStringList result;
	if (element == "BeginNode") {
	}
	else if (element == "Change") {
	}
	else if (element == "ConditionLink") {
	}
	else if (element == "CyclicLink") {
	}
	else if (element == "ForNode") {
	}
	else if (element == "IfNode") {
	}
	else if (element == "Link") {
	}
	else if (element == "OutputSplitter") {
	}
	else if (element == "QUbiqConditionDiagram") {
	}
	else if (element == "Return") {
	}
	else if (element == "TakeGridOrListElements") {
	}
	else if (element == "Variable") {
	}
	else if (element == "While") {
	}
	else if (element == "BeginNodeInLogicEditor") {
	}
	else if (element == "ChangeInLogicEditor") {
	}
	else if (element == "ConditionLinkInLogicEditor") {
	}
	else if (element == "CyclicLinkInLogicEditor") {
	}
	else if (element == "ForNodeInLogicEditor") {
	}
	else if (element == "Go") {
	}
	else if (element == "IfNodeInLogicEditor") {
	}
	else if (element == "LinkInLogicEditor") {
	}
	else if (element == "Message") {
	}
	else if (element == "OutputSplitterInLogicEditor") {
	}
	else if (element == "QUbiqLogicDiagram") {
	}
	else if (element == "ReturnInLogicEditor") {
	}
	else if (element == "Take") {
	}
	else if (element == "TakeGridOrListElementsInLogicEditor") {
	}
	else if (element == "VariableInLogicEditor") {
	}
	else if (element == "WaitMessage") {
	}
	else if (element == "WhileInLogicEditor") {
	}
	else if (element == "BeginNodeInPresentationEditor") {
	}
	else if (element == "Button") {
	}
	else if (element == "ExitButton") {
	}
	else if (element == "Grid") {
	}
	else if (element == "Image") {
	}
	else if (element == "List") {
	}
	else if (element == "QUbiqPresentationDiagram") {
	}
	else if (element == "Slide") {
	}
	else if (element == "Text") {
	}
	else if (element == "TransitionLink") {
	}
	else if (element == "VariableInPresentationEditor") {
	}
	return result;
}

QStringList QUbiqMetamodelPlugin::getTypesContainedBy(QString const &element) const
{
	QStringList result;
	if (element == "QUbiqConditionDiagram") {
		result << "Return" << "AbstractGridOrListNode" << "Change" << "AbstractConditionalNode" << "Variable" << "OutputSplitter" << "BeginNode" ;
	}
	else if (element == "QUbiqLogicDiagram") {
		result << "Go" << "Message" << "Take" << "WaitMessage" << "VariableInLogicEditor" << "WhileInLogicEditor" << "IfNodeInLogicEditor" << "ChangeInLogicEditor" << "BeginNodeInLogicEditor" << "TakeGridOrListElementsInLogicEditor" << "ForNodeInLogicEditor" << "OutputSplitterInLogicEditor" << "ReturnInLogicEditor" ;
	}
	else if (element == "Button") {
		result << "Image" << "Text" ;
	}
	else if (element == "ExitButton") {
		result << "Text" << "Image" ;
	}
	else if (element == "Grid") {
		result << "Button" << "Text" << "Image" ;
	}
	else if (element == "List") {
		result << "Text" << "Image" << "Button" ;
	}
	else if (element == "QUbiqPresentationDiagram") {
		result << "AbstractHandlerNode" << "VariableInPresentationEditor" << "Text" << "Image" ;
	}
	else if (element == "Slide") {
		result << "AbstractHandlerNode" << "VariableInPresentationEditor" << "Text" << "Image" << "BeginNodeInPresentationEditor" ;
	}
	return result;
}

QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > QUbiqMetamodelPlugin::getPossibleEdges(QString const &element) const
{
	QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > result;
	Q_UNUSED(element);
	return result;
}

//(-1) means "edge", (+1) means "node"
int QUbiqMetamodelPlugin::isNodeOrEdge(QString const &element) const
{
	if (element == "AbstractConditionalNode")
		return 1;
	else if (element == "AbstractGridOrListNode")
		return 1;
	else if (element == "BeginNode")
		return 1;
	else if (element == "BoolValue")
		return 0;
	else if (element == "Change")
		return 1;
	else if (element == "ConditionLink")
		return -1;
	else if (element == "CyclicLink")
		return -1;
	else if (element == "CyclicValue")
		return 0;
	else if (element == "ElementTypes")
		return 0;
	else if (element == "ForNode")
		return 1;
	else if (element == "IfNode")
		return 1;
	else if (element == "Link")
		return -1;
	else if (element == "OutputSplitter")
		return 1;
	else if (element == "QUbiqConditionDiagram")
		return 1;
	else if (element == "Return")
		return 1;
	else if (element == "TakeGridOrListElements")
		return 1;
	else if (element == "Variable")
		return 1;
	else if (element == "While")
		return 1;
	else if (element == "AbstractMessage")
		return 1;
	else if (element == "BeginNodeInLogicEditor")
		return 1;
	else if (element == "ChangeInLogicEditor")
		return 1;
	else if (element == "ConditionLinkInLogicEditor")
		return -1;
	else if (element == "CyclicLinkInLogicEditor")
		return -1;
	else if (element == "CyclicValueInLogicEditor")
		return 0;
	else if (element == "ForNodeInLogicEditor")
		return 1;
	else if (element == "Go")
		return 1;
	else if (element == "HandlerType")
		return 0;
	else if (element == "IfNodeInLogicEditor")
		return 1;
	else if (element == "LinkInLogicEditor")
		return -1;
	else if (element == "Message")
		return 1;
	else if (element == "OutputSplitterInLogicEditor")
		return 1;
	else if (element == "QUbiqLogicDiagram")
		return 1;
	else if (element == "ReturnInLogicEditor")
		return 1;
	else if (element == "SentMessageType")
		return 0;
	else if (element == "SentToType")
		return 0;
	else if (element == "Take")
		return 1;
	else if (element == "TakeGridOrListElementsInLogicEditor")
		return 1;
	else if (element == "TakeType")
		return 0;
	else if (element == "VariableInLogicEditor")
		return 1;
	else if (element == "WaitMessage")
		return 1;
	else if (element == "WaitMessageType")
		return 0;
	else if (element == "WhileInLogicEditor")
		return 1;
	else if (element == "AbstractHandlerNode")
		return 1;
	else if (element == "AbstractIsNullNode")
		return 1;
	else if (element == "AbstractListsNode")
		return 1;
	else if (element == "AbstractSlideElement")
		return 1;
	else if (element == "BeginNodeInPresentationEditor")
		return 1;
	else if (element == "Button")
		return 1;
	else if (element == "ExitButton")
		return 1;
	else if (element == "Grid")
		return 1;
	else if (element == "Image")
		return 1;
	else if (element == "List")
		return 1;
	else if (element == "QUbiqPresentationDiagram")
		return 1;
	else if (element == "Slide")
		return 1;
	else if (element == "Text")
		return 1;
	else if (element == "TransitionLink")
		return -1;
	else if (element == "VariableInPresentationEditor")
		return 1;
	return 0;
}
QString QUbiqMetamodelPlugin::getGroupsXML() const 
{
	return QString::fromUtf8("");
}

QStringList QUbiqMetamodelPlugin::getEnumValues(QString name) const 
{
	QStringList result;
	if (name == "SentMessageType")
		result << QString::fromUtf8("request game") << QString::fromUtf8("request of playing field status") << QString::fromUtf8("sent a playing field status") << QString::fromUtf8("request a players list");
	else if (name == "CyclicValue")
		result << QString::fromUtf8("next") << QString::fromUtf8("after");
	else if (name == "ElementTypes")
		result << QString::fromUtf8("text") << QString::fromUtf8("list") << QString::fromUtf8("image") << QString::fromUtf8("grid") << QString::fromUtf8("number");
	else if (name == "TakeType")
		result << QString::fromUtf8("only unselected data") << QString::fromUtf8("all data") << QString::fromUtf8("only selected data");
	else if (name == "BoolValue")
		result << QString::fromUtf8("none") << QString::fromUtf8("false") << QString::fromUtf8("true");
	else if (name == "HandlerType")
		result << QString::fromUtf8("slide handler") << QString::fromUtf8("list handler") << QString::fromUtf8("button handler") << QString::fromUtf8("grid handler");
	else if (name == "CyclicValueInLogicEditor")
		result << QString::fromUtf8("next") << QString::fromUtf8("after");
	else if (name == "SentToType")
		result << QString::fromUtf8("server") << QString::fromUtf8("other player") << QString::fromUtf8("keyboard");
	else if (name == "WaitMessageType")
		result << QString::fromUtf8("response about playing field status") << QString::fromUtf8("response to game request") << QString::fromUtf8("pressing on a keyboard");
	return result;
}

QString QUbiqMetamodelPlugin::getPropertyType(QString const &element, QString const &property) const
{
	return mPropertyTypes[element][property];
}

QString QUbiqMetamodelPlugin::getPropertyDefaultValue(QString const &element, QString const &property) const
{
	return mPropertyDefault[element][property];
}

QList<qReal::ListenerInterface*> QUbiqMetamodelPlugin::listeners() const
{
	QList<qReal::ListenerInterface*> result;
	return result;
}
