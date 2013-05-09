#include "pluginInterface.h"

#include "elements.h"


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
	initPaletteGroupsMap();
	initPaletteGroupsDescriptionMap();
}

void RobotsMetamodelPlugin::initNameMap()
{
	diagramNameMap["RobotsDiagram"] = QString::fromUtf8("Диаграмма поведения робота");
	diagramNodeNameMap["RobotsDiagram"] = "RobotsDiagramNode";

	elementsNameMap["RobotsDiagram"]["Balance"] = QString::fromUtf8("Балансировка");
	elementsNameMap["RobotsDiagram"]["BalanceInit"] = QString::fromUtf8("Инициализация балансировки");
	elementsNameMap["RobotsDiagram"]["Beep"] = QString::fromUtf8("Гудок");
	elementsNameMap["RobotsDiagram"]["ControlFlow"] = QString::fromUtf8("Линия соединения");
	elementsNameMap["RobotsDiagram"]["EnginesBackward"] = QString::fromUtf8("Моторы назад");
	elementsNameMap["RobotsDiagram"]["EnginesForward"] = QString::fromUtf8("Моторы вперёд");
	elementsNameMap["RobotsDiagram"]["EnginesStop"] = QString::fromUtf8("Моторы стоп");
	elementsNameMap["RobotsDiagram"]["FinalNode"] = QString::fromUtf8("Конец");
	elementsNameMap["RobotsDiagram"]["Fork"] = QString::fromUtf8("Параллельные задачи");
	elementsNameMap["RobotsDiagram"]["Function"] = QString::fromUtf8("Функция");
	elementsNameMap["RobotsDiagram"]["IfBlock"] = QString::fromUtf8("Условие");
	elementsNameMap["RobotsDiagram"]["InitialNode"] = QString::fromUtf8("Начало");
	elementsNameMap["RobotsDiagram"]["Loop"] = QString::fromUtf8("Цикл");
	elementsNameMap["RobotsDiagram"]["NullificationEncoder"] = QString::fromUtf8("Сбросить показания энкодера");
	elementsNameMap["RobotsDiagram"]["PlayTone"] = QString::fromUtf8("Играть звук");
	elementsNameMap["RobotsDiagram"]["RobotsDiagramNode"] = QString::fromUtf8("Диаграмма поведения робота");
	elementsNameMap["RobotsDiagram"]["SubprogramRobots"] = QString::fromUtf8("Подпрограмма");
	elementsNameMap["RobotsDiagram"]["Timer"] = QString::fromUtf8("Таймер");
	elementsNameMap["RobotsDiagram"]["VariableInit"] = QString::fromUtf8("Инициализация переменной");
	elementsNameMap["RobotsDiagram"]["WaitForAccelerometer"] = QString::fromUtf8("Ждать Акселерометр");
	elementsNameMap["RobotsDiagram"]["WaitForColor"] = QString::fromUtf8("Ждать цвет");
	elementsNameMap["RobotsDiagram"]["WaitForColorIntensity"] = QString::fromUtf8("Ждать интенсивность цвета");
	elementsNameMap["RobotsDiagram"]["WaitForEncoder"] = QString::fromUtf8("Ждать энкодер");
	elementsNameMap["RobotsDiagram"]["WaitForGyroscope"] = QString::fromUtf8("Ждать гироскоп");
	elementsNameMap["RobotsDiagram"]["WaitForLight"] = QString::fromUtf8("Ждать свет");
	elementsNameMap["RobotsDiagram"]["WaitForSonarDistance"] = QString::fromUtf8("Ждать сонар");
	elementsNameMap["RobotsDiagram"]["WaitForSound"] = QString::fromUtf8("Ждать сенсор звука");
	elementsNameMap["RobotsDiagram"]["WaitForTouchSensor"] = QString::fromUtf8("Ждать сенсор касания");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Balance"]["forward"] = QString::fromUtf8("Значение forward");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Balance"]["gyroOffset"] = QString::fromUtf8("Значение для калибровки (что бы это ни значило)");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Balance"]["port"] = QString::fromUtf8("Порт гироскопа");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Balance"]["port1"] = QString::fromUtf8("Порт первого мотора");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Balance"]["port2"] = QString::fromUtf8("Порт второго мотора");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Balance"]["pwm1"] = QString::fromUtf8("Выходное значение мощности первого мотора");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Balance"]["pwm2"] = QString::fromUtf8("Выходное значение мощности второго мотора");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Balance"]["turn"] = QString::fromUtf8("Значение turn");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Beep"]["Volume"] = QString::fromUtf8("Громкость");
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
	propertiesDisplayedNamesMap["RobotsDiagram"]["Function"]["Init"] = QString::fromUtf8("Инициализация");
	propertiesDisplayedNamesMap["RobotsDiagram"]["IfBlock"]["Condition"] = QString::fromUtf8("Условие");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Loop"]["Iterations"] = QString::fromUtf8("Итерации");
	propertiesDisplayedNamesMap["RobotsDiagram"]["NullificationEncoder"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["PlayTone"]["Duration"] = QString::fromUtf8("Длительность");
	propertiesDisplayedNamesMap["RobotsDiagram"]["PlayTone"]["Frequency"] = QString::fromUtf8("Частота");
	propertiesDisplayedNamesMap["RobotsDiagram"]["PlayTone"]["Volume"] = QString::fromUtf8("Громкость");
	propertiesDisplayedNamesMap["RobotsDiagram"]["PlayTone"]["WaitForCompletion"] = QString::fromUtf8("Ждать завершения");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Timer"]["Delay"] = QString::fromUtf8("Задержка");
	propertiesDisplayedNamesMap["RobotsDiagram"]["VariableInit"]["value"] = QString::fromUtf8("Значение");
	propertiesDisplayedNamesMap["RobotsDiagram"]["VariableInit"]["variable"] = QString::fromUtf8("Переменная");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForAccelerometer"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForColor"]["Color"] = QString::fromUtf8("Цвет");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForColor"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForColorIntensity"]["Intensity"] = QString::fromUtf8("Интенсивность");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForColorIntensity"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForColorIntensity"]["Sign"] = QString::fromUtf8("Считанное значение");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForEncoder"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForEncoder"]["TachoLimit"] = QString::fromUtf8("Предел оборотов");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForGyroscope"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForLight"]["Percents"] = QString::fromUtf8("Проценты");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForLight"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForLight"]["Sign"] = QString::fromUtf8("Считанное значение");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForSonarDistance"]["Distance"] = QString::fromUtf8("Дистанция");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForSonarDistance"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForSonarDistance"]["Sign"] = QString::fromUtf8("Считанное значение");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForSound"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForTouchSensor"]["Port"] = QString::fromUtf8("Порт");
}

void RobotsMetamodelPlugin::initPaletteGroupsMap()
{
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Алгоритмы")].append(QString::fromUtf8("IfBlock"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Алгоритмы")].append(QString::fromUtf8("ControlFlow"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Алгоритмы")].append(QString::fromUtf8("RobotsDiagramNode"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Алгоритмы")].append(QString::fromUtf8("Loop"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Действия")].append(QString::fromUtf8("Fork"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Действия")].append(QString::fromUtf8("Beep"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Действия")].append(QString::fromUtf8("PlayTone"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Действия")].append(QString::fromUtf8("EnginesForward"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Действия")].append(QString::fromUtf8("EnginesBackward"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Действия")].append(QString::fromUtf8("EnginesStop"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Действия")].append(QString::fromUtf8("Function"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Инициализация")].append(QString::fromUtf8("InitialNode"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Инициализация")].append(QString::fromUtf8("InitialBlock"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Инициализация")].append(QString::fromUtf8("FinalNode"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Инициализация")].append(QString::fromUtf8("NullificationEncoder"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Ожидания")].append(QString::fromUtf8("Timer"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Ожидания")].append(QString::fromUtf8("WaitForTouchSensor"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Ожидания")].append(QString::fromUtf8("WaitForSonarDistance"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Ожидания")].append(QString::fromUtf8("WaitForColor"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Ожидания")].append(QString::fromUtf8("WaitForEncoder"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Ожидания")].append(QString::fromUtf8("WaitForColorIntensity"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Ожидания")].append(QString::fromUtf8("WaitForLight"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Ожидания")].append(QString::fromUtf8("WaitForSound"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Ожидания")].append(QString::fromUtf8("WaitForGyroscope"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Ожидания")].append(QString::fromUtf8("WaitForAccelerometer"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Сегвей")].append(QString::fromUtf8("Balance"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Сегвей")].append(QString::fromUtf8("BalanceInit"));
}

void RobotsMetamodelPlugin::initPaletteGroupsDescriptionMap()
{
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
	propertyTypes["Function"]["Init"] = "Bool";
	propertyTypes["PlayTone"]["WaitForCompletion"] = "Bool";
	propertyTypes["WaitForAccelerometer"]["Port"] = "SensorPort";
	propertyTypes["WaitForColor"]["Color"] = "Color";
	propertyTypes["WaitForColor"]["Port"] = "SensorPort";
	propertyTypes["WaitForColorIntensity"]["Port"] = "SensorPort";
	propertyTypes["WaitForColorIntensity"]["Sign"] = "DistanceSign";
	propertyTypes["WaitForGyroscope"]["Port"] = "SensorPort";
	propertyTypes["WaitForLight"]["Port"] = "SensorPort";
	propertyTypes["WaitForLight"]["Sign"] = "DistanceSign";
	propertyTypes["WaitForSonarDistance"]["Port"] = "SensorPort";
	propertyTypes["WaitForSonarDistance"]["Sign"] = "DistanceSign";
	propertyTypes["WaitForSound"]["Port"] = "SensorPort";
	propertyTypes["WaitForTouchSensor"]["Port"] = "SensorPort";
}

void RobotsMetamodelPlugin::initPropertyDefaultsMap()
{
	propertyDefault["Balance"]["forward"] = QString::fromUtf8("0");
	propertyDefault["Balance"]["gyroOffset"] = QString::fromUtf8("0");
	propertyDefault["Balance"]["port"] = QString::fromUtf8("4");
	propertyDefault["Balance"]["port1"] = QString::fromUtf8("B");
	propertyDefault["Balance"]["port2"] = QString::fromUtf8("C");
	propertyDefault["Balance"]["turn"] = QString::fromUtf8("0");
	propertyDefault["Beep"]["Volume"] = QString::fromUtf8("50");
	propertyDefault["Beep"]["WaitForCompletion"] = QString::fromUtf8("true");
	propertyDefault["EnginesBackward"]["Ports"] = QString::fromUtf8("B, C");
	propertyDefault["EnginesBackward"]["Power"] = QString::fromUtf8("100");
	propertyDefault["EnginesBackward"]["TachoLimit"] = QString::fromUtf8("0");
	propertyDefault["EnginesForward"]["Ports"] = QString::fromUtf8("B, C");
	propertyDefault["EnginesForward"]["Power"] = QString::fromUtf8("100");
	propertyDefault["EnginesForward"]["TachoLimit"] = QString::fromUtf8("0");
	propertyDefault["EnginesStop"]["Ports"] = QString::fromUtf8("B, C");
	propertyDefault["IfBlock"]["Condition"] = QString::fromUtf8("0");
	propertyDefault["Loop"]["Iterations"] = QString::fromUtf8("10");
	propertyDefault["PlayTone"]["Duration"] = QString::fromUtf8("1000");
	propertyDefault["PlayTone"]["Frequency"] = QString::fromUtf8("1000");
	propertyDefault["PlayTone"]["Volume"] = QString::fromUtf8("50");
	propertyDefault["PlayTone"]["WaitForCompletion"] = QString::fromUtf8("true");
	propertyDefault["Timer"]["Delay"] = QString::fromUtf8("0");
	propertyDefault["VariableInit"]["value"] = QString::fromUtf8("0");
	propertyDefault["VariableInit"]["variable"] = QString::fromUtf8("0");
	propertyDefault["WaitForAccelerometer"]["Port"] = QString::fromUtf8("1");
	propertyDefault["WaitForColor"]["Port"] = QString::fromUtf8("1");
	propertyDefault["WaitForColorIntensity"]["Intensity"] = QString::fromUtf8("0");
	propertyDefault["WaitForColorIntensity"]["Port"] = QString::fromUtf8("1");
	propertyDefault["WaitForColorIntensity"]["Sign"] = QString::fromUtf8("равно");
	propertyDefault["WaitForGyroscope"]["Port"] = QString::fromUtf8("1");
	propertyDefault["WaitForLight"]["Percents"] = QString::fromUtf8("0");
	propertyDefault["WaitForLight"]["Port"] = QString::fromUtf8("1");
	propertyDefault["WaitForLight"]["Sign"] = QString::fromUtf8("равно");
	propertyDefault["WaitForSonarDistance"]["Distance"] = QString::fromUtf8("0");
	propertyDefault["WaitForSonarDistance"]["Port"] = QString::fromUtf8("1");
	propertyDefault["WaitForSonarDistance"]["Sign"] = QString::fromUtf8("равно");
	propertyDefault["WaitForSound"]["Port"] = QString::fromUtf8("1");
	propertyDefault["WaitForTouchSensor"]["Port"] = QString::fromUtf8("1");
}

void RobotsMetamodelPlugin::initDescriptionMap()
{
}

void RobotsMetamodelPlugin::initParentsMap()
{
	parentsMap["RobotsDiagram"]["Balance"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["BalanceInit"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
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
	parentsMap["RobotsDiagram"]["SensorBlock"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["SubprogramRobots"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["Timer"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["VariableInit"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["WaitForAccelerometer"]
		<< qMakePair(QString("RobotsDiagram"), QString("SensorBlock"))
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
	parentsMap["RobotsDiagram"]["WaitForGyroscope"]
		<< qMakePair(QString("RobotsDiagram"), QString("SensorBlock"))
	;
	parentsMap["RobotsDiagram"]["WaitForLight"]
		<< qMakePair(QString("RobotsDiagram"), QString("SensorBlock"))
	;
	parentsMap["RobotsDiagram"]["WaitForSonarDistance"]
		<< qMakePair(QString("RobotsDiagram"), QString("SensorBlock"))
	;
	parentsMap["RobotsDiagram"]["WaitForSound"]
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

QStringList RobotsMetamodelPlugin::diagramPaletteGroups(QString const &diagram) const
{
	return paletteGroupsMap[diagram].keys();
}

QStringList RobotsMetamodelPlugin::diagramPaletteGroupList(QString const &diagram, QString const &group) const
{
	return paletteGroupsMap[diagram][group];
}

QString RobotsMetamodelPlugin::diagramPaletteGroupDescription(QString const &diagram, QString const &group) const
{
	return paletteGroupsDescriptionMap[diagram][group];
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
	if (element == "Balance")
		return new Balance();
	else if (element == "BalanceInit")
		return new BalanceInit();
	else if (element == "Beep")
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
	else if (element == "SubprogramRobots")
		return new SubprogramRobots();
	else if (element == "Timer")
		return new Timer();
	else if (element == "VariableInit")
		return new VariableInit();
	else if (element == "WaitForAccelerometer")
		return new WaitForAccelerometer();
	else if (element == "WaitForColor")
		return new WaitForColor();
	else if (element == "WaitForColorIntensity")
		return new WaitForColorIntensity();
	else if (element == "WaitForEncoder")
		return new WaitForEncoder();
	else if (element == "WaitForGyroscope")
		return new WaitForGyroscope();
	else if (element == "WaitForLight")
		return new WaitForLight();
	else if (element == "WaitForSonarDistance")
		return new WaitForSonarDistance();
	else if (element == "WaitForSound")
		return new WaitForSound();
	else if (element == "WaitForTouchSensor")
		return new WaitForTouchSensor();
	else {
		Q_ASSERT(!"Request for creation of an element with unknown name");
		return NULL;
	}
}

bool RobotsMetamodelPlugin::isParentOf(QString const &parentDiagram, QString const &parentElement, QString const &childDiagram, QString const &childElement) const
{
	if (childDiagram == parentDiagram && childElement == parentElement)
		return true;
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
	if (element == "Balance")
		result  << "forward" << "gyroOffset" << "port" << "port1" << "port2" << "pwm1" << "pwm2"
		 << "turn";
	else if (element == "BalanceInit")
;
	else if (element == "Beep")
		result  << "Volume" << "WaitForCompletion";
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
		result  << "Body" << "Init";
	else if (element == "IfBlock")
		result  << "Condition";
	else if (element == "InitialNode")
;
	else if (element == "Loop")
		result  << "Iterations";
	else if (element == "NullificationEncoder")
		result  << "Port";
	else if (element == "PlayTone")
		result  << "Duration" << "Frequency" << "Volume" << "WaitForCompletion";
	else if (element == "RobotsDiagramNode")
;
	else if (element == "SubprogramRobots")
;
	else if (element == "Timer")
		result  << "Delay";
	else if (element == "VariableInit")
		result  << "value" << "variable";
	else if (element == "WaitForAccelerometer")
		result  << "Port";
	else if (element == "WaitForColor")
		result  << "Color" << "Port";
	else if (element == "WaitForColorIntensity")
		result  << "Intensity" << "Port" << "Sign";
	else if (element == "WaitForEncoder")
		result  << "Port" << "TachoLimit";
	else if (element == "WaitForGyroscope")
		result  << "Port";
	else if (element == "WaitForLight")
		result  << "Percents" << "Port" << "Sign";
	else if (element == "WaitForSonarDistance")
		result  << "Distance" << "Port" << "Sign";
	else if (element == "WaitForSound")
		result  << "Port";
	else if (element == "WaitForTouchSensor")
		result  << "Port";
	return result;
}

QStringList RobotsMetamodelPlugin::getReferenceProperties(QString const &/*diagram*/, QString const &element) const
{
	QStringList result;
	if (element == "Balance")
;
	else if (element == "BalanceInit")
;
	else if (element == "Beep")
;
	else if (element == "ControlFlow")
;
	else if (element == "EnginesBackward")
;
	else if (element == "EnginesForward")
;
	else if (element == "EnginesStop")
;
	else if (element == "FinalNode")
;
	else if (element == "Fork")
;
	else if (element == "Function")
;
	else if (element == "IfBlock")
;
	else if (element == "InitialNode")
;
	else if (element == "Loop")
;
	else if (element == "NullificationEncoder")
;
	else if (element == "PlayTone")
;
	else if (element == "RobotsDiagramNode")
;
	else if (element == "SubprogramRobots")
;
	else if (element == "Timer")
;
	else if (element == "VariableInit")
;
	else if (element == "WaitForAccelerometer")
;
	else if (element == "WaitForColor")
;
	else if (element == "WaitForColorIntensity")
;
	else if (element == "WaitForEncoder")
;
	else if (element == "WaitForGyroscope")
;
	else if (element == "WaitForLight")
;
	else if (element == "WaitForSonarDistance")
;
	else if (element == "WaitForSound")
;
	else if (element == "WaitForTouchSensor")
;
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
	else if (element == "Balance")
		return 1;
	else if (element == "BalanceInit")
		return 1;
	else if (element == "Beep")
		return 1;
	else if (element == "BreakEngineMode")
		return 0;
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
	else if (element == "SubprogramRobots")
		return 1;
	else if (element == "Timer")
		return 1;
	else if (element == "VariableInit")
		return 1;
	else if (element == "WaitForAccelerometer")
		return 1;
	else if (element == "WaitForColor")
		return 1;
	else if (element == "WaitForColorIntensity")
		return 1;
	else if (element == "WaitForEncoder")
		return 1;
	else if (element == "WaitForGyroscope")
		return 1;
	else if (element == "WaitForLight")
		return 1;
	else if (element == "WaitForSonarDistance")
		return 1;
	else if (element == "WaitForSound")
		return 1;
	else if (element == "WaitForTouchSensor")
		return 1;
	return 0;
}
QString RobotsMetamodelPlugin::getGroupsXML() const 
{
	return QString::fromUtf8("");
}

QStringList RobotsMetamodelPlugin::getEnumValues(QString name) const 
{
	QStringList result;
	if (name == "Bool")
		result << QString::fromUtf8("false") << QString::fromUtf8("true");
	else if (name == "Sensors")
		result << QString::fromUtf8("Не используется") << QString::fromUtf8("Ультразвуковой сенсор") << QString::fromUtf8("Сенсор касания (булево значение)") << QString::fromUtf8("Сенсор касания (сырое значение)") << QString::fromUtf8("Сенсор цвета (все цвета)") << QString::fromUtf8("Сенсор цвета (красный)") << QString::fromUtf8("Сенсор цвета (зеленый)") << QString::fromUtf8("Сенсор цвета (синий)") << QString::fromUtf8("Сенсор цвета (пассивный)") << QString::fromUtf8("Сенсок звука") << QString::fromUtf8("Гироскоп") << QString::fromUtf8("Акселерометр");
	else if (name == "AggregationKind")
		result << QString::fromUtf8("none") << QString::fromUtf8("shared") << QString::fromUtf8("composite");
	else if (name == "BreakEngineMode")
		result << QString::fromUtf8("тормозить") << QString::fromUtf8("скользить");
	else if (name == "DistanceSign")
		result << QString::fromUtf8("равно") << QString::fromUtf8("больше") << QString::fromUtf8("меньше") << QString::fromUtf8("не меньше") << QString::fromUtf8("не больше");
	else if (name == "VisibilityKind")
		result << QString::fromUtf8("private") << QString::fromUtf8("protected") << QString::fromUtf8("public") << QString::fromUtf8("package");
	else if (name == "ParameterDirectionKind")
		result << QString::fromUtf8("in") << QString::fromUtf8("inout") << QString::fromUtf8("out") << QString::fromUtf8("return");
	else if (name == "CallConcurrencyKind")
		result << QString::fromUtf8("sequential") << QString::fromUtf8("guarded") << QString::fromUtf8("concurrent");
	else if (name == "Color")
		result << QString::fromUtf8("") << QString::fromUtf8("Чёрный") << QString::fromUtf8("Синий") << QString::fromUtf8("Зелёный") << QString::fromUtf8("Жёлтый") << QString::fromUtf8("Красный") << QString::fromUtf8("Белый");
	else if (name == "SensorPort")
		result << QString::fromUtf8("1") << QString::fromUtf8("2") << QString::fromUtf8("3") << QString::fromUtf8("4");
	else if (name == "GuardType")
		result << QString::fromUtf8("") << QString::fromUtf8("итерация") << QString::fromUtf8("меньше 0") << QString::fromUtf8("больше 0") << QString::fromUtf8("равно 0");
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
