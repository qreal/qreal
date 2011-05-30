#include "pluginInterface.h"

#include "elements.h"


Q_EXPORT_PLUGIN2(qreal_editors, AndroidMetamodelPlugin)

AndroidMetamodelPlugin::AndroidMetamodelPlugin()
{
	initPlugin();
}

void AndroidMetamodelPlugin::initPlugin()
{
	initNameMap();
	initMouseGestureMap();
	initPropertyMap();
	initPropertyDefaultsMap();
	initDescriptionMap();
	initParentsMap();
}

void AndroidMetamodelPlugin::initNameMap()
{
	diagramNameMap["Handler_Diagram"] = QString::fromUtf8("Handler Diagram");
	diagramNodeNameMap["Handler_Diagram"] = "Handler Diagram";

	diagramNameMap["Layout_Diagram"] = QString::fromUtf8("Layout Diagram");
	diagramNodeNameMap["Layout_Diagram"] = "Layout Diagram";

	diagramNameMap["Main_Diagram"] = QString::fromUtf8("Main Diagram");
	diagramNodeNameMap["Main_Diagram"] = "Main Diagram";

	elementsNameMap["Handler_Diagram"]["List"] = QString::fromUtf8("List");
	elementsNameMap["Handler_Diagram"]["Object"] = QString::fromUtf8("Object");
	elementsNameMap["Handler_Diagram"]["Parameter"] = QString::fromUtf8("Parameter");
	elementsNameMap["Handler_Diagram"]["Tag"] = QString::fromUtf8("Tag");
	elementsNameMap["Handler_Diagram"]["Text"] = QString::fromUtf8("Text");
	elementsNameMap["Handler_Diagram"]["Variable"] = QString::fromUtf8("Variable");
	elementsNameMap["Handler_Diagram"]["CollectIn"] = QString::fromUtf8("collectIn");
	elementsNameMap["Handler_Diagram"]["Enclose"] = QString::fromUtf8("enclose");
	elementsNameMap["Handler_Diagram"]["SaveAs"] = QString::fromUtf8("saveAs");
	elementsNameMap["Handler_Diagram"]["SaveIn"] = QString::fromUtf8("saveIn");
	elementsNameMap["Layout_Diagram"]["Button"] = QString::fromUtf8("Button");
	elementsNameMap["Layout_Diagram"]["Check_Box"] = QString::fromUtf8("Check Box");
	elementsNameMap["Layout_Diagram"]["Edit_Text"] = QString::fromUtf8("Edit Text");
	elementsNameMap["Layout_Diagram"]["Frame_Layout"] = QString::fromUtf8("Frame Layout");
	elementsNameMap["Layout_Diagram"]["Gallery"] = QString::fromUtf8("Gallery");
	elementsNameMap["Layout_Diagram"]["Grid_View"] = QString::fromUtf8("Grid View");
	elementsNameMap["Layout_Diagram"]["Image"] = QString::fromUtf8("Image");
	elementsNameMap["Layout_Diagram"]["Linear_Layout"] = QString::fromUtf8("Linear Layout");
	elementsNameMap["Layout_Diagram"]["List_View"] = QString::fromUtf8("List View");
	elementsNameMap["Layout_Diagram"]["List_of_Text_Views"] = QString::fromUtf8("List of Text Views");
	elementsNameMap["Layout_Diagram"]["Scroll_View"] = QString::fromUtf8("Scroll View");
	elementsNameMap["Layout_Diagram"]["Spinner"] = QString::fromUtf8("Spinner");
	elementsNameMap["Layout_Diagram"]["Table_Layout"] = QString::fromUtf8("Table Layout");
	elementsNameMap["Layout_Diagram"]["Table_Row"] = QString::fromUtf8("Table Row");
	elementsNameMap["Layout_Diagram"]["Text_View"] = QString::fromUtf8("Text View");
	elementsNameMap["Layout_Diagram"]["Contains"] = QString::fromUtf8("contains");
	elementsNameMap["Layout_Diagram"]["FollowingBy"] = QString::fromUtf8("followingBy");
	elementsNameMap["Main_Diagram"]["Begin"] = QString::fromUtf8("Begin");
	elementsNameMap["Main_Diagram"]["Close_Dialog"] = QString::fromUtf8("Close Dialog");
	elementsNameMap["Main_Diagram"]["Create_Activity"] = QString::fromUtf8("Create Activity");
	elementsNameMap["Main_Diagram"]["Create_Bound_Service"] = QString::fromUtf8("Create Bound Service");
	elementsNameMap["Main_Diagram"]["Create_Tab_Layout"] = QString::fromUtf8("Create Tab Layout");
	elementsNameMap["Main_Diagram"]["Custom_Toast_Notification"] = QString::fromUtf8("Custom Toast Notification");
	elementsNameMap["Main_Diagram"]["End"] = QString::fromUtf8("End");
	elementsNameMap["Main_Diagram"]["Http_Request"] = QString::fromUtf8("Http Request");
	elementsNameMap["Main_Diagram"]["Service_Success"] = QString::fromUtf8("Service Success");
	elementsNameMap["Main_Diagram"]["Set_Content_View"] = QString::fromUtf8("Set Content View");
	elementsNameMap["Main_Diagram"]["Show_Custom_Dialog"] = QString::fromUtf8("Show Custom Dialog");
	elementsNameMap["Main_Diagram"]["Show_Progress_Wheel"] = QString::fromUtf8("Show Progress Wheel");
	elementsNameMap["Main_Diagram"]["Show_Text_Dialog"] = QString::fromUtf8("Show Text Dialog");
	elementsNameMap["Main_Diagram"]["Toast_Notification"] = QString::fromUtf8("Toast Notification");
	elementsNameMap["Main_Diagram"]["BackgroundFlow"] = QString::fromUtf8("backgroundFlow");
	elementsNameMap["Main_Diagram"]["Fail"] = QString::fromUtf8("fail");
	elementsNameMap["Main_Diagram"]["MainFlow"] = QString::fromUtf8("mainFlow");
	elementsNameMap["Main_Diagram"]["Next"] = QString::fromUtf8("next");
	elementsNameMap["Main_Diagram"]["OnButtonClick"] = QString::fromUtf8("onButtonClick");
	elementsNameMap["Main_Diagram"]["OnClose"] = QString::fromUtf8("onClose");
	elementsNameMap["Main_Diagram"]["OnCreate"] = QString::fromUtf8("onCreate");
	elementsNameMap["Main_Diagram"]["OnDismiss"] = QString::fromUtf8("onDismiss");
	elementsNameMap["Main_Diagram"]["OnItemClick"] = QString::fromUtf8("onItemClick");
	elementsNameMap["Main_Diagram"]["Success"] = QString::fromUtf8("success");
	elementsNameMap["Main_Diagram"]["Tab"] = QString::fromUtf8("tab");
}

void AndroidMetamodelPlugin::initMouseGestureMap()
{
}

void AndroidMetamodelPlugin::initPropertyMap()
{
	propertyTypes["List"]["name"] = "String";
	propertyTypes["Object"]["name"] = "String";
	propertyTypes["Parameter"]["name"] = "String";
	propertyTypes["Tag"]["name"] = "String";
	propertyTypes["Variable"]["name"] = "String";
	propertyTypes["Button"]["label"] = "String";
	propertyTypes["Button"]["layoutHeight"] = "Layout_Value";
	propertyTypes["Button"]["layoutWidth"] = "Layout_Value";
	propertyTypes["Button"]["padding"] = "String";
	propertyTypes["Button"]["variable"] = "String";
	propertyTypes["Check_Box"]["default"] = "Check_Value";
	propertyTypes["Check_Box"]["layoutHeight"] = "Layout_Value";
	propertyTypes["Check_Box"]["layoutWidth"] = "Layout_Value";
	propertyTypes["Check_Box"]["padding"] = "String";
	propertyTypes["Check_Box"]["text"] = "String";
	propertyTypes["Check_Box"]["variable"] = "String";
	propertyTypes["Edit_Text"]["isPassword"] = "Bool";
	propertyTypes["Edit_Text"]["layoutHeight"] = "Layout_Value";
	propertyTypes["Edit_Text"]["layoutWidth"] = "Layout_Value";
	propertyTypes["Edit_Text"]["padding"] = "String";
	propertyTypes["Edit_Text"]["variable"] = "String";
	propertyTypes["Frame_Layout"]["background"] = "String";
	propertyTypes["Frame_Layout"]["layoutHeight"] = "Layout_Value";
	propertyTypes["Frame_Layout"]["layoutWidth"] = "Layout_Value";
	propertyTypes["Frame_Layout"]["padding"] = "String";
	propertyTypes["Gallery"]["layoutHeight"] = "Layout_Value";
	propertyTypes["Gallery"]["layoutWidth"] = "Layout_Value";
	propertyTypes["Gallery"]["padding"] = "String";
	propertyTypes["Grid_View"]["columnWidth"] = "Int";
	propertyTypes["Grid_View"]["horizontalSpacing"] = "Int";
	propertyTypes["Grid_View"]["layoutHeight"] = "Layout_Value";
	propertyTypes["Grid_View"]["layoutWidth"] = "Layout_Value";
	propertyTypes["Grid_View"]["numColumns"] = "Int";
	propertyTypes["Grid_View"]["padding"] = "String";
	propertyTypes["Grid_View"]["stretchMode"] = "Stretch_Mode_Value";
	propertyTypes["Grid_View"]["verticalSpacing"] = "Int";
	propertyTypes["Image"]["layoutHeight"] = "Layout_Value";
	propertyTypes["Image"]["layoutWidth"] = "Layout_Value";
	propertyTypes["Image"]["padding"] = "String";
	propertyTypes["Image"]["scaleType"] = "Scale_Value";
	propertyTypes["Image"]["source"] = "String";
	propertyTypes["Linear_Layout"]["background"] = "String";
	propertyTypes["Linear_Layout"]["layoutHeight"] = "Layout_Value";
	propertyTypes["Linear_Layout"]["layoutWidth"] = "Layout_Value";
	propertyTypes["Linear_Layout"]["orientation"] = "Orientation_Value";
	propertyTypes["Linear_Layout"]["padding"] = "String";
	propertyTypes["List_View"]["layoutHeight"] = "Layout_Value";
	propertyTypes["List_View"]["layoutWidth"] = "Layout_Value";
	propertyTypes["List_View"]["padding"] = "String";
	propertyTypes["List_of_Text_Views"]["itemsContent"] = "String";
	propertyTypes["List_of_Text_Views"]["itemsIds"] = "String";
	propertyTypes["List_of_Text_Views"]["layoutHeight"] = "Layout_Value";
	propertyTypes["List_of_Text_Views"]["layoutWidth"] = "Layout_Value";
	propertyTypes["List_of_Text_Views"]["padding"] = "String";
	propertyTypes["List_of_Text_Views"]["textViewLayoutHeight"] = "Layout_Value";
	propertyTypes["List_of_Text_Views"]["textViewLayoutWidth"] = "Layout_Value";
	propertyTypes["Scroll_View"]["layoutHeight"] = "Layout_Value";
	propertyTypes["Scroll_View"]["layoutWidth"] = "Layout_Value";
	propertyTypes["Scroll_View"]["padding"] = "String";
	propertyTypes["Spinner"]["itemsContent"] = "String";
	propertyTypes["Spinner"]["itemsIds"] = "String";
	propertyTypes["Spinner"]["layoutHeight"] = "Layout_Value";
	propertyTypes["Spinner"]["layoutWidth"] = "Layout_Value";
	propertyTypes["Spinner"]["padding"] = "String";
	propertyTypes["Table_Layout"]["layoutHeight"] = "Layout_Value";
	propertyTypes["Table_Layout"]["layoutWidth"] = "Layout_Value";
	propertyTypes["Table_Layout"]["padding"] = "String";
	propertyTypes["Table_Row"]["layoutHeight"] = "Layout_Value";
	propertyTypes["Table_Row"]["layoutWidth"] = "Layout_Value";
	propertyTypes["Table_Row"]["padding"] = "String";
	propertyTypes["Text_View"]["layoutHeight"] = "Layout_Value";
	propertyTypes["Text_View"]["layoutWidth"] = "Layout_Value";
	propertyTypes["Text_View"]["padding"] = "String";
	propertyTypes["Text_View"]["text"] = "String";
	propertyTypes["Create_Activity"]["label"] = "String";
	propertyTypes["Create_Activity"]["layout"] = "LayoutDiagram";
	propertyTypes["Create_Tab_Layout"]["deselectedIcons"] = "String";
	propertyTypes["Create_Tab_Layout"]["selectedIcons"] = "String";
	propertyTypes["Create_Tab_Layout"]["tabsIds"] = "String";
	propertyTypes["Create_Tab_Layout"]["tabsText"] = "String[]";
	propertyTypes["Custom_Toast_Notification"]["duration"] = "Toast_Duration_Value";
	propertyTypes["Custom_Toast_Notification"]["layout"] = "Layout_Diagram";
	propertyTypes["Http_Request"]["handler"] = "Handler_Diagram";
	propertyTypes["Http_Request"]["url"] = "String";
	propertyTypes["Set_Content_View"]["layout"] = "Layout_Diagram";
	propertyTypes["Show_Custom_Dialog"]["isCancelable"] = "Bool";
	propertyTypes["Show_Custom_Dialog"]["layout"] = "Layout_Diagram";
	propertyTypes["Show_Custom_Dialog"]["title"] = "String";
	propertyTypes["Show_Text_Dialog"]["isCancelable"] = "Bool";
	propertyTypes["Show_Text_Dialog"]["negativeButton"] = "String";
	propertyTypes["Show_Text_Dialog"]["neutralButton"] = "String";
	propertyTypes["Show_Text_Dialog"]["positiveButton"] = "String";
	propertyTypes["Show_Text_Dialog"]["text"] = "String";
	propertyTypes["Toast_Notification"]["duration"] = "Toast_Duration_Value";
	propertyTypes["Toast_Notification"]["text"] = "String";
	propertyTypes["OnButtonClick"]["buttonName"] = "String";
	propertyTypes["OnItemClick"]["itemName"] = "String";
	propertyTypes["Tab"]["tabId"] = "String";
}

void AndroidMetamodelPlugin::initPropertyDefaultsMap()
{
}

void AndroidMetamodelPlugin::initDescriptionMap()
{
}

void AndroidMetamodelPlugin::initParentsMap()
{
	parentsMap["Layout_Diagram"]["Button"]
		<< qMakePair(QString("Layout_Diagram"), QString("View"))
	;
	parentsMap["Layout_Diagram"]["Check_Box"]
		<< qMakePair(QString("Layout_Diagram"), QString("View"))
	;
	parentsMap["Layout_Diagram"]["Edit_Text"]
		<< qMakePair(QString("Layout_Diagram"), QString("View"))
	;
	parentsMap["Layout_Diagram"]["Frame_Layout"]
		<< qMakePair(QString("Layout_Diagram"), QString("View"))
	;
	parentsMap["Layout_Diagram"]["Gallery"]
		<< qMakePair(QString("Layout_Diagram"), QString("View"))
	;
	parentsMap["Layout_Diagram"]["Grid_View"]
		<< qMakePair(QString("Layout_Diagram"), QString("View"))
	;
	parentsMap["Layout_Diagram"]["Image"]
		<< qMakePair(QString("Layout_Diagram"), QString("View"))
	;
	parentsMap["Layout_Diagram"]["Linear_Layout"]
		<< qMakePair(QString("Layout_Diagram"), QString("View"))
	;
	parentsMap["Layout_Diagram"]["List_View"]
		<< qMakePair(QString("Layout_Diagram"), QString("View"))
	;
	parentsMap["Layout_Diagram"]["List_of_Text_Views"]
		<< qMakePair(QString("Layout_Diagram"), QString("View"))
	;
	parentsMap["Layout_Diagram"]["Scroll_View"]
		<< qMakePair(QString("Layout_Diagram"), QString("View"))
	;
	parentsMap["Layout_Diagram"]["Spinner"]
		<< qMakePair(QString("Layout_Diagram"), QString("View"))
	;
	parentsMap["Layout_Diagram"]["Table_Layout"]
		<< qMakePair(QString("Layout_Diagram"), QString("View"))
	;
	parentsMap["Layout_Diagram"]["Table_Row"]
		<< qMakePair(QString("Layout_Diagram"), QString("View"))
	;
	parentsMap["Layout_Diagram"]["Text_View"]
		<< qMakePair(QString("Layout_Diagram"), QString("View"))
	;
}

QStringList AndroidMetamodelPlugin::diagrams() const
{
	return diagramNameMap.keys();
}

QStringList AndroidMetamodelPlugin::elements(QString const &diagram) const
{
	return elementsNameMap[diagram].keys();
}

QStringList AndroidMetamodelPlugin::getPropertiesWithDefaultValues(QString const &element) const
{
	return propertyDefault[element].keys();
}

QIcon AndroidMetamodelPlugin::getIcon(SdfIconEngineV2Interface *engine) const
{
	return QIcon(engine);
}

QString AndroidMetamodelPlugin::editorName() const
{
	 return "AndroidMetamodel Editor";
}

QString AndroidMetamodelPlugin::diagramName(QString const &diagram) const
{
	return diagramNameMap[diagram];
}

QString AndroidMetamodelPlugin::diagramNodeName(QString const &diagram) const
{
	return diagramNodeNameMap[diagram];
}

QString AndroidMetamodelPlugin::elementName(QString const &diagram, QString const &element) const
{
	return elementsNameMap[diagram][element];
}

QString AndroidMetamodelPlugin::elementDescription(QString const &diagram, QString const &element) const
{
	return elementsDescriptionMap[diagram][element];
}

QString AndroidMetamodelPlugin::propertyDescription(QString const &diagram, QString const &element, QString const &property) const
{
	return propertiesDescriptionMap[diagram][element][property];
}

QString AndroidMetamodelPlugin::elementMouseGesture(QString const &diagram, QString const &element) const
{
	return elementMouseGesturesMap[diagram][element];
}

UML::ElementImpl* AndroidMetamodelPlugin::getGraphicalObject(QString const &/*diagram*/, QString const &element) const
{
	if (element == "List")
		return new UML::List();
	else if (element == "Object")
		return new UML::Object();
	else if (element == "Parameter")
		return new UML::Parameter();
	else if (element == "Tag")
		return new UML::Tag();
	else if (element == "Text")
		return new UML::Text();
	else if (element == "Variable")
		return new UML::Variable();
	else if (element == "CollectIn")
		return new UML::CollectIn();
	else if (element == "Enclose")
		return new UML::Enclose();
	else if (element == "SaveAs")
		return new UML::SaveAs();
	else if (element == "SaveIn")
		return new UML::SaveIn();
	else if (element == "Button")
		return new UML::Button();
	else if (element == "Check_Box")
		return new UML::Check_Box();
	else if (element == "Edit_Text")
		return new UML::Edit_Text();
	else if (element == "Frame_Layout")
		return new UML::Frame_Layout();
	else if (element == "Gallery")
		return new UML::Gallery();
	else if (element == "Grid_View")
		return new UML::Grid_View();
	else if (element == "Image")
		return new UML::Image();
	else if (element == "Linear_Layout")
		return new UML::Linear_Layout();
	else if (element == "List_View")
		return new UML::List_View();
	else if (element == "List_of_Text_Views")
		return new UML::List_of_Text_Views();
	else if (element == "Scroll_View")
		return new UML::Scroll_View();
	else if (element == "Spinner")
		return new UML::Spinner();
	else if (element == "Table_Layout")
		return new UML::Table_Layout();
	else if (element == "Table_Row")
		return new UML::Table_Row();
	else if (element == "Text_View")
		return new UML::Text_View();
	else if (element == "Contains")
		return new UML::Contains();
	else if (element == "FollowingBy")
		return new UML::FollowingBy();
	else if (element == "Begin")
		return new UML::Begin();
	else if (element == "Close_Dialog")
		return new UML::Close_Dialog();
	else if (element == "Create_Activity")
		return new UML::Create_Activity();
	else if (element == "Create_Bound_Service")
		return new UML::Create_Bound_Service();
	else if (element == "Create_Tab_Layout")
		return new UML::Create_Tab_Layout();
	else if (element == "Custom_Toast_Notification")
		return new UML::Custom_Toast_Notification();
	else if (element == "End")
		return new UML::End();
	else if (element == "Http_Request")
		return new UML::Http_Request();
	else if (element == "Service_Success")
		return new UML::Service_Success();
	else if (element == "Set_Content_View")
		return new UML::Set_Content_View();
	else if (element == "Show_Custom_Dialog")
		return new UML::Show_Custom_Dialog();
	else if (element == "Show_Progress_Wheel")
		return new UML::Show_Progress_Wheel();
	else if (element == "Show_Text_Dialog")
		return new UML::Show_Text_Dialog();
	else if (element == "Toast_Notification")
		return new UML::Toast_Notification();
	else if (element == "BackgroundFlow")
		return new UML::BackgroundFlow();
	else if (element == "Fail")
		return new UML::Fail();
	else if (element == "MainFlow")
		return new UML::MainFlow();
	else if (element == "Next")
		return new UML::Next();
	else if (element == "OnButtonClick")
		return new UML::OnButtonClick();
	else if (element == "OnClose")
		return new UML::OnClose();
	else if (element == "OnCreate")
		return new UML::OnCreate();
	else if (element == "OnDismiss")
		return new UML::OnDismiss();
	else if (element == "OnItemClick")
		return new UML::OnItemClick();
	else if (element == "Success")
		return new UML::Success();
	else if (element == "Tab")
		return new UML::Tab();
	else {
		Q_ASSERT(!"Request for creation of an element with unknown name");
		return NULL;
	}
}

bool AndroidMetamodelPlugin::isParentOf(QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const
{
	if (parentsMap[childDiagram][childElement].contains(qMakePair(parentDiagram, parentElement)))
		return true;
	typedef QPair<QString, QString> StringPair;
	foreach (StringPair const &pair, parentsMap[childDiagram][childElement])
		if (isParentOf(parentDiagram, parentElement, pair.first, pair.second))
			return true;
	return false;
}
QStringList AndroidMetamodelPlugin::getPropertyNames(QString const &/*diagram*/, QString const &element) const
{
	QStringList result;
	if (element == "List")
;
	else if (element == "Object")
;
	else if (element == "Parameter")
;
	else if (element == "Tag")
;
	else if (element == "Text")
;
	else if (element == "Variable")
;
	else if (element == "CollectIn")
;
	else if (element == "Enclose")
;
	else if (element == "SaveAs")
;
	else if (element == "SaveIn")
;
	else if (element == "Button")
		result  << "label" << "layoutHeight" << "layoutWidth" << "padding" << "variable";
	else if (element == "Check_Box")
		result  << "default" << "layoutHeight" << "layoutWidth" << "padding" << "text" << "variable";
	else if (element == "Edit_Text")
		result  << "isPassword" << "layoutHeight" << "layoutWidth" << "padding" << "variable";
	else if (element == "Frame_Layout")
		result  << "background" << "layoutHeight" << "layoutWidth" << "padding";
	else if (element == "Gallery")
		result  << "layoutHeight" << "layoutWidth" << "padding";
	else if (element == "Grid_View")
		result  << "columnWidth" << "horizontalSpacing" << "layoutHeight" << "layoutWidth" << "numColumns"
		 << "padding" << "stretchMode" << "verticalSpacing";
	else if (element == "Image")
		result  << "layoutHeight" << "layoutWidth" << "padding" << "scaleType" << "source";
	else if (element == "Linear_Layout")
		result  << "background" << "layoutHeight" << "layoutWidth" << "orientation" << "padding";
	else if (element == "List_View")
		result  << "layoutHeight" << "layoutWidth" << "padding";
	else if (element == "List_of_Text_Views")
		result  << "itemsContent" << "itemsIds" << "layoutHeight" << "layoutWidth" << "padding"
		 << "textViewLayoutHeight" << "textViewLayoutWidth";
	else if (element == "Scroll_View")
		result  << "layoutHeight" << "layoutWidth" << "padding";
	else if (element == "Spinner")
		result  << "itemsContent" << "itemsIds" << "layoutHeight" << "layoutWidth" << "padding";
	else if (element == "Table_Layout")
		result  << "layoutHeight" << "layoutWidth" << "padding";
	else if (element == "Table_Row")
		result  << "layoutHeight" << "layoutWidth" << "padding";
	else if (element == "Text_View")
		result  << "layoutHeight" << "layoutWidth" << "padding" << "text";
	else if (element == "Contains")
;
	else if (element == "FollowingBy")
;
	else if (element == "Begin")
;
	else if (element == "Close_Dialog")
;
	else if (element == "Create_Activity")
		result  << "label" << "layout";
	else if (element == "Create_Bound_Service")
;
	else if (element == "Create_Tab_Layout")
		result  << "deselectedIcons" << "selectedIcons" << "tabsIds" << "tabsText";
	else if (element == "Custom_Toast_Notification")
		result  << "duration" << "layout";
	else if (element == "End")
;
	else if (element == "Http_Request")
		result  << "handler" << "url";
	else if (element == "Service_Success")
;
	else if (element == "Set_Content_View")
		result  << "layout";
	else if (element == "Show_Custom_Dialog")
		result  << "isCancelable" << "layout" << "title";
	else if (element == "Show_Progress_Wheel")
;
	else if (element == "Show_Text_Dialog")
		result  << "isCancelable" << "negativeButton" << "neutralButton" << "positiveButton" << "text";
	else if (element == "Toast_Notification")
		result  << "duration" << "text";
	else if (element == "BackgroundFlow")
;
	else if (element == "Fail")
;
	else if (element == "MainFlow")
;
	else if (element == "Next")
;
	else if (element == "OnButtonClick")
		result  << "buttonName";
	else if (element == "OnClose")
;
	else if (element == "OnCreate")
;
	else if (element == "OnDismiss")
;
	else if (element == "OnItemClick")
		result  << "itemName";
	else if (element == "Success")
;
	else if (element == "Tab")
		result  << "tabId";
	return result;
}

QStringList AndroidMetamodelPlugin::getTypesContainedBy(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element);
	return result;
}

QStringList AndroidMetamodelPlugin::getConnectedTypes(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element);
	return result;
}

QStringList AndroidMetamodelPlugin::getUsedTypes(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element);
	return result;
}

QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > AndroidMetamodelPlugin::getPossibleEdges(QString const &element) const
{
	QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > result;
	if (element == "CollectIn")
		result << qMakePair(qMakePair(QString("Parameter"),QString("List")),qMakePair(true,QString("CollectIn"))) << qMakePair(qMakePair(QString("Object"),QString("List")),qMakePair(false,QString("CollectIn"))) << qMakePair(qMakePair(QString("Text"),QString("List")),qMakePair(false,QString("CollectIn")));
	else if (element == "Enclose")
		result << qMakePair(qMakePair(QString("Tag"),QString("Text")),qMakePair(true,QString("Enclose"))) << qMakePair(qMakePair(QString("Tag"),QString("Tag")),qMakePair(true,QString("Enclose"))) << qMakePair(qMakePair(QString("Tag"),QString("Parameter")),qMakePair(true,QString("Enclose")));
	else if (element == "SaveAs")
		result << qMakePair(qMakePair(QString("Tag"),QString("Object")),qMakePair(true,QString("SaveAs")));
	else if (element == "SaveIn")
		result << qMakePair(qMakePair(QString("Parameter"),QString("Variable")),qMakePair(true,QString("SaveIn"))) << qMakePair(qMakePair(QString("Object"),QString("Variable")),qMakePair(true,QString("SaveIn"))) << qMakePair(qMakePair(QString("Text"),QString("Variable")),qMakePair(true,QString("SaveIn")));
	else if (element == "OnCreate")
		result << qMakePair(qMakePair(QString("Create_Activity"),QString("Set_Content_View")),qMakePair(true,QString("OnCreate")));
	else if (element == "Tab")
		result << qMakePair(qMakePair(QString("Create_Tab_Layout"),QString("Create_Activity")),qMakePair(false,QString("Tab")));
	return result;
}

//(-1) means "edge", (+1) means "node"
int AndroidMetamodelPlugin::isNodeOrEdge(QString const &element) const
{
	if (element == "List")
		return 1;
	else if (element == "Object")
		return 1;
	else if (element == "Parameter")
		return 1;
	else if (element == "Tag")
		return 1;
	else if (element == "Text")
		return 1;
	else if (element == "Variable")
		return 1;
	else if (element == "CollectIn")
		return -1;
	else if (element == "Enclose")
		return -1;
	else if (element == "SaveAs")
		return -1;
	else if (element == "SaveIn")
		return -1;
	else if (element == "Button")
		return 1;
	else if (element == "Check_Box")
		return 1;
	else if (element == "Check_Value")
		return 0;
	else if (element == "Edit_Text")
		return 1;
	else if (element == "Frame_Layout")
		return 1;
	else if (element == "Gallery")
		return 1;
	else if (element == "Grid_View")
		return 1;
	else if (element == "Image")
		return 1;
	else if (element == "Layout_Value")
		return 0;
	else if (element == "Linear_Layout")
		return 1;
	else if (element == "List_View")
		return 1;
	else if (element == "List_of_Text_Views")
		return 1;
	else if (element == "Orientation_Value")
		return 0;
	else if (element == "Scale_Value")
		return 0;
	else if (element == "Scroll_View")
		return 1;
	else if (element == "Spinner")
		return 1;
	else if (element == "Stretch_Mode_Value")
		return 0;
	else if (element == "Table_Layout")
		return 1;
	else if (element == "Table_Row")
		return 1;
	else if (element == "Text_View")
		return 1;
	else if (element == "View")
		return 1;
	else if (element == "Contains")
		return -1;
	else if (element == "FollowingBy")
		return -1;
	else if (element == "Begin")
		return 1;
	else if (element == "Close_Dialog")
		return 1;
	else if (element == "Create_Activity")
		return 1;
	else if (element == "Create_Bound_Service")
		return 1;
	else if (element == "Create_Tab_Layout")
		return 1;
	else if (element == "Custom_Toast_Notification")
		return 1;
	else if (element == "End")
		return 1;
	else if (element == "Http_Request")
		return 1;
	else if (element == "Service_Success")
		return 1;
	else if (element == "Set_Content_View")
		return 1;
	else if (element == "Show_Custom_Dialog")
		return 1;
	else if (element == "Show_Progress_Wheel")
		return 1;
	else if (element == "Show_Text_Dialog")
		return 1;
	else if (element == "Toast_Duration_Value")
		return 0;
	else if (element == "Toast_Notification")
		return 1;
	else if (element == "BackgroundFlow")
		return -1;
	else if (element == "Fail")
		return -1;
	else if (element == "MainFlow")
		return -1;
	else if (element == "Next")
		return -1;
	else if (element == "OnButtonClick")
		return -1;
	else if (element == "OnClose")
		return -1;
	else if (element == "OnCreate")
		return -1;
	else if (element == "OnDismiss")
		return -1;
	else if (element == "OnItemClick")
		return -1;
	else if (element == "Success")
		return -1;
	else if (element == "Tab")
		return -1;
	return 0;
}
QStringList AndroidMetamodelPlugin::getEnumValues(QString name) const 
{
	QStringList result;
	if (name == "Layout_Value")
		result << QString::fromUtf8("fill_parent") << QString::fromUtf8("wrap_content");
	else if (name == "Orientation_Value")
		result << QString::fromUtf8("horizontal") << QString::fromUtf8("vertical");
	else if (name == "Toast_Duration_Value")
		result << QString::fromUtf8("long") << QString::fromUtf8("short");
	else if (name == "Check_Value")
		result << QString::fromUtf8("checked") << QString::fromUtf8("not_checked");
	else if (name == "Stretch_Mode_Value")
		result << QString::fromUtf8("no_stretch") << QString::fromUtf8("strectch_spacing") << QString::fromUtf8("stretch_spacing_uniform") << QString::fromUtf8("stretch_column_width");
	else if (name == "Scale_Value")
		result << QString::fromUtf8("matrix") << QString::fromUtf8("fit_XY") << QString::fromUtf8("fit_start") << QString::fromUtf8("fit_center") << QString::fromUtf8("fit_end") << QString::fromUtf8("center") << QString::fromUtf8("center_crop");
	return result;
}

QString AndroidMetamodelPlugin::getPropertyType(QString const &element, QString const &property) const
{
	return propertyTypes[element][property];
}

QString AndroidMetamodelPlugin::getPropertyDefaultValue(QString const &element, QString const &property) const
{
	return propertyDefault[element][property];
}

QList<qReal::ListenerInterface*> AndroidMetamodelPlugin::listeners() const
{
	QList<qReal::ListenerInterface*> result;
	return result;
}
