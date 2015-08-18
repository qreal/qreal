var mainWindow = api.ui().mainWindow();
assert(mainWindow != null);
api.changeWindow(mainWindow);
api.wait(500);
var menuSettings = api.ui().getMenu("menuSettings");
assert(menuSettings != null);

api.ui().activateMenu(menuSettings);
api.wait(200);

var actionPreferences = api.ui().getActionInMenu(menuSettings, "actionPreferences");
assert(actionPreferences != null);
assert(api.ui().isEnabledAndVisible(actionPreferences));
assert(!api.ui().actionIsCheckable(actionPreferences));
assert(!api.ui().actionIsChecked(actionPreferences));
assert(!api.ui().isSubMenuInMenu(menuSettings, actionPreferences));

var actionShow_grid = api.ui().getActionInMenu(menuSettings, "actionShow_grid");
assert(actionShow_grid != null);
assert(api.ui().isEnabledAndVisible(actionShow_grid));
assert(api.ui().actionIsCheckable(actionShow_grid));
assert(api.ui().actionIsChecked(actionShow_grid));
assert(!api.ui().isSubMenuInMenu(menuSettings, actionShow_grid));

var actionShow_alignment = api.ui().getActionInMenu(menuSettings, "actionShow_alignment");
assert(actionShow_alignment != null);
assert(api.ui().isEnabledAndVisible(actionShow_alignment));
assert(api.ui().actionIsCheckable(actionShow_alignment));
assert(!api.ui().actionIsChecked(actionShow_alignment));
assert(!api.ui().isSubMenuInMenu(menuSettings, actionShow_alignment));

var actionSwitch_on_grid = api.ui().getActionInMenu(menuSettings, "actionSwitch_on_grid");
assert(actionSwitch_on_grid != null);
assert(api.ui().isEnabledAndVisible(actionSwitch_on_grid));
assert(api.ui().actionIsCheckable(actionSwitch_on_grid));
assert(api.ui().actionIsChecked(actionSwitch_on_grid));
assert(!api.ui().isSubMenuInMenu(menuSettings, actionSwitch_on_grid));

var actionSwitch_on_alignment = api.ui().getActionInMenu(menuSettings, "actionSwitch_on_alignment");
assert(actionSwitch_on_alignment != null);
assert(api.ui().isEnabledAndVisible(actionSwitch_on_alignment));
assert(api.ui().actionIsCheckable(actionSwitch_on_alignment));
assert(!api.ui().actionIsChecked(actionSwitch_on_alignment));
assert(!api.ui().isSubMenuInMenu(menuSettings, actionSwitch_on_alignment));

var actionShow_all_text = api.ui().getActionInMenu(menuSettings, "actionShow_all_text");
assert(actionShow_all_text != null);
assert(api.ui().isEnabledAndVisible(actionShow_all_text));
assert(api.ui().actionIsCheckable(actionShow_all_text));
assert(api.ui().actionIsChecked(actionShow_all_text));
assert(!api.ui().isSubMenuInMenu(menuSettings, actionShow_all_text));


//api.ui().activateMenuAction(menuSettings, actionQuit);
//api.wait(200);
