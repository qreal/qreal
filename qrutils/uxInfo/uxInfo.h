#pragma once

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>

#include "../utilsDeclSpec.h"
#include "../../qrkernel/uxInfoInterface.h"

namespace utils {

class QRUTILS_EXPORT UXInfo : public qReal::UXInfoInterface {

public:
  static UXInfo* instance();
  ~UXInfo();

  static void reportCreation(QString const &editorName, QString const elementName);
  static void reportErrors(QString const &type, QString const &editorName, QString const &elementName, QString const &message);
  static void reportTotalTime(QString const &totalTime, int const &exitCode);
  static void reportMenuElements(QString const &elementName, QString const &status = "none");
  static void reportMouseClick(QPoint const &pos);
  void reportSettingsChanges(QString const &name, QVariant const &oldValue, QVariant const &newValue);
  static void setStatus(bool status);
  static void reportTestStarted();
  static void reportTestFinished();
private:
  UXInfo();

  void reportCreationOfElements(QString const &editorName, QString const elementName);
  void reportErrorsOfElements(QString const &type, QString const &editorName, QString const &elementName, QString const &message);
  void reportTotalTimeOfExec(QString const &totalTime, int const &exitCode);
  void reportMenuElementsUsing(QString const &elementName, QString const &status = "none");
  void reportMouseClickPosition(QPoint const &pos);
  void reportSettingsChangesInfo(const QString &name, const QString &oldValue, const QString &newValue);
  void setActualStatus(bool status);
  void reportTestStartedInfo();
  void reportTestFinishedInfo();
  QString currentDateTime();

  static UXInfo* object;
  bool mStatus;

  QFile mElementOnSceneCreationFile;
  static QTextStream elementOnSceneCreationStream;
  static int creationNumber;

  QFile mErrorReporterFile;
  static QTextStream errorReporterStream;
  static int errorReporterNumber;

  QFile mTotalTimeFile;
  static QTextStream totalTimeStream;

  QFile mMenuElementUsingFile;
  static QTextStream menuElementUsingStream;
  static int menuElementUsingNumber;

  QFile mMouseClickPositionFile;
  static QTextStream mouseClickPositionStream;
  static int mouseClickPositionNumber;

  QFile mSettingChangesFile;
  static QTextStream settingChangesStream;
  static int settingChangesNumber;

  static int testNumber;
};
}