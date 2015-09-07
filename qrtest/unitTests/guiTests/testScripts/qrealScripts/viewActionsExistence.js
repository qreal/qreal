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

var menu_View = ui.getMenu("menu_View");
assert(menu_View != null);

utils.activateMenu(menu_View);
api.wait(200);

var actionFullscreen = ui.getActionInMenu(menu_View, "actionFullscreen");
checkAction(actionFullscreen, true, false, false);
expect(!ui.isSubMenuInMenu(menu_View, actionFullscreen));

var actionZoom_In = ui.getActionInMenu(menu_View, "actionZoom_In");
checkAction(actionZoom_In, false, false, false);
expect(!ui.isSubMenuInMenu(menu_View, actionZoom_In));

var actionZoom_Out = ui.getActionInMenu(menu_View, "actionZoom_Out");
checkAction(actionZoom_Out, false, false, false);
expect(!ui.isSubMenuInMenu(menu_View, actionZoom_Out));

var actionShowSplash = ui.getActionInMenu(menu_View, "actionShowSplash");
checkAction(actionShowSplash, true, true, false);
expect(!ui.isSubMenuInMenu(menu_View, actionShowSplash));

var actionMenuPanels = ui.getActionInMenu(menu_View, "Panels");
checkAction(actionMenuPanels, true, false, false);
expect(ui.isSubMenuInMenu(menu_View, actionMenuPanels));

utils.activateMenuAction(menu_View, actionMenuPanels);
api.wait(200);

var subMenuPanels = ui.getMenuContainedByAction(actionMenuPanels);
assert(subMenuPanels != null);

var actionMiniMap = ui.getActionInMenu(subMenuPanels, "Mini Map");
checkAction(actionMiniMap, true, true, false);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionMiniMap));

var actionPalette = ui.getActionInMenu(subMenuPanels, "Palette");
checkAction(actionPalette, true, true, true);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionPalette));

var actionLogicalExplorer = ui.getActionInMenu(subMenuPanels, "Logical Model Explorer");
checkAction(actionLogicalExplorer, true, true, false);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionLogicalExplorer));

var actionErrors = ui.getActionInMenu(subMenuPanels, "Errors");
checkAction(actionErrors, true, true, false);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionErrors));

var actionGraphicalExplorer = ui.getActionInMenu(subMenuPanels, "Graphical Model Explorer");
checkAction(actionGraphicalExplorer, true, true, false);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionGraphicalExplorer));

var actionProperty = ui.getActionInMenu(subMenuPanels, "Property Editor");
checkAction(actionProperty, true, true, false);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionProperty));

var actionFileToolbar = ui.getActionInMenu(subMenuPanels, "File Toolbar");
checkAction(actionFileToolbar, true, true, true);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionFileToolbar));

var actionEditToolbar = ui.getActionInMenu(subMenuPanels, "Edit Toolbar");
checkAction(actionEditToolbar, true, true, true);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionEditToolbar));

var actionViewToolbar = ui.getActionInMenu(subMenuPanels, "View Toolbar");
checkAction(actionViewToolbar, true, true, true);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionViewToolbar));

var actionInterpreterToolbar = ui.getActionInMenu(subMenuPanels, "Interpreter Toolbar");
checkAction(actionInterpreterToolbar, true, true, false);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionInterpreterToolbar));

var actionGeneratorsToolbar = ui.getActionInMenu(subMenuPanels, "Generators Toolbar");
checkAction(actionGeneratorsToolbar, true, true, false);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionGeneratorsToolbar));
