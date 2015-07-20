/* Copyright 2014 - 2015 CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "guiTests.h"

#include <QtScript/QScriptContext>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QtCore/QCoreApplication>
#include <QEventLoop>
#include <QTimer>
#include <QThread>
#include <QWidget>
#include <QDebug>

using namespace guiTesting;
using namespace qReal;

// мб добавить assertnotnull для объектов в скрипте

QScriptValue scriptAssert(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 1) {
		ADD_FAILURE() << "'assert' shall have exactly one argument";
		return {};
	}

	if (!context->argument(0).toBool()) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString(); // mid(1)?
	}

	return {};
}

void wait(int duration) // а надо ли копипастить?
{
	QEventLoop eventLoop;
	if (duration != -1) {
		QTimer::singleShot(duration, &eventLoop, SLOT(quit()));
	}

	eventLoop.exec();
}

void guiTests::SetUp() // возможно эти строчки следует поместить для сетапа для всех тестов
// возможно стоит вставить проверку на некорректно составленный тест в скрипте с помощью QScriptSyntaxCheckResult
{
	QString program = QApplication::applicationFilePath() + "/../qreal-d.exe";
	QStringList arguments;
	QProcess *qrealInstance = new QProcess();
	// qrealInstance->waitForStarted(30000); // надо ли блокировать сигналы?
	qrealInstance->start(program, arguments);

	wait(10000);
qDebug() << "wtf";
	foreach (QWidget *widget, qrealInstance) {
		qDebug() << widget->windowTitle();
		if (widget->windowTitle() == QString("QReal")) {
			qDebug() << "YOLO";
		   /* widget->hide();
			wait(1000);
			widget->show();
			wait(1000);
			widget->hide();
			wait(1000);
			widget->show();*/
		}

	}
	//MainWidnowScriptAPIInterface *mMainWidnowScriptAPIInterface = qrealInstance->findChild<MainWidnowScriptAPIInterface *>("QReal");
	//if (mMainWidnowScriptAPIInterface == nullptr) {
	//	ADD_FAILURE() << "MainWindow is not found"; // можно ли сюда добавлять ассерты? (в сетап)
	//}
	//qrealInstance->dumpObjectTree();
	//while(qrealInstance->state() != 2){}; // костыль, надо бы и время учитывать
	// if (!qrealInstance->waitForStarted(10000)) {
	//	// как-то обработать (на случай если программа не запустилась или путь неверный)
	//}
	mQrealInstance = qrealInstance;

	// app.setAttribute(Qt::AA_Use96Dpi, true); // может понадобится на Xdisplay?
	mMainWidnowScriptAPIInterface->registerNewFunction(scriptAssert);
}

void guiTests::TearDown()
{
	mQrealInstance->terminate();
	// ниже надо написать блок об обработке ошибки выхода
	if (mQrealInstance->exitStatus() != QProcess::NormalExit) {
		ADD_FAILURE() << "Error code of creshed process: " << mQrealInstance->error(); // можно ли писать fail в teardown
	}
	delete mQrealInstance; // или deletelater?
}

void guiTests::run(const QString &script)
{
	mMainWidnowScriptAPIInterface->evaluateScript(script);
}

void guiTests::runFromFile(const QString &fileName1)
{	
	QString fileName = QApplication::applicationFilePath() + "/../../../qrtest/unitTests/guiTests/testScripts/" + fileName1;
	QFile scriptFile(fileName);
	if (!scriptFile.open(QIODevice::ReadOnly)) {
		// handle error
		// qDebug() << "Cant open file for reading for gui test: " << fileName.toStdString(); // может убрать?
		ADD_FAILURE() << "Cant open file for reading for gui test: " << fileName.toStdString();
	}
	QTextStream stream(&scriptFile);
	QString contents = stream.readAll();
	scriptFile.close();
	run(contents);
}

TEST_F(guiTests, sanityCheck)
{
	run("1 + 1");
	ASSERT_EQ(2, 1 + 1);
}

TEST_F(guiTests, guiTestExample)
{
	runFromFile("guiTestExample.js"); // мб qs?
}

TEST_F(guiTests, guiTestExampleInvalid)
{
	runFromFile("guiTestExampleInvalid.js");
}
