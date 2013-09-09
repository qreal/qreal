#include "generator.h"
#include "../../../qrutils/nameNormalizer.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QMessageBox>

using namespace ubiq::generator;
using namespace qReal;
using namespace utils;

QString const templateDir = "./templates";
QString const templateFileNameForms = "qUbiqForms.cs";
QString const templateFileNameVariables = "qUbiqVariables.cs";
QString const templateFileNameCSProj = "qUbiqCSProject.csproj";
QString const templateFileNameAppConfig = "qUbiqApp.config";

/// Generation target file

Generator::Generator(QString const &outputDirPath
		, QString const &programName
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
	: AbstractGenerator(templateDir, QString(outputDirPath + "/" + programName).replace("\\", "/"), logicalModel, errorReporter)
{
}

Generator::~Generator()
{
}

void Generator::generate()
{
    QMessageBox msgBox;
    msgBox.setText("generate executed");
    msgBox.setInformativeText("ubiq generators 'generate()'");
    msgBox.exec();
}
