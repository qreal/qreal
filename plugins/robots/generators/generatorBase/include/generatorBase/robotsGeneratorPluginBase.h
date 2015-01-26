#pragma once

#include <QtWidgets/QApplication>

#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include <qrgui/textEditor/languageInfo.h>
#include <qrrepo/repoApi.h>

#include "generatorKitPluginInterface.h"
#include "robotsGeneratorDeclSpec.h"
#include "masterGeneratorBase.h"

namespace generatorBase {

class ROBOTS_GENERATOR_EXPORT RobotsGeneratorPluginBase : public QObject, public GeneratorKitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(generatorBase::GeneratorKitPluginInterface)

public:
	RobotsGeneratorPluginBase();

	void init(qReal::PluginConfigurator const &configurator
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage) override;

protected slots:
	/// Calls code generator. Returns true if operation was successful.
	/// @param openTab If true after code generation a tab with generated code will be opened.
	virtual bool generateCode(bool openTab = true);

	/// Changes path to code source file and regenerate necessary extra files.
	void regenerateCode(qReal::Id const &diagram, QFileInfo const &oldFileInfo, QFileInfo const &newFileInfo);

	/// Adds new path to another code source file for chosen diagram
	void addNewCode(qReal::Id const &diagram, QFileInfo const &fileInfo);

	/// Remove diagram with paths to code source files.
	void removeDiagram(qReal::Id const &diagram);

	void removeCode(QFileInfo const &fileInfo);

protected:
	/// Override must return a link to concrete master generator instance for
	/// developped plugin. Caller takes ownership so override may forget about it.
	virtual MasterGeneratorBase *masterGenerator() = 0;

	virtual void regenerateExtraFiles(QFileInfo const &newFileInfo) = 0;
	QFileInfo srcPath();
	virtual QString defaultFilePath(QString const &project) const;
	QFileInfo generateCodeForProcessing();
	QString generatorName() const override;

	/// Returns an information about the language code on which will be generated;
	/// this information will be used by text editors when user will be edit the generated code.
	virtual qReal::text::LanguageInfo language() const = 0;

	/// Returns default name for generated file.
	virtual QString defaultProjectName() const;

	virtual bool canGenerateTo(QString const &project);

	/// Interface of MainWindow
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;  // Does not have ownership

	/// Interface of project manager (allows to perform open/save activities)
	qReal::ProjectManagementInterface *mProjectManager; // Does not have ownership

	/// Control interface of the repository
	qrRepo::RepoApi const *mRepo;  // Does not have ownership

	interpreterBase::robotModel::RobotModelManagerInterface const *mRobotModelManager;
	qrtext::LanguageToolboxInterface *mTextLanguage;  // Does not have ownership

	QList<qReal::HotKeyActionInfo> mHotKeyActionInfos;
	qReal::SystemEvents *mSystemEvents; // Does not have ownership
	qReal::TextManagerInterface *mTextManager;
	QMultiHash<qReal::Id, QFileInfo> mCodePath;
};

}
