TEMPLATE = subdirs

SUBDIRS += \
	mainWindow \
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
	thirdparty \

pluginManager.file = $$PWD/plugins/pluginManager/pluginManager.pro
toolPluginInterface.file = $$PWD/plugins/toolPluginInterface/toolPluginInterface.pro
editorPluginInterface.file = $$PWD/plugins/editorPluginInterface/editorPluginInterface.pro

mainWindow.depends = \
	models \
	editor \
	controller \
	dialogs \
	preferencesDialog \
	textEditor \
	hotKeyManager \
	brandManager \
	pluginManager \
	thirdparty \

models.depends = \
	pluginManager \

editor.depends = \
	models \
	controller \
	mouseGestures \
	brandManager \
	pluginManager \
	thirdparty \

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
