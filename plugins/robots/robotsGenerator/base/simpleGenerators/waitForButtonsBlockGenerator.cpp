#include "waitForButtonsBlockGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

WaitForButtonsBlockGenerator::WaitForButtonsBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, templatePath(repo, id), QList<Binding *>()
			<< Binding::createConverting("@@ENTER_CLICKS@@", "CentralButtonClicks"
					, customizer.factory()->intPropertyConverter())
			<< Binding::createConverting("@@RUN_CLICKS@@", "LeftButtonClicks"
					, customizer.factory()->intPropertyConverter())
			, parent)
{
	if (enterUsed(repo, id)) {
		QString const enterDeclaration = readTemplate("buttons/enterVariableDeclaration.t");
		customizer.factory()->variables()->appendManualDeclaration(enterDeclaration);
	}

	if (runUsed(repo, id)) {
		QString const runDeclaration = readTemplate("buttons/runVariableDeclaration.t");
		customizer.factory()->variables()->appendManualDeclaration(runDeclaration);
	}
}

bool WaitForButtonsBlockGenerator::enterUsed(qrRepo::RepoApi const &repo, Id const &id) const
{
	QString const enterButtonClicks = repo.property(id, "CentralButtonClicks").toString();
	return !enterButtonClicks.isEmpty() && enterButtonClicks.trimmed() != "0";
}

bool WaitForButtonsBlockGenerator::runUsed(qrRepo::RepoApi const &repo, Id const &id) const
{
	QString const runButtonClicks = repo.property(id, "LeftButtonClicks").toString();
	return !runButtonClicks.isEmpty() && runButtonClicks.trimmed() != "0";
}

QString WaitForButtonsBlockGenerator::templatePath(qrRepo::RepoApi const &repo, Id const &id) const
{
	bool const listenEnter = enterUsed(repo, id);
	bool const listenRun = runUsed(repo, id);

	if (!listenEnter && !listenRun) {
		return "buttons/doNotWaitButtons.t";
	}

	if (listenEnter && !listenRun) {
		return "buttons/waitForEnterOnly.t";
	}


	if (!listenEnter && listenRun) {
		return "buttons/waitForRunOnly.t";
	}

	return "buttons/waitForBothButtons.t";
}
