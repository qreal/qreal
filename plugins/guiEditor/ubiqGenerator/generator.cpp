#include "generator.h"
#include "../../../qrutils/nameNormalizer.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QMessageBox>

using namespace ubiq::generator;
using namespace qReal;
using namespace utils;

QString const templateDir = "./templates";
QString const templateFileNameTest = "testFile.cs";

/// Generation target file

Generator::Generator(QString const &outputDirPath
		, QString const &programName
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
    : AbstractGenerator(templateDir, QString("C:\\Users\\efimw_000\\Documents\\study\\").replace("\\", "/"), logicalModel, errorReporter)
{
    Q_UNUSED outputDirPath;
    Q_UNUSED programName;
}

Generator::~Generator()
{
}

void Generator::generate()
{
/*
    QMessageBox msgBox;
    msgBox.setText("Generator::generate() executed");
    msgBox.setInformativeText(outpudDirPath);
    msgBox.exec();
*/
    //IdList toGenerate;
    //toGenerate << mApi.elementsByType("Frame");

    initGeneratingFiles();
    saveGeneratedFiles();
}

void Generator::initGeneratingFiles()
{
    loadTemplateFromFile(templateFileNameTest, mResultTestFile);
}

void Generator::saveGeneratedFiles()
{
    QString outputFileName = "test.cs";

    saveOutputFile(outputFileName, mResultTestFile);
}
