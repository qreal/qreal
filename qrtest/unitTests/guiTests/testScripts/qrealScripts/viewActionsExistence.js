init();

var menu_View = api.ui().getMenu("menu_View");
assert(menu_View != null);

api.ui().activateMenu(menu_View);
api.wait(200);

var actionFullscreen = api.ui().getActionInMenu(menu_View, "actionFullscreen");
checkAction(actionFullscreen, true, false, false);
assert(!api.ui().isSubMenuInMenu(menu_View, actionFullscreen));

var actionZoom_In = api.ui().getActionInMenu(menu_View, "actionZoom_In");
checkAction(actionZoom_In, false, false, false);
assert(!api.ui().isSubMenuInMenu(menu_View, actionZoom_In));

var actionZoom_Out = api.ui().getActionInMenu(menu_View, "actionZoom_Out");
checkAction(actionZoom_Out, false, false, false);
assert(!api.ui().isSubMenuInMenu(menu_View, actionZoom_Out));

var actionShowSplash = api.ui().getActionInMenu(menu_View, "actionShowSplash");
checkAction(actionShowSplash, true, true, false);
assert(!api.ui().isSubMenuInMenu(menu_View, actionShowSplash));

var actionMenuPanels = api.ui().getActionInMenu(menu_View, "Panels");
checkAction(actionMenuPanels, true, false, false);
assert(api.ui().isSubMenuInMenu(menu_View, actionMenuPanels));

api.ui().activateMenuAction(menu_View, actionMenuPanels);
api.wait(200);

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
