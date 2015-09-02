init();

var menuSettings = api.ui().getMenu("menuSettings");
assert(menuSettings != null);

api.ui().activateMenu(menuSettings);
api.wait(200);

var actionPreferences = api.ui().getActionInMenu(menuSettings, "actionPreferences");
checkAction(actionPreferences, true, false, false);
assert(!api.ui().isSubMenuInMenu(menuSettings, actionPreferences));

var actionShow_grid = api.ui().getActionInMenu(menuSettings, "actionShow_grid");
checkAction(actionShow_grid, true, true, true);
assert(!api.ui().isSubMenuInMenu(menuSettings, actionShow_grid));

var actionShow_alignment = api.ui().getActionInMenu(menuSettings, "actionShow_alignment");
checkAction(actionShow_alignment, true, true, false);
assert(!api.ui().isSubMenuInMenu(menuSettings, actionShow_alignment));

var actionSwitch_on_grid = api.ui().getActionInMenu(menuSettings, "actionSwitch_on_grid");
checkAction(actionSwitch_on_grid, true, true, true);
assert(!api.ui().isSubMenuInMenu(menuSettings, actionSwitch_on_grid));

var actionSwitch_on_alignment = api.ui().getActionInMenu(menuSettings, "actionSwitch_on_alignment");
checkAction(actionSwitch_on_alignment, true, true, false);
assert(!api.ui().isSubMenuInMenu(menuSettings, actionSwitch_on_alignment));

var actionShow_all_text = api.ui().getActionInMenu(menuSettings, "actionShow_all_text");
checkAction(actionShow_all_text, true, true, true);
assert(!api.ui().isSubMenuInMenu(menuSettings, actionShow_all_text));
