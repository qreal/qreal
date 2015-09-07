/* Copyright 2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

// For easy writing of scripts
// ---------------------------
init();
var mainWindow;
var ui;
var utils;
var keyboard;
var palette;
var hints;
var scene;
var cursor;
// For easy writing of scripts
// ---------------------------

var menuSettings = ui.getMenu("menuSettings");
assert(menuSettings != null);

utils.activateMenu(menuSettings);
api.wait(200);

var actionPreferences = ui.getActionInMenu(menuSettings, "actionPreferences");
checkAction(actionPreferences, true, false, false);
expect(!ui.isSubMenuInMenu(menuSettings, actionPreferences));

var actionShow_grid = ui.getActionInMenu(menuSettings, "actionShow_grid");
checkAction(actionShow_grid, true, true, true);
expect(!ui.isSubMenuInMenu(menuSettings, actionShow_grid));

var actionShow_alignment = ui.getActionInMenu(menuSettings, "actionShow_alignment");
checkAction(actionShow_alignment, true, true, false);
expect(!ui.isSubMenuInMenu(menuSettings, actionShow_alignment));

var actionSwitch_on_grid = ui.getActionInMenu(menuSettings, "actionSwitch_on_grid");
checkAction(actionSwitch_on_grid, true, true, true);
expect(!ui.isSubMenuInMenu(menuSettings, actionSwitch_on_grid));

var actionSwitch_on_alignment = ui.getActionInMenu(menuSettings, "actionSwitch_on_alignment");
checkAction(actionSwitch_on_alignment, true, true, false);
expect(!ui.isSubMenuInMenu(menuSettings, actionSwitch_on_alignment));

var actionShow_all_text = ui.getActionInMenu(menuSettings, "actionShow_all_text");
checkAction(actionShow_all_text, true, true, true);
expect(!ui.isSubMenuInMenu(menuSettings, actionShow_all_text));
