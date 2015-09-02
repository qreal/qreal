init();

var menuTools = api.ui().getMenu("menuTools");
assert(menuTools != null);

api.ui().activateMenu(menuTools);
api.wait(200);

var actionGesturesShow = api.ui().getActionInMenu(menuTools, "actionGesturesShow");
checkAction(actionGesturesShow, true, false, false);
assert(!api.ui().isSubMenuInMenu(menuTools, actionGesturesShow));

//closeExpectedDialog(mainWindow, "Gestures Show", 3000); ?
//api.ui().activateMenuAction(menuTools, actionGesturesShow);
//api.wait(500);
