#include "drawBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

DrawBlockGenerator::DrawBlockGenerator()
{
}

QList<SmartLine> DrawBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	int numb = nxtGen->bmpFilesNumber();
	QString imageName = "Image_" + QString::number(numb);
	nxtGen->increaseBmpCounter();
	nxtGen->addBmpFileName(imageName);

	generateBmpFile(nxtGen, imageName, logicElementId);

	if(numb){
		result.append(SmartLine("for(int _counter = 0; _counter < sizeof(lcd); _counter++) {", elementId, SmartLine::increase));
		result.append(SmartLine("lcd_copy[_counter] = lcd[_counter];", elementId));
		result.append(SmartLine("}", elementId, SmartLine::decrease));
	}

	result.append(SmartLine("ecrobot_bmp2lcd(BMP_DATA_START(" + imageName + "), lcd, 100, 64);", elementId));

	if(numb){
		result.append(SmartLine("for(int _counter = 0; _counter < sizeof(lcd); _counter++) {", elementId, SmartLine::increase));
		result.append(SmartLine("lcd[_counter] = lcd[_counter] | lcd_copy[_counter];", elementId));
		result.append(SmartLine("}", elementId, SmartLine::decrease));
	}

	result.append(SmartLine("display_bitmap_copy(lcd, 100, 8, 0, 0);", elementId));
	result.append(SmartLine("display_update();", elementId));

	return result;
}
