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
	editorPluginInterface \
	toolPluginInterface \
	interpretedPluginInterface \
	thirdparty \

pluginManager.subdir = $$PWD/plugins/pluginManager
editorPluginInterface.subdir = $$PWD/plugins/editorPluginInterface
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
