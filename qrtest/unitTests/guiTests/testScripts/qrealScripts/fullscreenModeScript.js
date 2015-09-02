init();

var menu_View = api.ui().getMenu("menu_View");
assert(menu_View != null);

api.ui().activateMenu(menu_View);
api.wait(200);

var actionFullscreen = api.ui().getActionInMenu(menu_View, "actionFullscreen");
api.ui().activateMenuAction(menu_View, actionFullscreen);
api.wait(300);

api.ui().activateMenu(menu_View);
api.wait(200);

//checks the fullscreen states
var actionMenuPanels = api.ui().getActionInMenu(menu_View, "Panels");
checkAction(actionMenuPanels, true, false, false);
assert(api.ui().isSubMenuInMenu(menu_View, actionMenuPanels));

var subMenuPanels = api.ui().getMenuContainedByAction(actionMenuPanels);
assert(subMenuPanels != null);

var actionMiniMap = api.ui().getActionInMenu(subMenuPanels, "Mini Map");
checkAction(actionMiniMap, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionMiniMap));

var actionPalette = api.ui().getActionInMenu(subMenuPanels, "Palette");
checkAction(actionPalette, true, true, true);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionPalette));

var actionLogicalExplorer = api.ui().getActionInMenu(subMenuPanels, "Logical Model Explorer");
checkAction(actionLogicalExplorer, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionLogicalExplorer));

var actionErrors = api.ui().getActionInMenu(subMenuPanels, "Errors");
checkAction(actionErrors, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionErrors));

var actionGraphicalExplorer = api.ui().getActionInMenu(subMenuPanels, "Graphical Model Explorer");
checkAction(actionGraphicalExplorer, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionGraphicalExplorer));

var actionProperty = api.ui().getActionInMenu(subMenuPanels, "Property Editor");
checkAction(actionProperty, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionProperty));

var actionFileToolbar = api.ui().getActionInMenu(subMenuPanels, "File Toolbar");
checkAction(actionFileToolbar, true, true, true);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionFileToolbar));

var actionEditToolbar = api.ui().getActionInMenu(subMenuPanels, "Edit Toolbar");
checkAction(actionEditToolbar, true, true, true);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionEditToolbar));

var actionViewToolbar = api.ui().getActionInMenu(subMenuPanels, "View Toolbar");
checkAction(actionViewToolbar, true, true, true);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionViewToolbar));

var actionInterpreterToolbar = api.ui().getActionInMenu(subMenuPanels, "Interpreter Toolbar");
checkAction(actionInterpreterToolbar, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionInterpreterToolbar));

var actionGeneratorsToolbar = api.ui().getActionInMenu(subMenuPanels, "Generators Toolbar");
checkAction(actionGeneratorsToolbar, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionGeneratorsToolbar));

api.ui().activateMenuAction(menu_View, actionFullscreen);
api.wait(300);

api.ui().activateMenu(menu_View);
api.wait(200);

//checks the primordial states
actionMenuPanels = api.ui().getActionInMenu(menu_View, "Panels");
checkAction(actionMenuPanels, true, false, false);
assert(api.ui().isSubMenuInMenu(menu_View, actionMenuPanels));

subMenuPanels = api.ui().getMenuContainedByAction(actionMenuPanels);
assert(subMenuPanels != null);

actionMiniMap = api.ui().getActionInMenu(subMenuPanels, "Mini Map");
checkAction(actionMiniMap, true, true, true);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionMiniMap));

actionPalette = api.ui().getActionInMenu(subMenuPanels, "Palette");
checkAction(actionPalette, true, true, true);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionPalette));

actionLogicalExplorer = api.ui().getActionInMenu(subMenuPanels, "Logical Model Explorer");
checkAction(actionLogicalExplorer, true, true, true);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionLogicalExplorer));

actionErrors = api.ui().getActionInMenu(subMenuPanels, "Errors");
checkAction(actionErrors, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionErrors));

actionGraphicalExplorer = api.ui().getActionInMenu(subMenuPanels, "Graphical Model Explorer");
checkAction(actionGraphicalExplorer, true, true, true);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionGraphicalExplorer));

actionProperty = api.ui().getActionInMenu(subMenuPanels, "Property Editor");
checkAction(actionProperty, true, true, true);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionProperty));

actionFileToolbar = api.ui().getActionInMenu(subMenuPanels, "File Toolbar");
checkAction(actionFileToolbar, true, true, true);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionFileToolbar));

actionEditToolbar = api.ui().getActionInMenu(subMenuPanels, "Edit Toolbar");
checkAction(actionEditToolbar, true, true, true);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionEditToolbar));

actionViewToolbar = api.ui().getActionInMenu(subMenuPanels, "View Toolbar");
checkAction(actionViewToolbar, true, true, true);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionViewToolbar));

actionInterpreterToolbar = api.ui().getActionInMenu(subMenuPanels, "Interpreter Toolbar");
checkAction(actionViewToolbar, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionInterpreterToolbar));

actionGeneratorsToolbar = api.ui().getActionInMenu(subMenuPanels, "Generators Toolbar");
checkAction(actionViewToolbar, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionGeneratorsToolbar));

// try to turn on errors dockwidget and turn off palette dockwidget
api.ui().activateMenuAction(menu_View, actionMenuPanels);
actionErrors = api.ui().getActionInMenu(subMenuPanels, "Errors");
api.ui().activateMenuAction(subMenuPanels, actionErrors);
api.ui().activateMenuAction(menu_View, actionMenuPanels);
actionPalette = api.ui().getActionInMenu(subMenuPanels, "Palette");
api.ui().activateMenuAction(subMenuPanels, actionPalette);

// changes the fullscreen mode and checks the "Error" action
api.ui().activateMenu(menu_View);
api.ui().activateMenuAction(menu_View, actionFullscreen);
api.wait(300);

api.ui().activateMenu(menu_View);
api.wait(200);

// changes the second time
api.ui().activateMenu(menu_View);
api.ui().activateMenuAction(menu_View, actionFullscreen);
api.wait(300);

api.ui().activateMenu(menu_View);
api.wait(200);

// checks the error state
// need repeatedly to call getMenuContainedByAction coz there the necessary signal is emitted
subMenuPanels = api.ui().getMenuContainedByAction(actionMenuPanels);
assert(subMenuPanels != null);
actionErrors = api.ui().getActionInMenu(subMenuPanels, "Errors");
checkAction(actionErrors, true, true, true);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionErrors));
actionPalette = api.ui().getActionInMenu(subMenuPanels, "Palette");
checkAction(actionPalette, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionPalette));
