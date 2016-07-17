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

#ifdef Q_WS_WIN
#include "mainwindow/windowsExternBrowser.h"
#endif

#ifdef Q_OS_LINUX
#include "mainwindow/linuxExternBrowser.h"
#endif

#ifdef Q_OS_MAC
#pragma message ("Mac OS is not needed, use Windows or Linux instead")
#include "mainwindow/macExternBrowser.h"
#endif
