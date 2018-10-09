/* Copyright 2017 CyberTech Labs Ltd.
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

#include "tofPart.h"

using namespace pioneer::lua;

TofPart::TofPart(const QStringList &pathsToTemplates)
	: InitTerminateCodeGenerator(pathsToTemplates)
{
}

void TofPart::reinit()
{
	mIsUsed = false;
}

QString TofPart::initCode()
{
	if (mIsUsed) {
		return readTemplateIfExists("initialization/tof.t");
	} else {
		return QString();
	}
}

QString TofPart::terminateCode()
{
	return QString();
}

void TofPart::registerUsage()
{
	mIsUsed = true;
}
