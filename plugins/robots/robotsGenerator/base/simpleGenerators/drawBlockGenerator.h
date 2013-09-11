#pragma once

#include <QtGui/QImage>
#include <QtGui/QPainter>

#include "bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

class DrawBlockGenerator : public BindingGenerator
{
public:
	DrawBlockGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);

	virtual QString generate();

protected:
	virtual void drawBmp(QPainter *painter) = 0;

private:
	QString imageName(GeneratorCustomizer &customizer) const;
};

}
}
}
}
