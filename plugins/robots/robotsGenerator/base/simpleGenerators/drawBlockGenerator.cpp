#include "drawBlockGenerator.h"

using namespace qReal::robots::generators::simple;

DrawBlockGenerator::DrawBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "zopa.t", QList<Binding *>(), parent)
{
}

//QList<SmartLine_old> DrawBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
//		, qReal::Id const &elementId, qReal::Id const &logicElementId)
//{
//	QList<SmartLine_old> result;

//	int const numb = nxtGen->imageGenerator().bmpFilesCount();
//	QString const imageName = "Image_" + QString::number(numb);
//	nxtGen->imageGenerator().increaseBmpCounter();
//	nxtGen->imageGenerator().addBmpFileName(imageName);
//	nxtGen->variables().hasImages();

//	generateBmpFile(nxtGen, imageName, logicElementId);

//	if (numb) {
//		result.append(SmartLine_old("for (int _counter = 0; _counter < sizeof(lcd); ++_counter) {", elementId, SmartLine_old::increase));
//		result.append(SmartLine_old("lcd_copy[_counter] = lcd[_counter];", elementId));
//		result.append(SmartLine_old("}", elementId, SmartLine_old::decrease));
//	}

//	result.append(SmartLine_old("ecrobot_bmp2lcd(BMP_DATA_START(" + imageName + "), lcd, 100, 64);", elementId));

//	if (numb) {
//		result.append(SmartLine_old("for (int _counter = 0; _counter < sizeof(lcd); ++_counter) {", elementId, SmartLine_old::increase));
//		result.append(SmartLine_old("lcd[_counter] = lcd[_counter] | lcd_copy[_counter];", elementId));
//		result.append(SmartLine_old("}", elementId, SmartLine_old::decrease));
//	}

//	result.append(SmartLine_old("display_bitmap_copy(lcd, 100, 8, 0, 0);", elementId));
//	result.append(SmartLine_old("display_update();", elementId));

//	return result;
//}

//void DrawBlockGenerator::generateBmpFile(NxtOSEKRobotGenerator *nxtGen
//		, QString const &name, qReal::Id const &logicElementId)
//{
//	QImage image(displayWidth, displayHeight, QImage::Format_Mono);
//	QPainter painter(&image);
//	painter.eraseRect(0, 0, displayWidth, displayHeight);
//	drawBmp(nxtGen, logicElementId, &painter);
//	image.save("nxt-tools/example0/" + name + ".bmp", "BMP", -1);
//}
