#include "backgroundColorConverter.h"

using namespace trik::converters;

BackgroundColorConverter::BackgroundColorConverter(QString const &pathToTemplates)
	: generatorBase::converters::EnumConverterBase(pathToTemplates)
{
	addMapping(QString::fromUtf8("Белый"), "backgroundColors/white.t");
	addMapping(QString::fromUtf8("Чёрный"), "backgroundColors/black.t");
	addMapping(QString::fromUtf8("Красный"), "backgroundColors/red.t");
	addMapping(QString::fromUtf8("Тёмно-красный"), "backgroundColors/darkRed.t");
	addMapping(QString::fromUtf8("Зелёный"), "backgroundColors/green.t");
	addMapping(QString::fromUtf8("Тёмно-зелёный"), "backgroundColors/darkGreen.t");
	addMapping(QString::fromUtf8("Синий"), "backgroundColors/blue.t");
	addMapping(QString::fromUtf8("Тёмно-синий"), "backgroundColors/darkBlue.t");
	addMapping(QString::fromUtf8("Сине-зелёный"), "backgroundColors/cyan.t");
	addMapping(QString::fromUtf8("Тёмносине-зелёный"), "backgroundColors/darkCyan.t");
	addMapping(QString::fromUtf8("Пурпурный"), "backgroundColors/magenta.t");
	addMapping(QString::fromUtf8("Тёмно-пурпурный"), "backgroundColors/darkMagenta.t");
	addMapping(QString::fromUtf8("Жёлтый"), "backgroundColors/yellow.t");
	addMapping(QString::fromUtf8("Тёмно-жёлтый"), "backgroundColors/darkYellow.t");
	addMapping(QString::fromUtf8("Серый"), "backgroundColors/gray.t");
	addMapping(QString::fromUtf8("Тёмно-серый"), "backgroundColors/darkGray.t");
	addMapping(QString::fromUtf8("Светло-серый"), "backgroundColors/lightGray.t");
}
