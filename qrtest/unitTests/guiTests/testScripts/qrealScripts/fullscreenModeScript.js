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

// For autocompletion and syntax highlighting
var mainWindow, ui, utils, keyboard, palette, hints, scene, cursor;

var menu_View = ui.getMenu("menu_View");
assert(menu_View != null);

utils.activateMenu(menu_View);
api.wait(200);

var actionFullscreen = ui.getActionInMenu(menu_View, "actionFullscreen");
utils.activateMenuAction(menu_View, actionFullscreen);
api.wait(300);

utils.activateMenu(menu_View);
api.wait(200);

//checks the fullscreen states
var actionMenuPanels = ui.getActionInMenu(menu_View, "Panels");
checkAction(actionMenuPanels, true, false, false);
assert(ui.isSubMenuInMenu(menu_View, actionMenuPanels));

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

utils.activateMenuAction(menu_View, actionFullscreen);
api.wait(300);

utils.activateMenu(menu_View);
api.wait(200);

//checks the primordial states
actionMenuPanels = ui.getActionInMenu(menu_View, "Panels");
checkAction(actionMenuPanels, true, false, false);
assert(ui.isSubMenuInMenu(menu_View, actionMenuPanels));

subMenuPanels = ui.getMenuContainedByAction(actionMenuPanels);
assert(subMenuPanels != null);

actionMiniMap = ui.getActionInMenu(subMenuPanels, "Mini Map");
checkAction(actionMiniMap, true, true, true);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionMiniMap));

actionPalette = ui.getActionInMenu(subMenuPanels, "Palette");
checkAction(actionPalette, true, true, true);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionPalette));

actionLogicalExplorer = ui.getActionInMenu(subMenuPanels, "Logical Model Explorer");
checkAction(actionLogicalExplorer, true, true, true);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionLogicalExplorer));

actionErrors = ui.getActionInMenu(subMenuPanels, "Errors");
checkAction(actionErrors, true, true, false);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionErrors));

actionGraphicalExplorer = ui.getActionInMenu(subMenuPanels, "Graphical Model Explorer");
checkAction(actionGraphicalExplorer, true, true, true);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionGraphicalExplorer));

actionProperty = ui.getActionInMenu(subMenuPanels, "Property Editor");
checkAction(actionProperty, true, true, true);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionProperty));

actionFileToolbar = ui.getActionInMenu(subMenuPanels, "File Toolbar");
checkAction(actionFileToolbar, true, true, true);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionFileToolbar));

actionEditToolbar = ui.getActionInMenu(subMenuPanels, "Edit Toolbar");
checkAction(actionEditToolbar, true, true, true);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionEditToolbar));

actionViewToolbar = ui.getActionInMenu(subMenuPanels, "View Toolbar");
checkAction(actionViewToolbar, true, true, true);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionViewToolbar));

actionInterpreterToolbar = ui.getActionInMenu(subMenuPanels, "Interpreter Toolbar");
checkAction(actionViewToolbar, true, true, false);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionInterpreterToolbar));

actionGeneratorsToolbar = ui.getActionInMenu(subMenuPanels, "Generators Toolbar");
checkAction(actionViewToolbar, true, true, false);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionGeneratorsToolbar));

// try to turn on errors dockwidget and turn off palette dockwidget
utils.activateMenuAction(menu_View, actionMenuPanels);
actionErrors = ui.getActionInMenu(subMenuPanels, "Errors");
utils.activateMenuAction(subMenuPanels, actionErrors);
utils.activateMenuAction(menu_View, actionMenuPanels);
actionPalette = ui.getActionInMenu(subMenuPanels, "Palette");
utils.activateMenuAction(subMenuPanels, actionPalette);

// changes the fullscreen mode and checks the "Error" action
utils.activateMenu(menu_View);
utils.activateMenuAction(menu_View, actionFullscreen);
api.wait(300);

utils.activateMenu(menu_View);
api.wait(200);

// changes the second time
utils.activateMenu(menu_View);
utils.activateMenuAction(menu_View, actionFullscreen);
api.wait(300);

utils.activateMenu(menu_View);
api.wait(200);

// checks the error state
// need repeatedly to call getMenuContainedByAction coz there the necessary signal is emitted
subMenuPanels = ui.getMenuContainedByAction(actionMenuPanels);
assert(subMenuPanels != null);
actionErrors = ui.getActionInMenu(subMenuPanels, "Errors");
checkAction(actionErrors, true, true, true);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionErrors));
actionPalette = ui.getActionInMenu(subMenuPanels, "Palette");
checkAction(actionPalette, true, true, false);
expect(!ui.isSubMenuInMenu(subMenuPanels, actionPalette));
