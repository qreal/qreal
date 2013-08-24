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
	elementsNameMap["RobotsDiagram"]["ClearScreen"] = QString::fromUtf8("Очистить экран");
	elementsNameMap["RobotsDiagram"]["CommentBlock"] = QString::fromUtf8("Комментарий");
	elementsNameMap["RobotsDiagram"]["ControlFlow"] = QString::fromUtf8("Линия соединения");
	elementsNameMap["RobotsDiagram"]["DrawCircle"] = QString::fromUtf8("Нарисовать круг");
	elementsNameMap["RobotsDiagram"]["DrawLine"] = QString::fromUtf8("Нарисовать линию");
	elementsNameMap["RobotsDiagram"]["DrawPixel"] = QString::fromUtf8("Нарисовать точку");
	elementsNameMap["RobotsDiagram"]["DrawRect"] = QString::fromUtf8("Нарисовать прямоугольник");
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
	elementsNameMap["RobotsDiagram"]["PrintText"] = QString::fromUtf8("Напечатать текст");
	elementsNameMap["RobotsDiagram"]["RobotsDiagramNode"] = QString::fromUtf8("Диаграмма поведения робота");
	elementsNameMap["RobotsDiagram"]["Subprogram"] = QString::fromUtf8("Подпрограмма");
	elementsNameMap["RobotsDiagram"]["Timer"] = QString::fromUtf8("Таймер");
	elementsNameMap["RobotsDiagram"]["VariableInit"] = QString::fromUtf8("Инициализация переменной");
	elementsNameMap["RobotsDiagram"]["WaitForAccelerometer"] = QString::fromUtf8("Ждать акселерометр");
	elementsNameMap["RobotsDiagram"]["WaitForButtons"] = QString::fromUtf8("Ждать нажатия кнопок");
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
	propertiesDisplayedNamesMap["RobotsDiagram"]["CommentBlock"]["Comment"] = QString::fromUtf8("Комментарий");
	propertiesDisplayedNamesMap["RobotsDiagram"]["ControlFlow"]["Guard"] = QString::fromUtf8("Условие");
	propertiesDisplayedNamesMap["RobotsDiagram"]["DrawCircle"]["CircleRadius"] = QString::fromUtf8("Радиус");
	propertiesDisplayedNamesMap["RobotsDiagram"]["DrawCircle"]["XCoordinateCircle"] = QString::fromUtf8("X координата");
	propertiesDisplayedNamesMap["RobotsDiagram"]["DrawCircle"]["YCoordinateCircle"] = QString::fromUtf8("Y координата");
	propertiesDisplayedNamesMap["RobotsDiagram"]["DrawLine"]["X1CoordinateLine"] = QString::fromUtf8("X1 координата");
	propertiesDisplayedNamesMap["RobotsDiagram"]["DrawLine"]["X2CoordinateLine"] = QString::fromUtf8("X2 координата");
	propertiesDisplayedNamesMap["RobotsDiagram"]["DrawLine"]["Y1CoordinateLine"] = QString::fromUtf8("Y1 координата");
	propertiesDisplayedNamesMap["RobotsDiagram"]["DrawLine"]["Y2CoordinateLine"] = QString::fromUtf8("Y2 координата");
	propertiesDisplayedNamesMap["RobotsDiagram"]["DrawPixel"]["XCoordinatePix"] = QString::fromUtf8("X координата");
	propertiesDisplayedNamesMap["RobotsDiagram"]["DrawPixel"]["YCoordinatePix"] = QString::fromUtf8("Y координата");
	propertiesDisplayedNamesMap["RobotsDiagram"]["DrawRect"]["HeightRect"] = QString::fromUtf8("Высота");
	propertiesDisplayedNamesMap["RobotsDiagram"]["DrawRect"]["WidthRect"] = QString::fromUtf8("Ширина");
	propertiesDisplayedNamesMap["RobotsDiagram"]["DrawRect"]["XCoordinateRect"] = QString::fromUtf8("X координата");
	propertiesDisplayedNamesMap["RobotsDiagram"]["DrawRect"]["YCoordinateRect"] = QString::fromUtf8("Y координата");
	propertiesDisplayedNamesMap["RobotsDiagram"]["EnginesBackward"]["Mode"] = QString::fromUtf8("Режим");
	propertiesDisplayedNamesMap["RobotsDiagram"]["EnginesBackward"]["Ports"] = QString::fromUtf8("Порты");
	propertiesDisplayedNamesMap["RobotsDiagram"]["EnginesBackward"]["Power"] = QString::fromUtf8("Мощность (%)");
	propertiesDisplayedNamesMap["RobotsDiagram"]["EnginesForward"]["Mode"] = QString::fromUtf8("Режим");
	propertiesDisplayedNamesMap["RobotsDiagram"]["EnginesForward"]["Ports"] = QString::fromUtf8("Порты");
	propertiesDisplayedNamesMap["RobotsDiagram"]["EnginesForward"]["Power"] = QString::fromUtf8("Мощность (%)");
	propertiesDisplayedNamesMap["RobotsDiagram"]["EnginesStop"]["Ports"] = QString::fromUtf8("Порты");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Function"]["Body"] = QString::fromUtf8("Тело функции");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Function"]["Init"] = QString::fromUtf8("Инициализация");
	propertiesDisplayedNamesMap["RobotsDiagram"]["IfBlock"]["Condition"] = QString::fromUtf8("Условие");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Loop"]["Iterations"] = QString::fromUtf8("Итерации");
	propertiesDisplayedNamesMap["RobotsDiagram"]["NullificationEncoder"]["Ports"] = QString::fromUtf8("Порты");
	propertiesDisplayedNamesMap["RobotsDiagram"]["PlayTone"]["Duration"] = QString::fromUtf8("Длительность");
	propertiesDisplayedNamesMap["RobotsDiagram"]["PlayTone"]["Frequency"] = QString::fromUtf8("Частота");
	propertiesDisplayedNamesMap["RobotsDiagram"]["PlayTone"]["Volume"] = QString::fromUtf8("Громкость");
	propertiesDisplayedNamesMap["RobotsDiagram"]["PlayTone"]["WaitForCompletion"] = QString::fromUtf8("Ждать завершения");
	propertiesDisplayedNamesMap["RobotsDiagram"]["PrintText"]["PrintText"] = QString::fromUtf8("Текст");
	propertiesDisplayedNamesMap["RobotsDiagram"]["PrintText"]["XCoordinateText"] = QString::fromUtf8("X координата");
	propertiesDisplayedNamesMap["RobotsDiagram"]["PrintText"]["YCoordinateText"] = QString::fromUtf8("Y координата");
	propertiesDisplayedNamesMap["RobotsDiagram"]["RobotsDiagramNode"]["sensor1Value"] = QString::fromUtf8("Сенсор1");
	propertiesDisplayedNamesMap["RobotsDiagram"]["RobotsDiagramNode"]["sensor2Value"] = QString::fromUtf8("Сенсор2");
	propertiesDisplayedNamesMap["RobotsDiagram"]["RobotsDiagramNode"]["sensor3Value"] = QString::fromUtf8("Сенсор3");
	propertiesDisplayedNamesMap["RobotsDiagram"]["RobotsDiagramNode"]["sensor4Value"] = QString::fromUtf8("Сенсор4");
	propertiesDisplayedNamesMap["RobotsDiagram"]["RobotsDiagramNode"]["worldModel"] = QString::fromUtf8("Модель мира");
	propertiesDisplayedNamesMap["RobotsDiagram"]["Timer"]["Delay"] = QString::fromUtf8("Задержка (мс)");
	propertiesDisplayedNamesMap["RobotsDiagram"]["VariableInit"]["value"] = QString::fromUtf8("Значение");
	propertiesDisplayedNamesMap["RobotsDiagram"]["VariableInit"]["variable"] = QString::fromUtf8("Переменная");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForAccelerometer"]["Acceleration"] = QString::fromUtf8("Ускорение");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForAccelerometer"]["AccelerationAxe"] = QString::fromUtf8("Ускорение по оси");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForAccelerometer"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForAccelerometer"]["Sign"] = QString::fromUtf8("Считанное значение");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForButtons"]["BottomButtonClicks"] = QString::fromUtf8("Количество нажатий нижней кнопки");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForButtons"]["CentralButtonClicks"] = QString::fromUtf8("Количество нажатий центральной кнопки");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForButtons"]["LeftButtonClicks"] = QString::fromUtf8("Количество нажатий левой кнопки");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForButtons"]["RightButtonClicks"] = QString::fromUtf8("Количество нажатий правой кнопки");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForColor"]["Color"] = QString::fromUtf8("Цвет");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForColor"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForColorIntensity"]["Intensity"] = QString::fromUtf8("Интенсивность");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForColorIntensity"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForColorIntensity"]["Sign"] = QString::fromUtf8("Считанное значение");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForEncoder"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForEncoder"]["Sign"] = QString::fromUtf8("Считанное значение");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForEncoder"]["TachoLimit"] = QString::fromUtf8("Предел оборотов");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForGyroscope"]["Degrees"] = QString::fromUtf8("Градусы/сек");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForGyroscope"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForGyroscope"]["Sign"] = QString::fromUtf8("Считанное значение");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForLight"]["Percents"] = QString::fromUtf8("Проценты");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForLight"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForLight"]["Sign"] = QString::fromUtf8("Считанное значение");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForSonarDistance"]["Distance"] = QString::fromUtf8("Дистанция");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForSonarDistance"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForSonarDistance"]["Sign"] = QString::fromUtf8("Считанное значение");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForSound"]["Port"] = QString::fromUtf8("Порт");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForSound"]["Sign"] = QString::fromUtf8("Считанное значение");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForSound"]["Volume"] = QString::fromUtf8("Громкость");
	propertiesDisplayedNamesMap["RobotsDiagram"]["WaitForTouchSensor"]["Port"] = QString::fromUtf8("Порт");
}

void RobotsMetamodelPlugin::initPaletteGroupsMap()
{
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Алгоритмы")].append(QString::fromUtf8("IfBlock"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Алгоритмы")].append(QString::fromUtf8("ControlFlow"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Алгоритмы")].append(QString::fromUtf8("Loop"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Алгоритмы")].append(QString::fromUtf8("CommentBlock"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Алгоритмы")].append(QString::fromUtf8("Subprogram"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Действия")].append(QString::fromUtf8("Fork"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Действия")].append(QString::fromUtf8("Beep"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Действия")].append(QString::fromUtf8("PlayTone"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Действия")].append(QString::fromUtf8("EnginesForward"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Действия")].append(QString::fromUtf8("EnginesBackward"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Действия")].append(QString::fromUtf8("EnginesStop"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Действия")].append(QString::fromUtf8("Function"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Инициализация")].append(QString::fromUtf8("InitialNode"));
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
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Ожидания")].append(QString::fromUtf8("WaitForButtons"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Рисование на дисплее")].append(QString::fromUtf8("ClearScreen"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Рисование на дисплее")].append(QString::fromUtf8("DrawRect"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Рисование на дисплее")].append(QString::fromUtf8("DrawPixel"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Рисование на дисплее")].append(QString::fromUtf8("DrawLine"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Рисование на дисплее")].append(QString::fromUtf8("DrawCircle"));
	paletteGroupsMap[QString::fromUtf8("RobotsDiagram")][QString::fromUtf8("Рисование на дисплее")].append(QString::fromUtf8("PrintText"));
}

void RobotsMetamodelPlugin::initPaletteGroupsDescriptionMap()
{
}

void RobotsMetamodelPlugin::initMouseGestureMap()
{
	elementMouseGesturesMap["RobotsDiagram"]["Beep"] = "152, 12 : 281, 12 :  | 152, "
			"183 : 152, 12 :  | 155, 170 : 154, 176 : 153, 183 : 151, 190 : 149, 196 : 146, 2"
			"02 : 143, 208 : 139, 214 : 134, 219 : 129, 224 : 123, 228 : 117, 231 : 111, 234 "
			": 105, 236 : 98, 238 : 91, 239 : 85, 239 : 79, 239 : 72, 238 : 65, 236 : 59, 234"
			" : 53, 231 : 47, 228 : 41, 224 : 36, 219 : 31, 214 : 27, 208 : 24, 202 : 21, 196"
			" : 19, 190 : 17, 183 : 16, 176 : 16, 170 : 16, 164 : 17, 157 : 19, 150 : 21, 144"
			" : 24, 138 : 27, 132 : 31, 126 : 36, 121 : 41, 116 : 47, 112 : 53, 109 : 59, 106"
			" : 65, 104 : 72, 102 : 79, 101 : 85, 101 : 91, 101 : 98, 102 : 105, 104 : 111, 1"
			"06 : 117, 109 : 123, 112 : 129, 116 : 134, 121 : 139, 126 : 143, 132 : 146, 138 "
			": 149, 144 : 151, 150 : 153, 157 : 154, 164 : 155, 170 : ";
	elementMouseGesturesMap["RobotsDiagram"]["EnginesBackward"] = "111, 224 : 0, 113"
			" :  | 113, 0 : 0, 113 :  | 0, 113 : 287, 113 : ";
	elementMouseGesturesMap["RobotsDiagram"]["EnginesForward"] = "185, 207 : 287, 10"
			"5 :  | 287, 105 : 182, 0 :  | 0, 105 : 287, 105 : ";
	elementMouseGesturesMap["RobotsDiagram"]["EnginesStop"] = "49, 117 : 185, 117 : "
			" | 223, 118 : 222, 128 : 221, 138 : 218, 148 : 215, 158 : 211, 167 : 205, 176 : "
			"199, 184 : 192, 192 : 184, 199 : 176, 205 : 167, 211 : 158, 215 : 148, 218 : 138"
			", 221 : 128, 222 : 118, 223 : 108, 222 : 98, 221 : 88, 218 : 78, 215 : 69, 211 :"
			" 60, 205 : 52, 199 : 44, 192 : 37, 184 : 31, 176 : 25, 167 : 21, 158 : 18, 148 :"
			" 15, 138 : 14, 128 : 13, 118 : 14, 108 : 15, 98 : 18, 88 : 21, 78 : 25, 69 : 31,"
			" 60 : 37, 52 : 44, 44 : 52, 37 : 60, 31 : 69, 25 : 78, 21 : 88, 18 : 98, 15 : 10"
			"8, 14 : 118, 13 : 128, 14 : 138, 15 : 148, 18 : 158, 21 : 167, 25 : 176, 31 : 18"
			"4, 37 : 192, 44 : 199, 52 : 205, 60 : 211, 69 : 215, 78 : 218, 88 : 221, 98 : 22"
			"2, 108 : 223, 118 : ";
	elementMouseGesturesMap["RobotsDiagram"]["FinalNode"] = "13, 200 : 198, 15 :  | "
			"12, 12 : 198, 198 : ";
	elementMouseGesturesMap["RobotsDiagram"]["Fork"] = "13, 86 : 231, 86 :  | 12, 12"
			" : 231, 12 : ";
	elementMouseGesturesMap["RobotsDiagram"]["Function"] = "58, 12 : 58, 172 :  | 58"
			", 12 : 148, 12 :  | 12, 96 : 108, 96 : ";
	elementMouseGesturesMap["RobotsDiagram"]["IfBlock"] = "96, 182 : 180, 94 :  | 98"
			", 12 : 180, 94 :  | 12, 98 : 96, 182 :  | 98, 12 : 12, 98 : ";
	elementMouseGesturesMap["RobotsDiagram"]["InitialNode"] = "210, 111 : 209, 120 :"
			" 208, 130 : 205, 139 : 202, 148 : 198, 157 : 193, 166 : 187, 173 : 181, 181 : 17"
			"3, 187 : 166, 193 : 157, 198 : 148, 202 : 139, 205 : 130, 208 : 120, 209 : 111, "
			"209 : 102, 209 : 92, 208 : 83, 205 : 74, 202 : 65, 198 : 56, 193 : 49, 187 : 41,"
			" 181 : 35, 173 : 29, 166 : 24, 157 : 20, 148 : 17, 139 : 14, 130 : 13, 120 : 13,"
			" 111 : 13, 102 : 14, 92 : 17, 83 : 20, 74 : 24, 65 : 29, 56 : 35, 49 : 41, 41 : "
			"49, 35 : 56, 29 : 65, 24 : 74, 20 : 83, 17 : 92, 14 : 102, 13 : 111, 13 : 120, 1"
			"3 : 130, 14 : 139, 17 : 148, 20 : 157, 24 : 166, 29 : 173, 35 : 181, 41 : 187, 4"
			"9 : 193, 56 : 198, 65 : 202, 74 : 205, 83 : 208, 92 : 209, 102 : 210, 111 : ";
	elementMouseGesturesMap["RobotsDiagram"]["Loop"] = "154, 251 : 205, 302 :  | 154"
			", 251 : 209, 196 :  | 100, 12 : 154, 66 :  | 154, 66 : 103, 117 :  | 12, 66 : 29"
			"7, 66 : 297, 251 : 12, 251 : 12, 66 : ";
	elementMouseGesturesMap["RobotsDiagram"]["NullificationEncoder"] = "112, 176 : 7"
			"8, 142 :  | 112, 176 : 148, 140 :  | 112, 33 : 112, 176 :  | 12, 12 : 210, 12 : "
			"210, 210 : 12, 210 : 12, 12 : ";
	elementMouseGesturesMap["RobotsDiagram"]["PlayTone"] = "130, 62 : 130, 146 :  | "
			"106, 0 : 106, 212 :  | 0, 106 : 106, 212 :  | 0, 106 : 106, 0 : ";
	elementMouseGesturesMap["RobotsDiagram"]["Timer"] = "12, 117 : 117, 118 :  | 117"
			", 12 : 117, 118 :  | 222, 117 : 221, 127 : 219, 137 : 217, 147 : 214, 157 : 209,"
			" 166 : 204, 175 : 198, 183 : 191, 191 : 183, 198 : 175, 204 : 166, 209 : 157, 21"
			"4 : 147, 217 : 137, 219 : 127, 221 : 117, 221 : 107, 221 : 97, 219 : 87, 217 : 7"
			"7, 214 : 68, 209 : 59, 204 : 51, 198 : 43, 191 : 36, 183 : 30, 175 : 25, 166 : 2"
			"0, 157 : 17, 147 : 15, 137 : 13, 127 : 13, 117 : 13, 107 : 15, 97 : 17, 87 : 20,"
			" 77 : 25, 68 : 30, 59 : 36, 51 : 43, 43 : 51, 36 : 59, 30 : 68, 25 : 77, 20 : 87"
			", 17 : 97, 15 : 107, 13 : 117, 13 : 127, 13 : 137, 15 : 147, 17 : 157, 20 : 166,"
			" 25 : 175, 30 : 183, 36 : 191, 43 : 198, 51 : 204, 59 : 209, 68 : 214, 77 : 217,"
			" 87 : 219, 97 : 221, 107 : 222, 117 : ";
	elementMouseGesturesMap["RobotsDiagram"]["WaitForButtons"] = "55, 58 : 179, 58 :"
			" 179, 182 : 55, 182 : 55, 58 :  | 12, 12 : 221, 12 : 221, 221 : 12, 221 : 12, 12"
			" : ";
	elementMouseGesturesMap["RobotsDiagram"]["WaitForColor"] = "196, 12 : 196, 154 :"
			"  | 12, 12 : 380, 12 : 380, 154 : 12, 154 : 12, 12 : ";
	elementMouseGesturesMap["RobotsDiagram"]["WaitForColorIntensity"] = "201, 137 : "
			"200, 143 : 199, 149 : 198, 154 : 196, 160 : 193, 166 : 190, 171 : 186, 176 : 182"
			", 180 : 178, 184 : 173, 188 : 168, 191 : 162, 194 : 156, 196 : 151, 197 : 145, 1"
			"98 : 139, 198 : 133, 198 : 127, 197 : 122, 196 : 116, 194 : 110, 191 : 105, 188 "
			": 100, 184 : 96, 180 : 92, 176 : 88, 171 : 85, 166 : 82, 160 : 80, 154 : 79, 149"
			" : 78, 143 : 78, 137 : 78, 131 : 79, 125 : 80, 120 : 82, 114 : 85, 108 : 88, 103"
			" : 92, 98 : 96, 94 : 100, 90 : 105, 86 : 110, 83 : 116, 80 : 122, 78 : 127, 77 :"
			" 133, 76 : 139, 76 : 145, 76 : 151, 77 : 156, 78 : 162, 80 : 168, 83 : 173, 86 :"
			" 178, 90 : 182, 94 : 186, 98 : 190, 103 : 193, 108 : 196, 114 : 198, 120 : 199, "
			"125 : 200, 131 : 201, 137 :  | 260, 136 : 259, 148 : 257, 160 : 254, 171 : 250, "
			"183 : 245, 194 : 239, 204 : 231, 214 : 223, 223 : 214, 231 : 204, 239 : 194, 245"
			" : 183, 250 : 171, 254 : 160, 257 : 148, 259 : 136, 259 : 124, 259 : 112, 257 : "
			"101, 254 : 89, 250 : 78, 245 : 68, 239 : 58, 231 : 49, 223 : 41, 214 : 33, 204 :"
			" 27, 194 : 22, 183 : 18, 171 : 15, 160 : 13, 148 : 13, 136 : 13, 124 : 15, 112 :"
			" 18, 101 : 22, 89 : 27, 78 : 33, 68 : 41, 58 : 49, 49 : 58, 41 : 68, 33 : 78, 27"
			" : 89, 22 : 101, 18 : 112, 15 : 124, 13 : 136, 13 : 148, 13 : 160, 15 : 171, 18 "
			": 183, 22 : 194, 27 : 204, 33 : 214, 41 : 223, 49 : 231, 58 : 239, 68 : 245, 78 "
			": 250, 89 : 254, 101 : 257, 112 : 259, 124 : 260, 136 : ";
	elementMouseGesturesMap["RobotsDiagram"]["WaitForEncoder"] = "203, 211 : 168, 17"
			"6 :  | 203, 211 : 240, 174 :  | 12, 48 : 50, 86 :  | 12, 48 : 48, 12 :  | 203, 4"
			"8 : 203, 211 :  | 12, 48 : 203, 48 : ";
	elementMouseGesturesMap["RobotsDiagram"]["WaitForGyroscope"] = "101, 106 : 212, "
			"106 :  | 156, 183 : 156, 107 :  | 12, 183 : 156, 183 :  | 12, 12 : 12, 183 :  | "
			"12, 12 : 164, 12 : ";
	elementMouseGesturesMap["RobotsDiagram"]["WaitForLight"] = "249, 220 : 349, 220 "
			":  | 130, 12 : 130, 102 :  | 249, 221 : 248, 232 : 247, 244 : 244, 255 : 240, 26"
			"6 : 235, 276 : 229, 286 : 222, 296 : 214, 304 : 206, 312 : 196, 319 : 186, 325 :"
			" 176, 330 : 165, 334 : 154, 337 : 142, 338 : 131, 339 : 120, 338 : 108, 337 : 97"
			", 334 : 86, 330 : 76, 325 : 66, 319 : 56, 312 : 48, 304 : 40, 296 : 33, 286 : 27"
			", 276 : 22, 266 : 18, 255 : 15, 244 : 14, 232 : 13, 221 : 14, 210 : 15, 198 : 18"
			", 187 : 22, 176 : 27, 166 : 33, 156 : 40, 146 : 48, 138 : 56, 130 : 66, 123 : 76"
			", 117 : 86, 112 : 97, 108 : 108, 105 : 120, 104 : 131, 103 : 142, 104 : 154, 105"
			" : 165, 108 : 176, 112 : 186, 117 : 196, 123 : 206, 130 : 214, 138 : 222, 146 : "
			"229, 156 : 235, 166 : 240, 176 : 244, 187 : 247, 198 : 248, 210 : 249, 221 : ";
	elementMouseGesturesMap["RobotsDiagram"]["WaitForSonarDistance"] = "190, 162 : 2"
			"19, 122 :  | 219, 122 : 190, 86 :  | 120, 122 : 219, 122 :  | 12, 118 : 118, 12 "
			":  | 12, 118 : 118, 224 :  | 118, 12 : 118, 224 : ";
	elementMouseGesturesMap["RobotsDiagram"]["WaitForSound"] = "12, 178 : 117, 178 :"
			"  | 117, 96 : 117, 178 :  | 13, 96 : 117, 96 :  | 13, 12 : 13, 96 :  | 13, 12 : "
			"118, 12 : ";
	elementMouseGesturesMap["RobotsDiagram"]["WaitForTouchSensor"] = "12, 123 : 174,"
			" 123 :  | 174, 12 : 358, 12 : 358, 234 : 174, 234 : 174, 12 : ";
}

void RobotsMetamodelPlugin::initPropertyMap()
{
	propertyTypes["Beep"]["WaitForCompletion"] = "Bool";
	propertyTypes["ControlFlow"]["Guard"] = "GuardType";
	propertyTypes["DrawCircle"]["CircleRadius"] = "Int";
	propertyTypes["DrawCircle"]["XCoordinateCircle"] = "Int";
	propertyTypes["DrawCircle"]["YCoordinateCircle"] = "Int";
	propertyTypes["DrawLine"]["X1CoordinateLine"] = "Int";
	propertyTypes["DrawLine"]["X2CoordinateLine"] = "Int";
	propertyTypes["DrawLine"]["Y1CoordinateLine"] = "Int";
	propertyTypes["DrawLine"]["Y2CoordinateLine"] = "Int";
	propertyTypes["DrawPixel"]["XCoordinatePix"] = "Int";
	propertyTypes["DrawPixel"]["YCoordinatePix"] = "Int";
	propertyTypes["DrawRect"]["HeightRect"] = "Int";
	propertyTypes["DrawRect"]["WidthRect"] = "Int";
	propertyTypes["DrawRect"]["XCoordinateRect"] = "Int";
	propertyTypes["DrawRect"]["YCoordinateRect"] = "Int";
	propertyTypes["EnginesBackward"]["Mode"] = "BreakEngineMode";
	propertyTypes["EnginesForward"]["Mode"] = "BreakEngineMode";
	propertyTypes["Function"]["Init"] = "Bool";
	propertyTypes["PlayTone"]["WaitForCompletion"] = "Bool";
	propertyTypes["WaitForAccelerometer"]["AccelerationAxe"] = "Accelerations";
	propertyTypes["WaitForAccelerometer"]["Port"] = "SensorPort";
	propertyTypes["WaitForAccelerometer"]["Sign"] = "DistanceSign";
	propertyTypes["WaitForColor"]["Color"] = "Color";
	propertyTypes["WaitForColor"]["Port"] = "SensorPort";
	propertyTypes["WaitForColorIntensity"]["Port"] = "SensorPort";
	propertyTypes["WaitForColorIntensity"]["Sign"] = "DistanceSign";
	propertyTypes["WaitForEncoder"]["Sign"] = "DistanceSign";
	propertyTypes["WaitForGyroscope"]["Port"] = "SensorPort";
	propertyTypes["WaitForGyroscope"]["Sign"] = "DistanceSign";
	propertyTypes["WaitForLight"]["Port"] = "SensorPort";
	propertyTypes["WaitForLight"]["Sign"] = "DistanceSign";
	propertyTypes["WaitForSonarDistance"]["Port"] = "SensorPort";
	propertyTypes["WaitForSonarDistance"]["Sign"] = "DistanceSign";
	propertyTypes["WaitForSound"]["Port"] = "SensorPort";
	propertyTypes["WaitForSound"]["Sign"] = "DistanceSign";
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
	propertyDefault["CommentBlock"]["Comment"] = QString::fromUtf8("Введите текст...");
	propertyDefault["DrawCircle"]["CircleRadius"] = QString::fromUtf8("5");
	propertyDefault["DrawCircle"]["XCoordinateCircle"] = QString::fromUtf8("0");
	propertyDefault["DrawCircle"]["YCoordinateCircle"] = QString::fromUtf8("0");
	propertyDefault["DrawLine"]["X1CoordinateLine"] = QString::fromUtf8("0");
	propertyDefault["DrawLine"]["X2CoordinateLine"] = QString::fromUtf8("10");
	propertyDefault["DrawLine"]["Y1CoordinateLine"] = QString::fromUtf8("0");
	propertyDefault["DrawLine"]["Y2CoordinateLine"] = QString::fromUtf8("10");
	propertyDefault["DrawPixel"]["XCoordinatePix"] = QString::fromUtf8("0");
	propertyDefault["DrawPixel"]["YCoordinatePix"] = QString::fromUtf8("0");
	propertyDefault["DrawRect"]["HeightRect"] = QString::fromUtf8("5");
	propertyDefault["DrawRect"]["WidthRect"] = QString::fromUtf8("5");
	propertyDefault["DrawRect"]["XCoordinateRect"] = QString::fromUtf8("0");
	propertyDefault["DrawRect"]["YCoordinateRect"] = QString::fromUtf8("0");
	propertyDefault["EnginesBackward"]["Mode"] = QString::fromUtf8("тормозить");
	propertyDefault["EnginesBackward"]["Ports"] = QString::fromUtf8("B, C");
	propertyDefault["EnginesBackward"]["Power"] = QString::fromUtf8("100");
	propertyDefault["EnginesForward"]["Mode"] = QString::fromUtf8("тормозить");
	propertyDefault["EnginesForward"]["Ports"] = QString::fromUtf8("B, C");
	propertyDefault["EnginesForward"]["Power"] = QString::fromUtf8("100");
	propertyDefault["EnginesStop"]["Ports"] = QString::fromUtf8("B, C");
	propertyDefault["IfBlock"]["Condition"] = QString::fromUtf8("0");
	propertyDefault["Loop"]["Iterations"] = QString::fromUtf8("10");
	propertyDefault["NullificationEncoder"]["Ports"] = QString::fromUtf8("A, B, C");
	propertyDefault["PlayTone"]["Duration"] = QString::fromUtf8("1000");
	propertyDefault["PlayTone"]["Frequency"] = QString::fromUtf8("1000");
	propertyDefault["PlayTone"]["Volume"] = QString::fromUtf8("50");
	propertyDefault["PlayTone"]["WaitForCompletion"] = QString::fromUtf8("true");
	propertyDefault["PrintText"]["XCoordinateText"] = QString::fromUtf8("0");
	propertyDefault["PrintText"]["YCoordinateText"] = QString::fromUtf8("0");
	propertyDefault["RobotsDiagramNode"]["sensor1Value"] = QString::fromUtf8("0");
	propertyDefault["RobotsDiagramNode"]["sensor2Value"] = QString::fromUtf8("0");
	propertyDefault["RobotsDiagramNode"]["sensor3Value"] = QString::fromUtf8("0");
	propertyDefault["RobotsDiagramNode"]["sensor4Value"] = QString::fromUtf8("0");
	propertyDefault["Timer"]["Delay"] = QString::fromUtf8("1000");
	propertyDefault["VariableInit"]["value"] = QString::fromUtf8("0");
	propertyDefault["VariableInit"]["variable"] = QString::fromUtf8("0");
	propertyDefault["WaitForAccelerometer"]["Acceleration"] = QString::fromUtf8("0");
	propertyDefault["WaitForAccelerometer"]["AccelerationAxe"] = QString::fromUtf8("X");
	propertyDefault["WaitForAccelerometer"]["Port"] = QString::fromUtf8("1");
	propertyDefault["WaitForAccelerometer"]["Sign"] = QString::fromUtf8("больше");
	propertyDefault["WaitForButtons"]["BottomButtonClicks"] = QString::fromUtf8("0");
	propertyDefault["WaitForButtons"]["CentralButtonClicks"] = QString::fromUtf8("1");
	propertyDefault["WaitForButtons"]["LeftButtonClicks"] = QString::fromUtf8("0");
	propertyDefault["WaitForButtons"]["RightButtonClicks"] = QString::fromUtf8("0");
	propertyDefault["WaitForColor"]["Port"] = QString::fromUtf8("1");
	propertyDefault["WaitForColorIntensity"]["Intensity"] = QString::fromUtf8("0");
	propertyDefault["WaitForColorIntensity"]["Port"] = QString::fromUtf8("1");
	propertyDefault["WaitForColorIntensity"]["Sign"] = QString::fromUtf8("больше");
	propertyDefault["WaitForEncoder"]["Port"] = QString::fromUtf8("B");
	propertyDefault["WaitForEncoder"]["Sign"] = QString::fromUtf8("больше");
	propertyDefault["WaitForGyroscope"]["Degrees"] = QString::fromUtf8("0");
	propertyDefault["WaitForGyroscope"]["Port"] = QString::fromUtf8("1");
	propertyDefault["WaitForGyroscope"]["Sign"] = QString::fromUtf8("больше");
	propertyDefault["WaitForLight"]["Percents"] = QString::fromUtf8("0");
	propertyDefault["WaitForLight"]["Port"] = QString::fromUtf8("1");
	propertyDefault["WaitForLight"]["Sign"] = QString::fromUtf8("больше");
	propertyDefault["WaitForSonarDistance"]["Distance"] = QString::fromUtf8("0");
	propertyDefault["WaitForSonarDistance"]["Port"] = QString::fromUtf8("1");
	propertyDefault["WaitForSonarDistance"]["Sign"] = QString::fromUtf8("больше");
	propertyDefault["WaitForSound"]["Port"] = QString::fromUtf8("1");
	propertyDefault["WaitForSound"]["Sign"] = QString::fromUtf8("больше");
	propertyDefault["WaitForSound"]["Volume"] = QString::fromUtf8("0");
	propertyDefault["WaitForTouchSensor"]["Port"] = QString::fromUtf8("1");
}

void RobotsMetamodelPlugin::initDescriptionMap()
{
	elementsDescriptionMap["RobotsDiagram"]["Beep"] = QString::fromUtf8("Проиграть на роботе звук с фиксированной частотой. Имеется один параметр — ждать ли завершения проигрывания звука или сразу же перейти к следующему блоку. Допустимые значения — true и false.");
	elementsDescriptionMap["RobotsDiagram"]["CommentBlock"] = QString::fromUtf8("Блок с текстовыми заметками, которые игнорируются при генерации и интерпретации. Используйте его для повышения наглядности диаграммы.");
	elementsDescriptionMap["RobotsDiagram"]["ControlFlow"] = QString::fromUtf8("Для того, чтобы создать связь между двумя элементами A и B, можно навести курсор на элемент A, нажать правую кнопку мыши и, не отпуская ее, провести линию до элемента B, либо 'вытащить' связь из кружочка справа от элемента");
	elementsDescriptionMap["RobotsDiagram"]["EnginesBackward"] = QString::fromUtf8("Включить моторы в режиме реверса по заданным портам с заданной мощностью. Порты задаются буквами A, B или C, разделенными запятыми. Мощность задается в процентах числом от -100 до 100, если задано отрицательное значение, мотор включается в обычном режиме.");
	elementsDescriptionMap["RobotsDiagram"]["EnginesForward"] = QString::fromUtf8("Включить моторы по заданным портам с заданной мощностью. Порты задаются буквами A, B или C, разделенными запятыми. Мощность задается в процентах числом от -100 до 100, если задано отрицательное значение, мотор включается в режиме реверса.");
	elementsDescriptionMap["RobotsDiagram"]["EnginesStop"] = QString::fromUtf8("Выключить моторы по заданным портам");
	elementsDescriptionMap["RobotsDiagram"]["FinalNode"] = QString::fromUtf8("Конец программы. Если программа состоит из нескольких параллельных участков выполнения, достижение этого блока завершает соответствующий участок выполнения. У данного блока не может быть исходящих связей.");
	elementsDescriptionMap["RobotsDiagram"]["Fork"] = QString::fromUtf8("Разделить выполнение программы на несколько потоков, которые с точки зрения программиста будут выполняться параллельно. Так, например, можно одновременно ждать срабатывания сенсора и истечения временного интервала. Блок должен иметь хотя бы две исходящие связи.");
	elementsDescriptionMap["RobotsDiagram"]["Function"] = QString::fromUtf8("Посчитать значение заданного выражения. Также в данном блоке допускается определение переменных. Подробнее про синтаксис допустимых выражений параметра 'Функция' см. в документации в разделе 'Синтаксис выражений'.");
	elementsDescriptionMap["RobotsDiagram"]["IfBlock"] = QString::fromUtf8("Разделить выполнение программы в соответствии с заданным условием. Значением параметра 'Условие' является некое логическое выражение, на основе значения которого будет осуществлен выбор дальнейшего пути выполнения диаграммы. У данного блока должны быть две исходящие связи, у одной из которых должно быть задано значение параметра 'Условие' (возможные варианты — 'истина' и 'ложь'). При невыполнении данного условия выполнение передается по другой связи.");
	elementsDescriptionMap["RobotsDiagram"]["InitialNode"] = QString::fromUtf8("Начальная точка выполнения программы. На каждой диаграмме такой блок должен быть только один, в него не должно быть входящих связей, исходящая связь из этого элемента должна быть только одна. Процесс интерпретации диаграммы начинается именно с этого блока.");
	elementsDescriptionMap["RobotsDiagram"]["Loop"] = QString::fromUtf8("Блок, организующий выполнение последовательности блоков несколько раз. Число повторений задается значением параметра 'Итераций'. Блок должен иметь две исходящие связи, одна из которых должна быть помечена значением 'итерация' (то есть значение параметра 'Условие' должно быть 'итерация'). Другая связь, исходящая из блока 'Цикл', должна оставаться непомеченной: по ней будет осуществляться переход, когда программа пройдет через блок 'Цикл' указанное число раз.");
	elementsDescriptionMap["RobotsDiagram"]["NullificationEncoder"] = QString::fromUtf8("Сбросить показания количества оборотов моторов");
	elementsDescriptionMap["RobotsDiagram"]["PlayTone"] = QString::fromUtf8("Проиграть на роботе звук с заданной частотой и длительностью. Аналогичен блоку 'Гудок', но позволяет задавать параметры звука. Имеется параметр, определяющий, ждать ли завершения проигрывания звука или сразу же перейти к следующему блоку.");
	elementsDescriptionMap["RobotsDiagram"]["Timer"] = QString::fromUtf8("Ждать заданное количество времени, в миллисекундах");
	elementsDescriptionMap["RobotsDiagram"]["WaitForColor"] = QString::fromUtf8("Ждать, пока сенсор цвета в режиме распознавания цветов не вернет указанный цвет");
	elementsDescriptionMap["RobotsDiagram"]["WaitForColorIntensity"] = QString::fromUtf8("Ждать, пока значение, возвращаемое сенсором цвета на указанном порту, не будет сравнимо с указанным в значении параметра 'Интенсивность' (задается в процентах). Еще один парамер — номер порта, к которому подключен сенсор цвета. Также параметром указывается операция, которая будет использоваться для сравнения с введенной интенсивностью.");
	elementsDescriptionMap["RobotsDiagram"]["WaitForEncoder"] = QString::fromUtf8("Ждать, пока показания счетчика количества оборотов на заданном моторе не достинут указанного в значении параметра 'Предел оборотов'.");
	elementsDescriptionMap["RobotsDiagram"]["WaitForLight"] = QString::fromUtf8("Ждать, пока значение, возвращаемое сенсором света на указанном порту, не будет сравнимо с указанным в значении параметра 'Проценты'. Еще один парамер — номер порта, к которому подключен сенсор цвета. Также параметром указывается операция, которая будет использоваться для сравнения со значением параметра 'Проценты'.");
	elementsDescriptionMap["RobotsDiagram"]["WaitForSonarDistance"] = QString::fromUtf8("Ждать, пока расстояние, возвращаемое ультразвуковым сенсором расстояния, не будет сравнимо с указанным в значении параметра 'Расстояние' (расстояние задается в сантиметрах, от 0 до 255). Еще один парамер — номер порта, к которому подключен датчик расстояния. Также параметром указывается операция, которая будет использоваться для сравнения с введенным расстоянием.");
	elementsDescriptionMap["RobotsDiagram"]["WaitForTouchSensor"] = QString::fromUtf8("Ждать, пока не сработает сенсор касания. Параметром указывается номер порта, к которому подключен сенсор касания. Допустимые значения: 1, 2, 3, 4.");
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
	parentsMap["RobotsDiagram"]["ClearScreen"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["CommentBlock"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["DrawCircle"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["DrawLine"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["DrawPixel"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["DrawRect"]
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
	parentsMap["RobotsDiagram"]["PrintText"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["SensorBlock"]
		<< qMakePair(QString("RobotsDiagram"), QString("AbstractNode"))
	;
	parentsMap["RobotsDiagram"]["Subprogram"]
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
	parentsMap["RobotsDiagram"]["WaitForButtons"]
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
	else if (element == "ClearScreen")
		return new ClearScreen();
	else if (element == "CommentBlock")
		return new CommentBlock();
	else if (element == "ControlFlow")
		return new ControlFlow();
	else if (element == "DrawCircle")
		return new DrawCircle();
	else if (element == "DrawLine")
		return new DrawLine();
	else if (element == "DrawPixel")
		return new DrawPixel();
	else if (element == "DrawRect")
		return new DrawRect();
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
	else if (element == "PrintText")
		return new PrintText();
	else if (element == "RobotsDiagramNode")
		return new RobotsDiagramNode();
	else if (element == "Subprogram")
		return new Subprogram();
	else if (element == "Timer")
		return new Timer();
	else if (element == "VariableInit")
		return new VariableInit();
	else if (element == "WaitForAccelerometer")
		return new WaitForAccelerometer();
	else if (element == "WaitForButtons")
		return new WaitForButtons();
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
	else if (element == "ClearScreen")
;
	else if (element == "CommentBlock")
		result  << "Comment";
	else if (element == "ControlFlow")
		result  << "Guard";
	else if (element == "DrawCircle")
		result  << "CircleRadius" << "XCoordinateCircle" << "YCoordinateCircle";
	else if (element == "DrawLine")
		result  << "X1CoordinateLine" << "X2CoordinateLine" << "Y1CoordinateLine" << "Y2CoordinateLine";
	else if (element == "DrawPixel")
		result  << "XCoordinatePix" << "YCoordinatePix";
	else if (element == "DrawRect")
		result  << "HeightRect" << "WidthRect" << "XCoordinateRect" << "YCoordinateRect";
	else if (element == "EnginesBackward")
		result  << "Mode" << "Ports" << "Power";
	else if (element == "EnginesForward")
		result  << "Mode" << "Ports" << "Power";
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
		result  << "Ports";
	else if (element == "PlayTone")
		result  << "Duration" << "Frequency" << "Volume" << "WaitForCompletion";
	else if (element == "PrintText")
		result  << "PrintText" << "XCoordinateText" << "YCoordinateText";
	else if (element == "RobotsDiagramNode")
		result  << "sensor1Value" << "sensor2Value" << "sensor3Value" << "sensor4Value" << "worldModel";
	else if (element == "Subprogram")
;
	else if (element == "Timer")
		result  << "Delay";
	else if (element == "VariableInit")
		result  << "value" << "variable";
	else if (element == "WaitForAccelerometer")
		result  << "Acceleration" << "AccelerationAxe" << "Port" << "Sign";
	else if (element == "WaitForButtons")
		result  << "BottomButtonClicks" << "CentralButtonClicks" << "LeftButtonClicks" << "RightButtonClicks";
	else if (element == "WaitForColor")
		result  << "Color" << "Port";
	else if (element == "WaitForColorIntensity")
		result  << "Intensity" << "Port" << "Sign";
	else if (element == "WaitForEncoder")
		result  << "Port" << "Sign" << "TachoLimit";
	else if (element == "WaitForGyroscope")
		result  << "Degrees" << "Port" << "Sign";
	else if (element == "WaitForLight")
		result  << "Percents" << "Port" << "Sign";
	else if (element == "WaitForSonarDistance")
		result  << "Distance" << "Port" << "Sign";
	else if (element == "WaitForSound")
		result  << "Port" << "Sign" << "Volume";
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
	else if (element == "ClearScreen")
;
	else if (element == "CommentBlock")
;
	else if (element == "ControlFlow")
;
	else if (element == "DrawCircle")
;
	else if (element == "DrawLine")
;
	else if (element == "DrawPixel")
;
	else if (element == "DrawRect")
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
	else if (element == "PrintText")
;
	else if (element == "RobotsDiagramNode")
;
	else if (element == "Subprogram")
;
	else if (element == "Timer")
;
	else if (element == "VariableInit")
;
	else if (element == "WaitForAccelerometer")
;
	else if (element == "WaitForButtons")
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
		result << qMakePair(qMakePair(QString("NonTyped"),QString("NonTyped")),qMakePair(false,QString("ControlFlow")));
	return result;
}

//(-1) means "edge", (+1) means "node"
int RobotsMetamodelPlugin::isNodeOrEdge(QString const &element) const
{
	if (element == "AbstractNode")
		return 1;
	else if (element == "Accelerations")
		return 0;
	else if (element == "Balance")
		return 1;
	else if (element == "BalanceInit")
		return 1;
	else if (element == "Beep")
		return 1;
	else if (element == "BreakEngineMode")
		return 0;
	else if (element == "ClearScreen")
		return 1;
	else if (element == "Color")
		return 0;
	else if (element == "CommentBlock")
		return 1;
	else if (element == "ControlFlow")
		return -1;
	else if (element == "DistanceSign")
		return 0;
	else if (element == "DrawCircle")
		return 1;
	else if (element == "DrawLine")
		return 1;
	else if (element == "DrawPixel")
		return 1;
	else if (element == "DrawRect")
		return 1;
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
	else if (element == "PrintText")
		return 1;
	else if (element == "RobotsDiagramNode")
		return 1;
	else if (element == "SensorBlock")
		return 1;
	else if (element == "SensorPort")
		return 0;
	else if (element == "Subprogram")
		return 1;
	else if (element == "Timer")
		return 1;
	else if (element == "VariableInit")
		return 1;
	else if (element == "WaitForAccelerometer")
		return 1;
	else if (element == "WaitForButtons")
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
	if (name == "CallConcurrencyKind")
		result << QString::fromUtf8("sequential") << QString::fromUtf8("guarded") << QString::fromUtf8("concurrent");
	else if (name == "Color")
		result << QString::fromUtf8("") << QString::fromUtf8("Чёрный") << QString::fromUtf8("Синий") << QString::fromUtf8("Зелёный") << QString::fromUtf8("Жёлтый") << QString::fromUtf8("Красный") << QString::fromUtf8("Белый");
	else if (name == "Bool")
		result << QString::fromUtf8("false") << QString::fromUtf8("true");
	else if (name == "Accelerations")
		result << QString::fromUtf8("ось x") << QString::fromUtf8("ось y") << QString::fromUtf8("ось z") << QString::fromUtf8("норма");
	else if (name == "DistanceSign")
		result << QString::fromUtf8("больше") << QString::fromUtf8("меньше") << QString::fromUtf8("не меньше") << QString::fromUtf8("не больше");
	else if (name == "BreakEngineMode")
		result << QString::fromUtf8("тормозить") << QString::fromUtf8("скользить");
	else if (name == "AggregationKind")
		result << QString::fromUtf8("none") << QString::fromUtf8("shared") << QString::fromUtf8("composite");
	else if (name == "GuardType")
		result << QString::fromUtf8("") << QString::fromUtf8("итерация") << QString::fromUtf8("истина") << QString::fromUtf8("ложь");
	else if (name == "ParameterDirectionKind")
		result << QString::fromUtf8("in") << QString::fromUtf8("inout") << QString::fromUtf8("out") << QString::fromUtf8("return");
	else if (name == "VisibilityKind")
		result << QString::fromUtf8("private") << QString::fromUtf8("protected") << QString::fromUtf8("public") << QString::fromUtf8("package");
	else if (name == "SensorPort")
		result << QString::fromUtf8("1") << QString::fromUtf8("2") << QString::fromUtf8("3") << QString::fromUtf8("4");
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
