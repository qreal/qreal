#pragma once

#include "bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for 'WaitForButtons' block
class WaitForButtonsBlockGenerator : public BindingGenerator
{
public:
	WaitForButtonsBlockGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);

private:
	bool enterUsed(qrRepo::RepoApi const &repo, Id const &id) const;
	bool runUsed(qrRepo::RepoApi const &repo, Id const &id) const;
	QString templatePath(qrRepo::RepoApi const &repo, Id const &id) const;
};

}
}
}
}
