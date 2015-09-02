init();

var menuHelp = api.ui().getMenu("menuHelp");
assert(menuHelp != null);

api.ui().activateMenu(menuHelp);
api.wait(200);

var actionHelp = api.ui().getActionInMenu(menuHelp, "actionHelp");
checkAction(actionHelp, true, false, false);
assert(!api.ui().isSubMenuInMenu(menuHelp, actionHelp));

var actionAbout = api.ui().getActionInMenu(menuHelp, "actionAbout");
checkAction(actionAbout, true, false, false);
assert(!api.ui().isSubMenuInMenu(menuHelp, actionAbout));

var actionAboutQt = api.ui().getActionInMenu(menuHelp, "actionAboutQt");
checkAction(actionAboutQt, true, false, false);
assert(!api.ui().isSubMenuInMenu(menuHelp, actionAboutQt));
