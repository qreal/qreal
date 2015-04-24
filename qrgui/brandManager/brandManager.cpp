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

#include "brandManager.h"

using namespace qReal;

BrandManager::BrandManager()
	: mFonts(new Fonts)
	, mStyles(new Styles(*mFonts))
{
}

BrandManager::~BrandManager()
{
	delete mFonts;
	delete mStyles;
}

BrandManager &BrandManager::instance()
{
	static BrandManager instance;
	return instance;
}

void BrandManager::configure(const ToolPluginManager *toolPluginManager)
{
	instance().mCustomizer = toolPluginManager->customizer();
}

const Fonts *BrandManager::fonts()
{
	return instance().mFonts;
}

const Styles *BrandManager::styles()
{
	return instance().mStyles;
}

QString BrandManager::applicationName()
{
	return instance().mCustomizer->windowTitle();
}

const QImage BrandManager::applicationLogo()
{
	return instance().mCustomizer->applicationLogo();
}

const QIcon BrandManager::applicationIcon()
{
	return instance().mCustomizer->applicationIcon();
}
