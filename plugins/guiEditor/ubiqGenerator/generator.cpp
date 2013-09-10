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
}

Generator::~Generator()
{
}

void Generator::generate()
{

    initGeneratingFiles();

    generateMainSwitch();

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

void Generator::generateMainSwitch()
{
    IdList toGenerate;
    toGenerate << mApi.elementsByType("Frame");

    QString outerSwitchBody = "";
    foreach (Id const &curFrame, toGenerate) {
        if (!mApi.isLogicalElement(curFrame)) {
            continue;
        }

        QString currentFrameCase = mTemplateUtils["@@eachFrameSwitchBody@@"];
        QString currentFrameName = mApi.property(curFrame, "name").toString();

        currentFrameCase.replace("@@frameName@@", currentFrameName);
        outerSwitchBody += currentFrameCase;
    }

    mResultTestFile.replace("@@eachFrameSwitches@@", outerSwitchBody);
}
