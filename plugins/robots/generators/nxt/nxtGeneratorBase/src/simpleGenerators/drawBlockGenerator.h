#pragma once

#include <QtGui/QImage>
#include <QtGui/QPainter>

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace nxt {
namespace simple {

/// A base for all generators for drawing blocks in NXT kit.
class DrawBlockGenerator : public generatorBase::simple::BindingGenerator
{
public:
	DrawBlockGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);

	virtual QString generate();

protected:
	/// Implementation must draw resulting picture using given painter
	virtual void drawBmp(QPainter *painter) = 0;

private:
	QString imageName(generatorBase::GeneratorCustomizer &customizer) const;
};

}
}
