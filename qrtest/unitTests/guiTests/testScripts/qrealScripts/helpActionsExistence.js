var mainWindow = api.ui().mainWindow();
assert(mainWindow != null);
api.changeWindow(mainWindow);
api.wait(500);
var menuHelp = api.ui().getMenu("menuHelp");
assert(menuHelp != null);

api.ui().activateMenu(menuHelp);
api.wait(200);

var actionHelp = api.ui().getActionInMenu(menuHelp, "actionHelp");
assert(actionHelp != null);
assert(api.ui().isEnabledAndVisible(actionHelp));
assert(!api.ui().actionIsCheckable(actionHelp));
assert(!api.ui().actionIsChecked(actionHelp));
assert(!api.ui().isSubMenuInMenu(menuHelp, actionHelp));

var actionAbout = api.ui().getActionInMenu(menuHelp, "actionAbout");
assert(actionAbout != null);
assert(api.ui().isEnabledAndVisible(actionAbout));
assert(!api.ui().actionIsCheckable(actionAbout));
assert(!api.ui().actionIsChecked(actionAbout));
assert(!api.ui().isSubMenuInMenu(menuHelp, actionAbout));

var actionAboutQt = api.ui().getActionInMenu(menuHelp, "actionAboutQt");
assert(actionAboutQt != null);
assert(api.ui().isEnabledAndVisible(actionAboutQt));
assert(!api.ui().actionIsCheckable(actionAboutQt));
assert(!api.ui().actionIsChecked(actionAboutQt));
assert(!api.ui().isSubMenuInMenu(menuHelp, actionAboutQt));

//api.ui().activateMenuAction(menuHelp, actionHelp);
//api.wait(200);
