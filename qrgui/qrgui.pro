# Copyright 2007-2015 QReal Research Group
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

TEMPLATE = subdirs

SUBDIRS += \
	mainWindow \
	systemFacade \
	models \
	editor \
	controller \
	dialogs \
	preferencesDialog \
	textEditor \
	mouseGestures \
	hotKeyManager \
	brandManager \
	pluginManager \
	metaMetaModel \
	toolPluginInterface \
	interpretedPluginInterface \
	thirdparty \

pluginManager.subdir = $$PWD/plugins/pluginManager
metaMetaModel.subdir = $$PWD/plugins/metaMetaModel
toolPluginInterface.subdir = $$PWD/plugins/toolPluginInterface
interpretedPluginInterface.subdir = $$PWD/plugins/interpretedPluginInterface

mainWindow.depends = \
	systemFacade \
	editor \
	controller \
	dialogs \
	preferencesDialog \
	textEditor \
	mouseGestures \
	hotKeyManager \
	brandManager \
	thirdparty \

systemFacade.depends = \
	models \
	textEditor \
	pluginManager \
	toolPluginInterface \

models.depends = \
	pluginManager \
	controller \

editor.depends = \
	models \
	controller \
	mouseGestures \
	brandManager \
	pluginManager \
	thirdparty \
	dialogs \

dialogs.depends = \
	models \
	thirdparty \

textEditor.depends = \
	toolPluginInterface \

hotKeyManager.depends = \
	preferencesDialog \

brandManager.depends = \
	pluginManager \

pluginManager.depends = \
	toolPluginInterface \
