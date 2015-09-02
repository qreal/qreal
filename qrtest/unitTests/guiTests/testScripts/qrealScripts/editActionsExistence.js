init();

var menuEdit = api.ui().getMenu("menuEdit");
assert(menuEdit != null);

api.ui().activateMenu(menuEdit);
api.wait(200);

var actionUndo = api.ui().getActionInMenu(menuEdit, "actionUndo");
checkAction(actionUndo, false, false, false);
assert(!api.ui().isSubMenuInMenu(menuEdit, actionUndo));

var actionRedo = api.ui().getActionInMenu(menuEdit, "actionRedo");
checkAction(actionRedo, false, false, false);
assert(!api.ui().isSubMenuInMenu(menuEdit, actionRedo));

var actionFind = api.ui().getActionInMenu(menuEdit, "actionFind");
checkAction(actionFind, false, false, false);
assert(!api.ui().isSubMenuInMenu(menuEdit, actionFind));
