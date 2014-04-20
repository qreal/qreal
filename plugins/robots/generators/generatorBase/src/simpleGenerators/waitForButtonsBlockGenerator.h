#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'WaitForButtons' block
class WaitForButtonsBlockGenerator : public BindingGenerator
{
public:
	WaitForButtonsBlockGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);

private:
	bool enterUsed(qrRepo::RepoApi const &repo, qReal::Id const &id) const;
	bool runUsed(qrRepo::RepoApi const &repo, qReal::Id const &id) const;
	QString templatePath(qrRepo::RepoApi const &repo, qReal::Id const &id) const;
};

}
}
