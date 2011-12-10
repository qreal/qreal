#include "pluginInterface.h"

#include "elements.h"


Q_EXPORT_PLUGIN2(qreal_editors, UbiqMetamodelPlugin)

UbiqMetamodelPlugin::UbiqMetamodelPlugin()
{
	initPlugin();
}

void UbiqMetamodelPlugin::initPlugin()
{
	initNameMap();
	initMouseGestureMap();
	initPropertyMap();
	initPropertyDefaultsMap();
	initDescriptionMap();
	initParentsMap();
}

void UbiqMetamodelPlugin::initNameMap()
{
	diagramNameMap["UbiqActivity"] = QString::fromUtf8("Ubiq Activity Diagram");
	diagramNodeNameMap["UbiqActivity"] = "ubiqActivityDiagram";

	diagramNameMap["UbiqDataStructures"] = QString::fromUtf8("Ubiq Data Structures");
	diagramNodeNameMap["UbiqDataStructures"] = "dataStructuresDiagram";

	diagramNameMap["UbiqMaster"] = QString::fromUtf8("Ubiq Master Diagram");
	diagramNodeNameMap["UbiqMaster"] = "ubiqMasterDiagram";

	elementsNameMap["UbiqActivity"]["Action"] = QString::fromUtf8("Action");
	elementsNameMap["UbiqActivity"]["ActivityFinalNode"] = QString::fromUtf8("Activity Final Node");
	elementsNameMap["UbiqActivity"]["ActualParameter"] = QString::fromUtf8("Actual Parameter");
	elementsNameMap["UbiqActivity"]["ControlFlow"] = QString::fromUtf8("Control Flow");
	elementsNameMap["UbiqActivity"]["DecisionNode"] = QString::fromUtf8("Decision Node");
	elementsNameMap["UbiqActivity"]["FormalParameter"] = QString::fromUtf8("Formal Parameter");
	elementsNameMap["UbiqActivity"]["FormalParameters"] = QString::fromUtf8("Function Signature");
	elementsNameMap["UbiqActivity"]["FunctionCall"] = QString::fromUtf8("Function Call");
	elementsNameMap["UbiqActivity"]["HandlerStart"] = QString::fromUtf8("Handler Start");
	elementsNameMap["UbiqActivity"]["InitialNode"] = QString::fromUtf8("Initial Node");
	elementsNameMap["UbiqActivity"]["MergeNode"] = QString::fromUtf8("Merge Node");
	elementsNameMap["UbiqActivity"]["ReturnAction"] = QString::fromUtf8("Return");
	elementsNameMap["UbiqActivity"]["ReturnValue"] = QString::fromUtf8("Return Value");
	elementsNameMap["UbiqActivity"]["UbiqActivityComment"] = QString::fromUtf8("Comment");
	elementsNameMap["UbiqActivity"]["UbiqActivityCommentLink"] = QString::fromUtf8("Comment Link");
	elementsNameMap["UbiqActivity"]["UbiqActivityDiagram"] = QString::fromUtf8("Activity Diagram");
	elementsNameMap["UbiqActivity"]["UbiqPackage"] = QString::fromUtf8("Package");
	elementsNameMap["UbiqDataStructures"]["Comment"] = QString::fromUtf8("comment");
	elementsNameMap["UbiqDataStructures"]["CommentLink"] = QString::fromUtf8("Comment Link");
	elementsNameMap["UbiqDataStructures"]["CustomClass"] = QString::fromUtf8("Custom Class");
	elementsNameMap["UbiqDataStructures"]["DataStructuresDiagram"] = QString::fromUtf8("Data Structures Diagram");
	elementsNameMap["UbiqDataStructures"]["EnumElement"] = QString::fromUtf8("Enum Element");
	elementsNameMap["UbiqDataStructures"]["ErrorCodes"] = QString::fromUtf8("Error Codes");
	elementsNameMap["UbiqDataStructures"]["Field"] = QString::fromUtf8("Field");
	elementsNameMap["UbiqDataStructures"]["MessageClass"] = QString::fromUtf8("Message Class");
	elementsNameMap["UbiqDataStructures"]["MessageCodes"] = QString::fromUtf8("Message Codes");
	elementsNameMap["UbiqMaster"]["Handler"] = QString::fromUtf8("Handler");
	elementsNameMap["UbiqMaster"]["MasterDiagramComment"] = QString::fromUtf8("masterDiagramComment");
	elementsNameMap["UbiqMaster"]["MasterDiagramCommentLink"] = QString::fromUtf8("Comment Link");
	elementsNameMap["UbiqMaster"]["MasterDiagramConstant"] = QString::fromUtf8("Constant");
	elementsNameMap["UbiqMaster"]["MasterDiagramField"] = QString::fromUtf8("Field");
	elementsNameMap["UbiqMaster"]["MasterNode"] = QString::fromUtf8("Master Node");
	elementsNameMap["UbiqMaster"]["Preprocessor"] = QString::fromUtf8("Preprocessor");
	elementsNameMap["UbiqMaster"]["UbiqMasterDiagram"] = QString::fromUtf8("Master Diagram");
}

void UbiqMetamodelPlugin::initMouseGestureMap()
{
	elementMouseGesturesMap["UbiqActivity"]["ActivityFinalNode"] = "29, 15 : 27, 20 "
			": 24, 24 : 20, 27 : 15, 28 : 10, 27 : 6, 24 : 3, 20 : 2, 15 : 3, 10 : 6, 6 : 10,"
			" 3 : 15, 2 : 20, 3 : 24, 6 : 27, 10 : 29, 15 : 36, 15 : 34, 23 : 29, 29 : 23, 34"
			" : 15, 35 : 7, 34 : 1, 29 : -4, 23 : -5, 15 : -4, 7 : 1, 1 : 7, -4 : 15, -5 : 23"
			", -4 : 29, 1 : 34, 7 : 36, 15 : ";
	elementMouseGesturesMap["UbiqActivity"]["DecisionNode"] = "30, 2 : 58, 30 : 30, "
			"58 : 2, 30 : 30, 2 : ";
	elementMouseGesturesMap["UbiqActivity"]["HandlerStart"] = "0, 0 : 100, 0 : 100, "
			"60 : 0, 60 : 15, 30 : 0, 0 : ";
	elementMouseGesturesMap["UbiqActivity"]["ReturnAction"] = "0, 0 : 84, 0 : 98, 30"
			" : 84, 58 : 0, 58 : 0, 0 : ";
	elementMouseGesturesMap["UbiqActivity"]["UbiqActivityDiagram"] = "10, 0 : 90, 0 "
			": 90, 100 : 10, 100 : 0, 90 : 0, 10 : 100, 10 : 100, 90 : ";
	elementMouseGesturesMap["UbiqDataStructures"]["CustomClass"] = "0, 0 : 75, 0 : 7"
			"5, 50 : 0, 50 : ";
	elementMouseGesturesMap["UbiqDataStructures"]["DataStructuresDiagram"] = "10, 0 "
			": 90, 0 : 90, 100 : 10, 100 : 0, 90 : 0, 10 : 100, 10 : 100, 90 : ";
	elementMouseGesturesMap["UbiqMaster"]["UbiqMasterDiagram"] = "10, 0 : 90, 0 : 90"
			", 100 : 10, 100 : 0, 90 : 0, 10 : 100, 10 : 100, 90 : ";
}

void UbiqMetamodelPlugin::initPropertyMap()
{
	propertyTypes["UbiqActivityComment"]["annotatedElement"] = "KernelElement";
	propertyTypes["UbiqActivityComment"]["ownedComment"] = "Comment";
	propertyTypes["Comment"]["annotatedElement"] = "KernelElement";
	propertyTypes["Comment"]["ownedComment"] = "Comment";
	propertyTypes["Field"]["serialize"] = "Bool";
	propertyTypes["Field"]["serializeAsShort"] = "Bool";
	propertyTypes["MasterDiagramComment"]["annotatedElement"] = "KernelElement";
	propertyTypes["MasterDiagramComment"]["ownedComment"] = "Comment";
	propertyTypes["MasterDiagramField"]["static"] = "Bool";
}

void UbiqMetamodelPlugin::initPropertyDefaultsMap()
{
}

void UbiqMetamodelPlugin::initDescriptionMap()
{
}

void UbiqMetamodelPlugin::initParentsMap()
{
	parentsMap["UbiqActivity"]["Action"]
		<< qMakePair(QString("UbiqActivity"), QString("AbstractActivityNode"))
	;
	parentsMap["UbiqActivity"]["ActivityFinalNode"]
		<< qMakePair(QString("UbiqActivity"), QString("AbstractActivityNode"))
	;
	parentsMap["UbiqActivity"]["DecisionNode"]
		<< qMakePair(QString("UbiqActivity"), QString("AbstractActivityNode"))
	;
	parentsMap["UbiqActivity"]["FormalParameters"]
		<< qMakePair(QString("UbiqActivity"), QString("AbstractActivityNode"))
	;
	parentsMap["UbiqActivity"]["FunctionCall"]
		<< qMakePair(QString("UbiqActivity"), QString("AbstractActivityNode"))
	;
	parentsMap["UbiqActivity"]["HandlerStart"]
		<< qMakePair(QString("UbiqActivity"), QString("AbstractActivityNode"))
	;
	parentsMap["UbiqActivity"]["InitialNode"]
		<< qMakePair(QString("UbiqActivity"), QString("AbstractActivityNode"))
	;
	parentsMap["UbiqActivity"]["MergeNode"]
		<< qMakePair(QString("UbiqActivity"), QString("AbstractActivityNode"))
	;
	parentsMap["UbiqActivity"]["ReturnAction"]
		<< qMakePair(QString("UbiqActivity"), QString("AbstractActivityNode"))
	;
	parentsMap["UbiqActivity"]["UbiqActivityComment"]
		<< qMakePair(QString("UbiqActivity"), QString("KernelElement"))
	;
	parentsMap["UbiqDataStructures"]["Comment"]
		<< qMakePair(QString("UbiqDataStructures"), QString("KernelElement"))
	;
	parentsMap["UbiqMaster"]["MasterDiagramComment"]
		<< qMakePair(QString("UbiqMaster"), QString("KernelElement"))
	;
}

QStringList UbiqMetamodelPlugin::diagrams() const
{
	return diagramNameMap.keys();
}

QStringList UbiqMetamodelPlugin::elements(QString const &diagram) const
{
	return elementsNameMap[diagram].keys();
}

QStringList UbiqMetamodelPlugin::getPropertiesWithDefaultValues(QString const &element) const
{
	return propertyDefault[element].keys();
}

QIcon UbiqMetamodelPlugin::getIcon(SdfIconEngineV2Interface *engine) const
{
	return QIcon(engine);
}

QString UbiqMetamodelPlugin::editorName() const
{
	 return "UbiqMetamodel Editor";
}

QString UbiqMetamodelPlugin::diagramName(QString const &diagram) const
{
	return diagramNameMap[diagram];
}

QString UbiqMetamodelPlugin::diagramNodeName(QString const &diagram) const
{
	return diagramNodeNameMap[diagram];
}

QString UbiqMetamodelPlugin::elementName(QString const &diagram, QString const &element) const
{
	return elementsNameMap[diagram][element];
}

QString UbiqMetamodelPlugin::elementDescription(QString const &diagram, QString const &element) const
{
	return elementsDescriptionMap[diagram][element];
}

QString UbiqMetamodelPlugin::propertyDescription(QString const &diagram, QString const &element, QString const &property) const
{
	return propertiesDescriptionMap[diagram][element][property];
}

QString UbiqMetamodelPlugin::propertyDisplayedName(QString const &diagram, QString const &element, QString const &property) const
{
	return propertiesDisplayedNamesMap[diagram][element][property];
}

QString UbiqMetamodelPlugin::elementMouseGesture(QString const &diagram, QString const &element) const
{
	return elementMouseGesturesMap[diagram][element];
}

ElementImpl* UbiqMetamodelPlugin::getGraphicalObject(QString const &/*diagram*/, QString const &element) const
{
	if (element == "Action")
		return new Action();
	else if (element == "ActivityFinalNode")
		return new ActivityFinalNode();
	else if (element == "ActualParameter")
		return new ActualParameter();
	else if (element == "ControlFlow")
		return new ControlFlow();
	else if (element == "DecisionNode")
		return new DecisionNode();
	else if (element == "FormalParameter")
		return new FormalParameter();
	else if (element == "FormalParameters")
		return new FormalParameters();
	else if (element == "FunctionCall")
		return new FunctionCall();
	else if (element == "HandlerStart")
		return new HandlerStart();
	else if (element == "InitialNode")
		return new InitialNode();
	else if (element == "MergeNode")
		return new MergeNode();
	else if (element == "ReturnAction")
		return new ReturnAction();
	else if (element == "ReturnValue")
		return new ReturnValue();
	else if (element == "UbiqActivityComment")
		return new UbiqActivityComment();
	else if (element == "UbiqActivityCommentLink")
		return new UbiqActivityCommentLink();
	else if (element == "UbiqActivityDiagram")
		return new UbiqActivityDiagram();
	else if (element == "UbiqPackage")
		return new UbiqPackage();
	else if (element == "Comment")
		return new Comment();
	else if (element == "CommentLink")
		return new CommentLink();
	else if (element == "CustomClass")
		return new CustomClass();
	else if (element == "DataStructuresDiagram")
		return new DataStructuresDiagram();
	else if (element == "EnumElement")
		return new EnumElement();
	else if (element == "ErrorCodes")
		return new ErrorCodes();
	else if (element == "Field")
		return new Field();
	else if (element == "MessageClass")
		return new MessageClass();
	else if (element == "MessageCodes")
		return new MessageCodes();
	else if (element == "Handler")
		return new Handler();
	else if (element == "MasterDiagramComment")
		return new MasterDiagramComment();
	else if (element == "MasterDiagramCommentLink")
		return new MasterDiagramCommentLink();
	else if (element == "MasterDiagramConstant")
		return new MasterDiagramConstant();
	else if (element == "MasterDiagramField")
		return new MasterDiagramField();
	else if (element == "MasterNode")
		return new MasterNode();
	else if (element == "Preprocessor")
		return new Preprocessor();
	else if (element == "UbiqMasterDiagram")
		return new UbiqMasterDiagram();
	else {
		Q_ASSERT(!"Request for creation of an element with unknown name");
		return NULL;
	}
}

bool UbiqMetamodelPlugin::isParentOf(QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const
{
	if (parentsMap[childDiagram][childElement].contains(qMakePair(parentDiagram, parentElement)))
		return true;
	typedef QPair<QString, QString> StringPair;
	foreach (StringPair const &pair, parentsMap[childDiagram][childElement])
		if (isParentOf(parentDiagram, parentElement, pair.first, pair.second))
			return true;
	return false;
}
QList<QPair<QString, QString> > UbiqMetamodelPlugin::getParentsOf(QString const &diagram, QString const &element) const
{
	return parentsMap[diagram][element];
}
QStringList UbiqMetamodelPlugin::getPropertyNames(QString const &/*diagram*/, QString const &element) const
{
	QStringList result;
	if (element == "Action")
;
	else if (element == "ActivityFinalNode")
;
	else if (element == "ActualParameter")
;
	else if (element == "ControlFlow")
		result  << "guard";
	else if (element == "DecisionNode")
;
	else if (element == "FormalParameter")
		result  << "type";
	else if (element == "FormalParameters")
		result  << "returnType";
	else if (element == "FunctionCall")
;
	else if (element == "HandlerStart")
;
	else if (element == "InitialNode")
;
	else if (element == "MergeNode")
;
	else if (element == "ReturnAction")
;
	else if (element == "ReturnValue")
		result  << "type";
	else if (element == "UbiqActivityComment")
		result  << "annotatedElement" << "body" << "ownedComment";
	else if (element == "UbiqActivityCommentLink")
;
	else if (element == "UbiqActivityDiagram")
;
	else if (element == "UbiqPackage")
;
	else if (element == "Comment")
		result  << "annotatedElement" << "body" << "ownedComment";
	else if (element == "CommentLink")
;
	else if (element == "CustomClass")
;
	else if (element == "DataStructuresDiagram")
;
	else if (element == "EnumElement")
		result  << "value";
	else if (element == "ErrorCodes")
;
	else if (element == "Field")
		result  << "defaultValue" << "serialize" << "serializeAsShort" << "type";
	else if (element == "MessageClass")
;
	else if (element == "MessageCodes")
;
	else if (element == "Handler")
		result  << "MessageInputParameter";
	else if (element == "MasterDiagramComment")
		result  << "annotatedElement" << "body" << "ownedComment";
	else if (element == "MasterDiagramCommentLink")
;
	else if (element == "MasterDiagramConstant")
		result  << "type" << "value";
	else if (element == "MasterDiagramField")
		result  << "defaultValue" << "static" << "type";
	else if (element == "MasterNode")
		result  << "initCode" << "onTcpIpCloseHandler";
	else if (element == "Preprocessor")
;
	else if (element == "UbiqMasterDiagram")
;
	return result;
}

QStringList UbiqMetamodelPlugin::getTypesContainedBy(QString const &element) const
{
	QStringList result;
	if (element == "FormalParameters")
		result << "FormalParameter" ;
	else if (element == "FunctionCall")
		result << "ActualParameter" << "ReturnValue" ;
	else if (element == "UbiqActivityDiagram")
		result << "UbiqActivityComment" << "AbstractActivityNode" ;
	else if (element == "UbiqPackage")
		result << "UbiqActivityComment" << "UbiqActivityDiagram" ;
	else if (element == "CustomClass")
		result << "Field" ;
	else if (element == "DataStructuresDiagram")
		result << "MessageClass" << "CustomClass" << "ErrorCodes" << "MessageCodes" << "Comment" ;
	else if (element == "ErrorCodes")
		result << "EnumElement" ;
	else if (element == "MessageClass")
		result << "Field" ;
	else if (element == "MessageCodes")
		result << "EnumElement" ;
	else if (element == "MasterNode")
		result << "Handler" << "MasterDiagramField" << "MasterDiagramConstant" << "Preprocessor" ;
	else if (element == "UbiqMasterDiagram")
		result << "MasterNode" << "MasterDiagramComment" ;
	return result;
}

QStringList UbiqMetamodelPlugin::getConnectedTypes(QString const &element) const
{
	QStringList result;
	if (element == "MasterNode")
		result << "UbiqActivityDiagram" ;
	return result;
}

QStringList UbiqMetamodelPlugin::getUsedTypes(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element);
	return result;
}

QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > UbiqMetamodelPlugin::getPossibleEdges(QString const &element) const
{
	QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > result;
	if (element == "ControlFlow")
		result << qMakePair(qMakePair(QString("AbstractActivityNode"),QString("AbstractActivityNode")),qMakePair(false,QString("ControlFlow")));
	return result;
}

//(-1) means "edge", (+1) means "node"
int UbiqMetamodelPlugin::isNodeOrEdge(QString const &element) const
{
	if (element == "AbstractActivityNode")
		return 1;
	else if (element == "Action")
		return 1;
	else if (element == "ActivityFinalNode")
		return 1;
	else if (element == "ActualParameter")
		return 1;
	else if (element == "ControlFlow")
		return -1;
	else if (element == "DecisionNode")
		return 1;
	else if (element == "FormalParameter")
		return 1;
	else if (element == "FormalParameters")
		return 1;
	else if (element == "FunctionCall")
		return 1;
	else if (element == "HandlerStart")
		return 1;
	else if (element == "InitialNode")
		return 1;
	else if (element == "MergeNode")
		return 1;
	else if (element == "ReturnAction")
		return 1;
	else if (element == "ReturnValue")
		return 1;
	else if (element == "UbiqActivityComment")
		return 1;
	else if (element == "UbiqActivityCommentLink")
		return -1;
	else if (element == "UbiqActivityDiagram")
		return 1;
	else if (element == "UbiqPackage")
		return 1;
	else if (element == "Comment")
		return 1;
	else if (element == "CommentLink")
		return -1;
	else if (element == "CustomClass")
		return 1;
	else if (element == "DataStructuresDiagram")
		return 1;
	else if (element == "EnumElement")
		return 1;
	else if (element == "ErrorCodes")
		return 1;
	else if (element == "Field")
		return 1;
	else if (element == "MessageClass")
		return 1;
	else if (element == "MessageCodes")
		return 1;
	else if (element == "Handler")
		return 1;
	else if (element == "MasterDiagramComment")
		return 1;
	else if (element == "MasterDiagramCommentLink")
		return -1;
	else if (element == "MasterDiagramConstant")
		return 1;
	else if (element == "MasterDiagramField")
		return 1;
	else if (element == "MasterNode")
		return 1;
	else if (element == "Preprocessor")
		return 1;
	else if (element == "UbiqMasterDiagram")
		return 1;
	return 0;
}
QStringList UbiqMetamodelPlugin::getEnumValues(QString name) const 
{
	QStringList result;
	if (name == "ExpansionKind")
		result << QString::fromUtf8("parallel") << QString::fromUtf8("iterative") << QString::fromUtf8("stream");
	else if (name == "Type")
		result << QString::fromUtf8("localPrecondition") << QString::fromUtf8("localPostCondation");
	else if (name == "AggregationKind")
		result << QString::fromUtf8("none") << QString::fromUtf8("shared") << QString::fromUtf8("composite");
	else if (name == "CallConcurrencyKind")
		result << QString::fromUtf8("sequential") << QString::fromUtf8("guarded") << QString::fromUtf8("concurrent");
	else if (name == "Bool")
		result << QString::fromUtf8("false") << QString::fromUtf8("true");
	else if (name == "ObjectNodeOrderingKind")
		result << QString::fromUtf8("unordered") << QString::fromUtf8("ordered") << QString::fromUtf8("LIFO") << QString::fromUtf8("FIFO");
	else if (name == "ParameterDirectionKind")
		result << QString::fromUtf8("in") << QString::fromUtf8("inout") << QString::fromUtf8("out") << QString::fromUtf8("return");
	else if (name == "VisibilityKind")
		result << QString::fromUtf8("private") << QString::fromUtf8("protected") << QString::fromUtf8("public") << QString::fromUtf8("package");
	else if (name == "ParameterEffectKind")
		result << QString::fromUtf8("create") << QString::fromUtf8("read") << QString::fromUtf8("update") << QString::fromUtf8("delete");
	return result;
}

QString UbiqMetamodelPlugin::getPropertyType(QString const &element, QString const &property) const
{
	return propertyTypes[element][property];
}

QString UbiqMetamodelPlugin::getPropertyDefaultValue(QString const &element, QString const &property) const
{
	return propertyDefault[element][property];
}

QList<qReal::ListenerInterface*> UbiqMetamodelPlugin::listeners() const
{
	QList<qReal::ListenerInterface*> result;
	return result;
}
