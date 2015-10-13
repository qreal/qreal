/* Copyright 2007-2015 QReal Research Group
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

#include <QtGui/QImage>
#include <QtGui/QPainter>

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace nxt {
namespace simple {

/// A base for all generators for drawing blocks in NXT kit.
class DrawBlockGenerator : public generatorBase::simple::BindingGenerator
{
public:
	DrawBlockGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
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
