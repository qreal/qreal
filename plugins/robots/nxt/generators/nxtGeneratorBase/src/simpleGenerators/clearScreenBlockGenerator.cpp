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

#include "clearScreenBlockGenerator.h"

#include "src/nxtGeneratorCustomizer.h"
#include "nxtGeneratorBase/parts/images.h"

using namespace nxt::simple;

ClearScreenBlockGenerator::ClearScreenBlockGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, static_cast<NxtGeneratorFactory *>(customizer.factory())->images().bmpFilesCount()
					// WARNING: this may not work when we traverse here earlier than
					// into drawing block but it will be executed before this block
					? "drawing/clearUsedScreen.t"
					: "drawing/clearUnusedScreen.t"
			, {}, parent)
{
}
