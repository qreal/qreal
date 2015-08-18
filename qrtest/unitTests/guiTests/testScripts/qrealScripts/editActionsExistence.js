var mainWindow = api.ui().mainWindow();
assert(mainWindow != null);
api.changeWindow(mainWindow);
api.wait(500);
var menuEdit = api.ui().getMenu("menuEdit");
assert(menuEdit != null);

api.ui().activateMenu(menuEdit);
api.wait(200);

var actionUndo = api.ui().getActionInMenu(menuEdit, "actionUndo");
assert(actionUndo != null);
assert(!api.ui().isEnabledAndVisible(actionUndo));
assert(!api.ui().actionIsCheckable(actionUndo));
assert(!api.ui().actionIsChecked(actionUndo));
assert(!api.ui().isSubMenuInMenu(menuEdit, actionUndo));

var actionRedo = api.ui().getActionInMenu(menuEdit, "actionRedo");
assert(actionRedo != null);
assert(!api.ui().isEnabledAndVisible(actionRedo));
assert(!api.ui().actionIsCheckable(actionRedo));
assert(!api.ui().actionIsChecked(actionRedo));
assert(!api.ui().isSubMenuInMenu(menuEdit, actionRedo));

var actionFind = api.ui().getActionInMenu(menuEdit, "actionFind");
assert(actionFind != null);
assert(!api.ui().isEnabledAndVisible(actionFind));
assert(!api.ui().actionIsCheckable(actionFind));
assert(!api.ui().actionIsChecked(actionFind));
assert(!api.ui().isSubMenuInMenu(menuEdit, actionFind));

//api.ui().activateMenuAction(menuEdit, actionFind);
