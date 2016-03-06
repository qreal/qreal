/* Copyright 2015-2016 QReal Research Group, CyberTech Labs Ltd.
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

#pragma once

#include <QtCore/QStringList>

#include <gtest/gtest.h>
#include <qrgui/mainWindow/mainWindow.h>

namespace guiTesting {

class TestAgent;

/// Test suite for standard qReal GUI.
/// @warning Test suite can work incorrectly in hidden mode (e.g. scripts are running during ur work in Qt SDK).
/// @warning Segmantation fault in a single test crushes all tests.
/// @note Use common scripts files for easy script writing.
/// @note You can leave the application at the quit()-able state after an script execution
/// or quit application by itself using virtual mouse and virtual keyboard.
class QRealGuiTests : public testing::Test
{
protected:
	QRealGuiTests();

	/// SetUp and TearDown are calling before/after every test.
	void SetUp() override;
	void TearDown() override;

	/// Starts evaluating \a script.
	/// @note Should be invoked once for every TEST.
	void run(const QString &script);

	/// Adds common scripts in list of scripts evaluating before main script evaluating.
	/// @note You can include several needed files.
	/// @note All including names should be relative like "common.js".
	void includeCommonScript(const QString &relativeFileName);

	/// Adds common scripts in list of scripts evaluating before main script evaluating.
	/// @note You can include several needed files.
	/// @note All including names should be relative like "common.js".
	void includeCommonScript(const QStringList &fileList);

	/// Aborts evaluating and closes the program with the freeze code.
	void failTest();

	/// @returns folder name with .js scripts for the test suite.
	QString scriptFolderName() const;

	/// Sets up folder name in \a folder. Just the folder name (without the other path).
	void setScriptFolderName(const QString &folder);

	/// @returns upper time limit for main window exposing, openning.
	int timeToExpose() const;

	/// Sets up time limit for main window exposing, openning.
	void setTimeToExpose(const int timeToExpose);

	/// @returns upper time limit for every test.
	int timeLimit() const;

	/// Sets up timeLimit.
	void setTimeLimit(const int timeLimit);

private:
	/// This function is for running common scripts before main script.
	/// @note You can include several needed files.
	void runCommonScript(const QString &relativeFileName);

	void checkScriptSyntax(const QString &script, const QString &errorMsg);

	void checkLastEvaluating(const QString &errorMsg);

	QString readFile(const QString &relativeFileName) const;

	void exterminate(const int returnCode);

	QString mScriptFolderName;
	int mTimeToExpose;
	int mTimeLimit;

	/// Doesn't have ownership.
	qReal::MainWindow *mWindow;

	/// Has ownership.
	qReal::gui::ScriptingControlAPI *mScriptingControlAPI;

	/// Relative names of common scripts evaluating before main script evaluating.
	QStringList mCommonScripts;
	int mReturnCode;

	// for debugging and statistics
	QStringList mCurrentIncludedFiles;
	QString mCurrentTotalProgram;
	QString mCurrentEvaluatingScript;
	QString mCurrentEvaluatingScriptFile;

	// for QTimer connects
	QString mScript;
	QString mFileName;
	QString mRelativeName;

	// for debugging and scripting
	/// Doesn't have ownership.
	TestAgent *mTestAgent;
};

}
