var mainWindow = api.ui().mainWindow();
assert(mainWindow != null);
api.changeWindow(mainWindow);
api.wait(500);
var menuTools = api.ui().getMenu("menuTools");
assert(menuTools != null);

api.ui().activateMenu(menuTools);
api.wait(200);

var actionGesturesShow = api.ui().getActionInMenu(menuTools, "actionGesturesShow");
assert(actionGesturesShow != null);
assert(api.ui().isEnabledAndVisible(actionGesturesShow));
assert(!api.ui().actionIsCheckable(actionGesturesShow));
assert(!api.ui().actionIsChecked(actionGesturesShow));
assert(!api.ui().isSubMenuInMenu(menuTools, actionGesturesShow));

//closeExpectedDialog(mainWindow, "Gestures Show", 3000); ?
//api.ui().activateMenuAction(menuTools, actionGesturesShow);
//api.wait(500);
