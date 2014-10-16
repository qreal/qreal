TEMPLATE = subdirs

SUBDIRS += \
	$$PWD/mainWindow \
	$$PWD/models \
	$$PWD/editor \
	$$PWD/controller \
	$$PWD/dialogs \
	$$PWD/preferencesDialog \
	$$PWD/textEditor \
	$$PWD/mouseGestures \
	$$PWD/hotKeyManager \
	$$PWD/brandManager \
	$$PWD/plugins/pluginManager \
	$$PWD/plugins/editorPluginInterface \
	$$PWD/plugins/toolPluginInterface \
	$$PWD/thirdparty \

mainWindow.depends = \
	models \
	editor \
	controller \
	dialogs \
	preferencesDialog \
	textEditor \
	hotKeyManager \
	brandManager \
	plugins/pluginManager \
	thirdparty \

models.depends = \
	plugins/pluginManager \

editor.depends = \
	models \
	controller \
	mouseGestures \
	brandManager \
	plugins/pluginManager \
	thirdparty \

dialogs.depends = \
	models \
	thirdparty \

textEditor.depends = \
	plugins/toolPluginInterface \

hotKeyManager.depends = \
	preferencesDialog \

brandManager.depends = \
	plugins/pluginManager \

editorPluginInterface.depends =
	plugins/toolPluginInterface
