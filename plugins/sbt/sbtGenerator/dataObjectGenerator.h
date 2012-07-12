#pragma once
#include "QString"
#include "QList"

#include "../../../qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"
#include "../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"
#include "../../../qrutils/generator/abstractGenerator.h"

using namespace qReal;
using namespace utils;

class DataObjectGenerator : public AbstractGenerator
{
public:
  DataObjectGenerator(QString const &templateDirPath
					, QString const &outputDirPath
					, qReal::LogicalModelAssistInterface const &logicalModel
					, qReal::ErrorReporterInterface &errorReporter
					);
  void generate();
  QList<QString> getFiles();

private:
QList<QString> mFiles;
};

