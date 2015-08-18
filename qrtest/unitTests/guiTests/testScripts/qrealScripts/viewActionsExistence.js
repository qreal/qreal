var mainWindow = api.ui().mainWindow();
assert(mainWindow != null);
api.changeWindow(mainWindow);
api.wait(500);
var menu_View = api.ui().getMenu("menu_View");
assert(menu_View != null);

api.ui().activateMenu(menu_View);
api.wait(200);

var actionFullscreen = api.ui().getActionInMenu(menu_View, "actionFullscreen");
assert(actionFullscreen != null);
assert(api.ui().isEnabledAndVisible(actionFullscreen));
assert(!api.ui().actionIsCheckable(actionFullscreen));
assert(!api.ui().actionIsChecked(actionFullscreen));
assert(!api.ui().isSubMenuInMenu(menu_View, actionFullscreen));

var actionZoom_In = api.ui().getActionInMenu(menu_View, "actionZoom_In");
assert(actionZoom_In != null);
assert(!api.ui().isEnabledAndVisible(actionZoom_In));
assert(!api.ui().actionIsCheckable(actionZoom_In));
assert(!api.ui().actionIsChecked(actionZoom_In));
assert(!api.ui().isSubMenuInMenu(menu_View, actionZoom_In));

var actionZoom_Out = api.ui().getActionInMenu(menu_View, "actionZoom_Out");
assert(actionZoom_Out != null);
assert(!api.ui().isEnabledAndVisible(actionZoom_Out));
assert(!api.ui().actionIsCheckable(actionZoom_Out));
assert(!api.ui().actionIsChecked(actionZoom_Out));
assert(!api.ui().isSubMenuInMenu(menu_View, actionZoom_Out));

var actionShowSplash = api.ui().getActionInMenu(menu_View, "actionShowSplash");
assert(actionShowSplash != null);
assert(api.ui().isEnabledAndVisible(actionShowSplash));
assert(api.ui().actionIsCheckable(actionShowSplash));
assert(api.ui().actionIsChecked(actionShowSplash));
assert(!api.ui().isSubMenuInMenu(menu_View, actionShowSplash));

var actionMenuPanels = api.ui().getActionInMenu(menu_View, "Panels");
assert(actionMenuPanels != null);
assert(api.ui().isEnabledAndVisible(actionMenuPanels));
assert(!api.ui().actionIsCheckable(actionMenuPanels));
assert(!api.ui().actionIsChecked(actionMenuPanels));
assert(api.ui().isSubMenuInMenu(menu_View, actionMenuPanels));

api.ui().activateMenuAction(menu_View, actionMenuPanels);
api.wait(200);

var subMenuPanels = api.ui().getMenuContainedByAction(actionMenuPanels);
assert(subMenuPanels != null);

var actionMiniMap = api.ui().getActionInMenu(subMenuPanels, "Mini Map");
assert(actionMiniMap != null);
assert(api.ui().isEnabledAndVisible(actionMiniMap));
assert(api.ui().actionIsCheckable(actionMiniMap));
assert(api.ui().actionIsChecked(actionMiniMap));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionMiniMap));

var actionPalette = api.ui().getActionInMenu(subMenuPanels, "Palette");
assert(actionPalette != null);
assert(api.ui().isEnabledAndVisible(actionPalette));
assert(api.ui().actionIsCheckable(actionPalette));
assert(api.ui().actionIsChecked(actionPalette));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionPalette));


var actionLogicalExplorer = api.ui().getActionInMenu(subMenuPanels, "Logical Model Explorer");
assert(actionLogicalExplorer != null);
assert(api.ui().isEnabledAndVisible(actionLogicalExplorer));
assert(api.ui().actionIsCheckable(actionLogicalExplorer));
assert(api.ui().actionIsChecked(actionLogicalExplorer));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionLogicalExplorer));


var actionErrors = api.ui().getActionInMenu(subMenuPanels, "Errors");
assert(actionErrors != null);
assert(api.ui().isEnabledAndVisible(actionErrors));
assert(api.ui().actionIsCheckable(actionErrors));
assert(!api.ui().actionIsChecked(actionErrors));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionErrors));


var actionGraphicalExplorer = api.ui().getActionInMenu(subMenuPanels, "Graphical Model Explorer");
assert(actionGraphicalExplorer != null);
assert(api.ui().isEnabledAndVisible(actionGraphicalExplorer));
assert(api.ui().actionIsCheckable(actionGraphicalExplorer));
assert(api.ui().actionIsChecked(actionGraphicalExplorer));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionGraphicalExplorer));


var actionProperty = api.ui().getActionInMenu(subMenuPanels, "Property Editor");
assert(actionProperty != null);
assert(api.ui().isEnabledAndVisible(actionProperty));
assert(api.ui().actionIsCheckable(actionProperty));
assert(api.ui().actionIsChecked(actionProperty));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionProperty));


var actionFileToolbar = api.ui().getActionInMenu(subMenuPanels, "File Toolbar");
assert(actionFileToolbar != null);
assert(api.ui().isEnabledAndVisible(actionFileToolbar));
assert(api.ui().actionIsCheckable(actionFileToolbar));
assert(api.ui().actionIsChecked(actionFileToolbar));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionFileToolbar));


var actionEditToolbar = api.ui().getActionInMenu(subMenuPanels, "Edit Toolbar");
assert(actionEditToolbar != null);
assert(api.ui().isEnabledAndVisible(actionEditToolbar));
assert(api.ui().actionIsCheckable(actionEditToolbar));
assert(api.ui().actionIsChecked(actionEditToolbar));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionEditToolbar));


var actionViewToolbar = api.ui().getActionInMenu(subMenuPanels, "View Toolbar");
assert(actionViewToolbar != null);
assert(api.ui().isEnabledAndVisible(actionViewToolbar));
assert(api.ui().actionIsCheckable(actionViewToolbar));
assert(api.ui().actionIsChecked(actionViewToolbar));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionViewToolbar));


var actionInterpreterToolbar = api.ui().getActionInMenu(subMenuPanels, "Interpreter Toolbar");
assert(actionInterpreterToolbar != null);
assert(api.ui().isEnabledAndVisible(actionInterpreterToolbar));
assert(api.ui().actionIsCheckable(actionInterpreterToolbar));
assert(!api.ui().actionIsChecked(actionInterpreterToolbar));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionInterpreterToolbar));


var actionGeneratorsToolbar = api.ui().getActionInMenu(subMenuPanels, "Generators Toolbar");
assert(actionGeneratorsToolbar != null);
assert(api.ui().isEnabledAndVisible(actionGeneratorsToolbar));
assert(api.ui().actionIsCheckable(actionGeneratorsToolbar));
assert(!api.ui().actionIsChecked(actionGeneratorsToolbar));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionGeneratorsToolbar));
