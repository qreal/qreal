#include "pluginInterface.h"

#include "elements.h"


Q_EXPORT_PLUGIN2(qreal_editors, RobotsMetamodelPlugin)

RobotsMetamodelPlugin::RobotsMetamodelPlugin()
{
	initPlugin();
}

void RobotsMetamodelPlugin::initPlugin()
{
	initNameMap();
	initMouseGestureMap();
	initPropertyMap();
	initPropertyDefaultsMap();
	initDescriptionMap();
	initParentsMap();
}

void RobotsMetamodelPlugin::initNameMap()
{
	diagramNameMap["RobotsDiagram"] = QString::fromUtf8("Диаграмма поведения робота");
	diagramNodeNameMap["RobotsDiagram"] = "RobotsDiagramNode";

	elementsNameMap["RobotsDiagram"]["Beep"] = QString::fromUtf8("Гудок");
	elementsNameMap["RobotsDiagram"]["ControlFlow"] = QString::fromUtf8("Поток управления");
	elementsNameMap["RobotsDiagram"]["EnginesBackward"] = QString::fromUtf8("Моторы назад");
	elementsNameMap["RobotsDiagram"]["EnginesForward"] = QString::fromUtf8("Моторы вперёд");
	elementsNameMap["RobotsDiagram"]["EnginesStop"] = QString::fromUtf8("Моторы стоп");
	elementsNameMap["RobotsDiagram"]["FinalNode"] = QString::fromUtf8("Конец");
	elementsNameMap["RobotsDiagram"]["Fork"] = QString::fromUtf8("Породить поток");
	elementsNameMap["RobotsDiagram"]["Function"] = QString::fromUtf8("Функция");
	elementsNameMap["RobotsDiagram"]["IfBlock"] = QString::fromUtf8("Условие");
	elementsNameMap["RobotsDiagram"]["InitialBlock"] = QString::fromUtf8("Блок инициализации");
	elementsNameMap["RobotsDiagram"]["InitialNode"] = QString::fromUtf8("Начало");
	elementsNameMap["RobotsDiagram"]["Loop"] = QString::fromUtf8("Цикл");
	elementsNameMap["RobotsDiagram"]["NullificationEncoder"] = QString::fromUtf8("Сбросить показания энкодера");
	elementsNameMap["RobotsDiagram"]["PlayTone"] = QString::fromUtf8("Играть звук");
	elementsNameMap["RobotsDiagram"]["RobotsDiagramNode"] = QString::fromUtf8("Диаграмма поведения робота");
	elementsNameMap["RobotsDiagram"]["Timer"] = QString::fromUtf8("Таймер");
	elementsNameMap["RobotsDiagram"]["WaitForColor"] = QString::fromUtf8("Ждать цвет");
	elementsNameMap["RobotsDiagram"]["WaitForColorIntensity"] = QString::fromUtf8("Ждать интенсивность цвета");
	elementsNameMap["RobotsDiagram"]["WaitForEncoder"] = QString::fromUtf8("Ждать энкодер");
	elementsNameMap["RobotsDiagram"]["WaitForSonarDistance"] = QString::fromUtf8("Ждать сонар");
	elementsNameMap["RobotsDiagram"]["WaitForTouchSensor"] = QString::fromUtf8("Ждать сенсор касания");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Beep"]["WaitForCompletion"] = QString::fromUtf8("Ждать завершения");
	propertiesDisplayedNamesMap["RobotsDiagram"]["ControlFlow"]["Guard"] = QString::fromUtf8("Условие");
	propertiesDisplayedNamesMap["RobotsDiagram"]["EnginesBackward"]["Ports"] = QString::fromUtf8("Порты");
	propertiesDisplayedNamesMap["RobotsDiagram"]["EnginesBackward"]["Power"] = QString::fromUtf8("Мощность (%)");
	propertiesDisplayedNamesMap["RobotsDiagram"]["EnginesBackward"]["TachoLimit"] = QString::fromUtf8("Предел оборотов (гр.)");
	propertiesDisplayedNamesMap["RobotsDiagram"]["EnginesForward"]["Ports"] = QString::fromUtf8("Порты");
	propertiesDisplayedNamesMap["RobotsDiagram"]["EnginesForward"]["Power"] = QString::fromUtf8("Мощность (%)");
	propertiesDisplayedNamesMap["RobotsDiagram"]["EnginesForward"]["TachoLimit"] = QString::fromUtf8("Предел оборотов (гр.)");
	propertiesDisplayedNamesMap["RobotsDiagram"]["EnginesStop"]["Ports"] = QString::fromUtf8("Порты");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Function"]["Body"] = QString::fromUtf8("Тело функции");
	propertiesDisplayedNamesMap["RobotsDiagram"]["IfBlock"]["Condition"] = QString::fromUtf8("Условие");
	propertiesDisplayedNamesMap["RobotsDiagram"]["InitialBlock"]["port_1"] = QString::fromUtf8("Порт 1");
	propertiesDisplayedNamesMap["RobotsDiagram"]["InitialBlock"]["port_2"] = QString::fromUtf8("Порт 2");
	propertiesDisplayedNamesMap["RobotsDiagram"]["InitialBlock"]["port_3"] = QString::fromUtf8("Порт 3");
	propertiesDisplayedNamesMap["RobotsDiagram"]["InitialBlock"]["port_4"] = QString::fromUtf8("Порт 4");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Loop"]["Iterations"] = QString::fromUtf8("Итерации");
	propertiesDisplayedNamesMap["RobotsDiagram"]["NullificationEncoder"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["PlayTone"]["Duration"] = QString::fromUtf8("Длительность");
	propertiesDisplayedNamesMap["RobotsDiagram"]["PlayTone"]["Frequency"] = QString::fromUtf8("Частота");
	propertiesDisplayedNamesMap["RobotsDiagram"]["PlayTone"]["WaitForCompletion"] = QString::fromUtf8("Ждать завершения");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Timer"]["Delay"] = QString::fromUtf8("Задержка");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForColor"]["Color"] = QString::fromUtf8("Цвет");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForColor"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForColorIntensity"]["Intensity"] = QString::fromUtf8("Интенсивность");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForColorIntensity"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForColorIntensity"]["Sign"] = QString::fromUtf8("Считанное значение");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForEncoder"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForEncoder"]["TachoLimit"] = QString::fromUtf8("Предел оборотов");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForSonarDistance"]["Distance"] = QString::fromUtf8("Дистанция");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForSonarDistance"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForSonarDistance"]["Sign"] = QString::fromUtf8("Считанное значение");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForTouchSensor"]["Port"] = QString::fromUtf8("Порт");
}

void RobotsMetamodelPlugin::initMouseGestureMap()
{
	elementMouseGesturesMap["RobotsDiagram"]["RobotsDiagramNode"] = "10, 0 : 90, 0 :"
			" 90, 100 : 10, 100 : 0, 90 : 0, 10 : 100, 10 : 100, 90 : ";
}

void RobotsMetamodelPlugin::initPropertyMap()
{
	propertyTypes["Beep"]["WaitForCompletion"] = "Bool";
	propertyTypes["ControlFlow"]["Guard"] = "GuardType";
	propertyTypes["InitialBlock"]["port_1"] = "Sensors";
	propertyTypes["InitialBlock"]["port_2"] = "Sensors";
	propertyTypes["InitialBlock"]["port_3"] = "Sensors";
	propertyTypes["InitialBlock"]["port_4"] = "Sensors";
	propertyTypes["PlayTone"]["WaitForCompletion"] = "Bool";
	propertyTypes["RobotsDiagramNode"]["isView"] = "Bool";
	propertyTypes["WaitForColor"]["Color"] = "Color";
	propertyTypes["WaitForColor"]["Port"] = "SensorPort";
	propertyTypes["WaitForColorIntensity"]["Port"] = "SensorPort";
	propertyTypes["WaitForColorIntensity"]["Sign"] = "DistanceSign";
	propertyTypes["WaitForSonarDistance"]["Port"] = "SensorPort";
	propertyTypes["WaitForSonarDistance"]["Sign"] = "DistanceSign";
	propertyTypes["WaitForTouchSensor"]["Port"] = "SensorPort";
}

void RobotsMetamodelPlugin::initPropertyDefaultsMap()
{
	propertyDefault["Beep"]["WaitForCompletion"] = QString::fromUtf8("true");
	propertyDefault["EnginesBackward"]["Ports"] = QString::fromUtf8("A, B");
	propertyDefault["EnginesBackward"]["Power"] = QString::fromUtf8("100");
	propertyDefault["EnginesBackward"]["TachoLimit"] = QString::fromUtf8("0");
	propertyDefault["EnginesForward"]["Ports"] = QString::fromUtf8("A, B");
	propertyDefault["EnginesForward"]["Power"] = QString::fromUtf8("100");
	propertyDefault["EnginesForward"]["TachoLimit"] = QString::fromUtf8("0");
	propertyDefault["EnginesStop"]["Ports"] = QString::fromUtf8("A, B");
	propertyDefault["IfBlock"]["Condition"] = QString::fromUtf8("0");
	propertyDefault["InitialBlock"]["port_1"] = QString::fromUtf8("Не используется");
	propertyDefault["InitialBlock"]["port_2"] = QString::fromUtf8("Не используется");
	propertyDefault["InitialBlock"]["port_3"] = QString::fromUtf8("Не используется");
	propertyDefault["InitialBlock"]["port_4"] = QString::fromUtf8("Не используется");
	propertyDefault["Loop"]["Iterations"] = QString::fromUtf8("10");
	propertyDefault["PlayTone"]["Duration"] = QString::fromUtf8("1000");
	propertyDefault["PlayTone"]["Frequency"] = QString::fromUtf8("1000");
	propertyDefault["PlayTone"]["WaitForCompletion"] = QString::fromUtf8("true");
	propertyDefault["Timer"]["Delay"] = QString::fromUtf8("0");
	propertyDefault["WaitForColor"]["Port"] = QString::fromUtf8("1");
	propertyDefault["WaitForColorIntensity"]["Intensity"] = QString::fromUtf8("0");
	propertyDefault["WaitForColorIntensity"]["Port"] = QString::fromUtf8("1");
	propertyDefault["WaitForColorIntensity"]["Sign"] = QString::fromUtf8("равно");
	propertyDefault["WaitForSonarDistance"]["Distance"] = QString::fromUtf8("0");
	propertyDefault["WaitForSonarDistance"]["Port"] = QString::fromUtf8("1");
	propertyDefault["WaitForSonarDistance"]["Sign"] = QString::fromUtf8("равно");
	propertyDefault["WaitForTouchSensor"]["Port"] = QString::fromUtf8("1");
}

void RobotsMetamodelPlugin::initDescriptionMap()
{
}

void RobotsMetamodelPlugin::initParentsMap()
{
	parentsMap["RobotsDiagram"]["Beep"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["EngineCommand"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["EngineMovementCommand"]
		<< qMakePair(QString("RobotsDiagram"), QString("EngineCommand"))
	;
	parentsMap["RobotsDiagram"]["EnginesBackward"]
		<< qMakePair(QString("RobotsDiagram"), QString("EngineMovementCommand"))
	;
	parentsMap["RobotsDiagram"]["EnginesForward"]
		<< qMakePair(QString("RobotsDiagram"), QString("EngineMovementCommand"))
	;
	parentsMap["RobotsDiagram"]["EnginesStop"]
		<< qMakePair(QString("RobotsDiagram"), QString("EngineCommand"))
	;
	parentsMap["RobotsDiagram"]["FinalNode"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["Fork"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["Function"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["IfBlock"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["InitialBlock"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["InitialNode"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["Loop"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["NullificationEncoder"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["PlayTone"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["RobotsDiagramNode"]
		<< qMakePair(QString("RobotsDiagram"), QString("Diagram"))
	;
	parentsMap["RobotsDiagram"]["SensorBlock"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["Timer"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["WaitForColor"]
		<< qMakePair(QString("RobotsDiagram"), QString("SensorBlock"))
	;
	parentsMap["RobotsDiagram"]["WaitForColorIntensity"]
		<< qMakePair(QString("RobotsDiagram"), QString("SensorBlock"))
	;
	parentsMap["RobotsDiagram"]["WaitForEncoder"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["WaitForSonarDistance"]
		<< qMakePair(QString("RobotsDiagram"), QString("SensorBlock"))
	;
	parentsMap["RobotsDiagram"]["WaitForTouchSensor"]
		<< qMakePair(QString("RobotsDiagram"), QString("SensorBlock"))
	;
}

QStringList RobotsMetamodelPlugin::diagrams() const
{
	return diagramNameMap.keys();
}

QStringList RobotsMetamodelPlugin::elements(QString const &diagram) const
{
	return elementsNameMap[diagram].keys();
}

QStringList RobotsMetamodelPlugin::getPropertiesWithDefaultValues(QString const &element) const
{
	return propertyDefault[element].keys();
}

QIcon RobotsMetamodelPlugin::getIcon(SdfIconEngineV2Interface *engine) const
{
	return QIcon(engine);
}

QString RobotsMetamodelPlugin::editorName() const
{
	 return "RobotsMetamodel Editor";
}

QString RobotsMetamodelPlugin::diagramName(QString const &diagram) const
{
	return diagramNameMap[diagram];
}

QString RobotsMetamodelPlugin::diagramNodeName(QString const &diagram) const
{
	return diagramNodeNameMap[diagram];
}

QString RobotsMetamodelPlugin::elementName(QString const &diagram, QString const &element) const
{
	return elementsNameMap[diagram][element];
}

QString RobotsMetamodelPlugin::elementDescription(QString const &diagram, QString const &element) const
{
	return elementsDescriptionMap[diagram][element];
}

QString RobotsMetamodelPlugin::propertyDescription(QString const &diagram, QString const &element, QString const &property) const
{
	return propertiesDescriptionMap[diagram][element][property];
}

QString RobotsMetamodelPlugin::propertyDisplayedName(QString const &diagram, QString const &element, QString const &property) const
{
	return propertiesDisplayedNamesMap[diagram][element][property];
}

QString RobotsMetamodelPlugin::elementMouseGesture(QString const &diagram, QString const &element) const
{
	return elementMouseGesturesMap[diagram][element];
}

ElementImpl* RobotsMetamodelPlugin::getGraphicalObject(QString const &/*diagram*/, QString const &element) const
{
	if (element == "Beep")
		return new Beep();
	else if (element == "ControlFlow")
		return new ControlFlow();
	else if (element == "EnginesBackward")
		return new EnginesBackward();
	else if (element == "EnginesForward")
		return new EnginesForward();
	else if (element == "EnginesStop")
		return new EnginesStop();
	else if (element == "FinalNode")
		return new FinalNode();
	else if (element == "Fork")
		return new Fork();
	else if (element == "Function")
		return new Function();
	else if (element == "IfBlock")
		return new IfBlock();
	else if (element == "InitialBlock")
		return new InitialBlock();
	else if (element == "InitialNode")
		return new InitialNode();
	else if (element == "Loop")
		return new Loop();
	else if (element == "NullificationEncoder")
		return new NullificationEncoder();
	else if (element == "PlayTone")
		return new PlayTone();
	else if (element == "RobotsDiagramNode")
		return new RobotsDiagramNode();
	else if (element == "Timer")
		return new Timer();
	else if (element == "WaitForColor")
		return new WaitForColor();
	else if (element == "WaitForColorIntensity")
		return new WaitForColorIntensity();
	else if (element == "WaitForEncoder")
		return new WaitForEncoder();
	else if (element == "WaitForSonarDistance")
		return new WaitForSonarDistance();
	else if (element == "WaitForTouchSensor")
		return new WaitForTouchSensor();
	else {
		Q_ASSERT(!"Request for creation of an element with unknown name");
		return NULL;
	}
}

bool RobotsMetamodelPlugin::isParentOf(QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const
{
	if (parentsMap[childDiagram][childElement].contains(qMakePair(parentDiagram, parentElement)))
		return true;
	typedef QPair<QString, QString> StringPair;
	foreach (StringPair const &pair, parentsMap[childDiagram][childElement])
		if (isParentOf(parentDiagram, parentElement, pair.first, pair.second))
			return true;
	return false;
}
QList<QPair<QString, QString> > RobotsMetamodelPlugin::getParentsOf(QString const &diagram, QString const &element) const
{
	return parentsMap[diagram][element];
}
QStringList RobotsMetamodelPlugin::getPropertyNames(QString const &/*diagram*/, QString const &element) const
{
	QStringList result;
	if (element == "Beep")
		result  << "WaitForCompletion";
	else if (element == "ControlFlow")
		result  << "Guard";
	else if (element == "EnginesBackward")
		result  << "Ports" << "Power" << "TachoLimit";
	else if (element == "EnginesForward")
		result  << "Ports" << "Power" << "TachoLimit";
	else if (element == "EnginesStop")
		result  << "Ports";
	else if (element == "FinalNode")
;
	else if (element == "Fork")
;
	else if (element == "Function")
		result  << "Body";
	else if (element == "IfBlock")
		result  << "Condition";
	else if (element == "InitialBlock")
		result  << "port_1" << "port_2" << "port_3" << "port_4";
	else if (element == "InitialNode")
;
	else if (element == "Loop")
		result  << "Iterations";
	else if (element == "NullificationEncoder")
		result  << "Port";
	else if (element == "PlayTone")
		result  << "Duration" << "Frequency" << "WaitForCompletion";
	else if (element == "RobotsDiagramNode")
		result  << "isView" << "language";
	else if (element == "Timer")
		result  << "Delay";
	else if (element == "WaitForColor")
		result  << "Color" << "Port";
	else if (element == "WaitForColorIntensity")
		result  << "Intensity" << "Port" << "Sign";
	else if (element == "WaitForEncoder")
		result  << "Port" << "TachoLimit";
	else if (element == "WaitForSonarDistance")
		result  << "Distance" << "Port" << "Sign";
	else if (element == "WaitForTouchSensor")
		result  << "Port";
	return result;
}

QStringList RobotsMetamodelPlugin::getTypesContainedBy(QString const &element) const
{
	QStringList result;
	if (element == "RobotsDiagramNode")
		result << "AbstractNode" ;
	return result;
}

QStringList RobotsMetamodelPlugin::getConnectedTypes(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element);
	return result;
}

QStringList RobotsMetamodelPlugin::getUsedTypes(QString const &element) const
{
	QStringList result;
	Q_UNUSED(element);
	return result;
}

QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > RobotsMetamodelPlugin::getPossibleEdges(QString const &element) const
{
	QList<QPair<QPair<QString,QString>,QPair<bool,QString> > > result;
	if (element == "ControlFlow")
		result << qMakePair(qMakePair(QString("AbstractNode"),QString("AbstractNode")),qMakePair(false,QString("ControlFlow")));
	return result;
}

//(-1) means "edge", (+1) means "node"
int RobotsMetamodelPlugin::isNodeOrEdge(QString const &element) const
{
	if (element == "AbstractNode")
		return 1;
	else if (element == "Beep")
		return 1;
	else if (element == "Color")
		return 0;
	else if (element == "ControlFlow")
		return -1;
	else if (element == "DistanceSign")
		return 0;
	else if (element == "EngineCommand")
		return 1;
	else if (element == "EngineMovementCommand")
		return 1;
	else if (element == "EnginesBackward")
		return 1;
	else if (element == "EnginesForward")
		return 1;
	else if (element == "EnginesStop")
		return 1;
	else if (element == "FinalNode")
		return 1;
	else if (element == "Fork")
		return 1;
	else if (element == "Function")
		return 1;
	else if (element == "GuardType")
		return 0;
	else if (element == "IfBlock")
		return 1;
	else if (element == "InitialBlock")
		return 1;
	else if (element == "InitialNode")
		return 1;
	else if (element == "Loop")
		return 1;
	else if (element == "NullificationEncoder")
		return 1;
	else if (element == "PlayTone")
		return 1;
	else if (element == "RobotsDiagramNode")
		return 1;
	else if (element == "SensorBlock")
		return 1;
	else if (element == "SensorPort")
		return 0;
	else if (element == "Sensors")
		return 0;
	else if (element == "Timer")
		return 1;
	else if (element == "WaitForColor")
		return 1;
	else if (element == "WaitForColorIntensity")
		return 1;
	else if (element == "WaitForEncoder")
		return 1;
	else if (element == "WaitForSonarDistance")
		return 1;
	else if (element == "WaitForTouchSensor")
		return 1;
	return 0;
}
QStringList RobotsMetamodelPlugin::getEnumValues(QString name) const 
{
	QStringList result;
	if (name == "ParameterDirectionKind")
		result << QString::fromUtf8("in") << QString::fromUtf8("inout") << QString::fromUtf8("out") << QString::fromUtf8("return");
	else if (name == "VisibilityKind")
		result << QString::fromUtf8("private") << QString::fromUtf8("protected") << QString::fromUtf8("public") << QString::fromUtf8("package");
	else if (name == "GuardType")
		result << QString::fromUtf8("итерация") << QString::fromUtf8("") << QString::fromUtf8("меньше 0") << QString::fromUtf8("больше 0") << QString::fromUtf8("равно 0");
	else if (name == "CallConcurrencyKind")
		result << QString::fromUtf8("sequential") << QString::fromUtf8("guarded") << QString::fromUtf8("concurrent");
	else if (name == "Color")
		result << QString::fromUtf8("Чёрный") << QString::fromUtf8("Синий") << QString::fromUtf8("Зелёный") << QString::fromUtf8("Жёлтый") << QString::fromUtf8("Красный") << QString::fromUtf8("Белый");
	else if (name == "Sensors")
		result << QString::fromUtf8("Не используется") << QString::fromUtf8("Сенсор нажатия (булево значение)") << QString::fromUtf8("Сенсор нажатия (сырое значение)") << QString::fromUtf8("Ультразвуковой сенсор") << QString::fromUtf8("Сенсор цвета (полные цвета)") << QString::fromUtf8("Сенсор цвета (красный)") << QString::fromUtf8("Сенсор цвета (зеленый)") << QString::fromUtf8("Сенсор цвета (синий)") << QString::fromUtf8("Сенсор цвета (пассивный)");
	else if (name == "AggregationKind")
		result << QString::fromUtf8("none") << QString::fromUtf8("shared") << QString::fromUtf8("composite");
	else if (name == "SensorPort")
		result << QString::fromUtf8("1") << QString::fromUtf8("2") << QString::fromUtf8("3") << QString::fromUtf8("4");
	else if (name == "Bool")
		result << QString::fromUtf8("false") << QString::fromUtf8("true");
	else if (name == "DistanceSign")
		result << QString::fromUtf8("равно") << QString::fromUtf8("больше") << QString::fromUtf8("меньше") << QString::fromUtf8("не меньше") << QString::fromUtf8("не больше");
	return result;
}

QString RobotsMetamodelPlugin::getPropertyType(QString const &element, QString const &property) const
{
	return propertyTypes[element][property];
}

QString RobotsMetamodelPlugin::getPropertyDefaultValue(QString const &element, QString const &property) const
{
	return propertyDefault[element][property];
}

QList<qReal::ListenerInterface*> RobotsMetamodelPlugin::listeners() const
{
	QList<qReal::ListenerInterface*> result;
	return result;
}
