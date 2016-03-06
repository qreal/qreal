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

var menu_View = ui.findMenu("menu_View");
assert(menu_View != null);

utils.activateMenu(menu_View);
api.wait(200);

var actionFullscreen = ui.findActionInMenu(menu_View, "actionFullscreen");
utils.activateMenuAction(menu_View, actionFullscreen);
api.wait(300);

utils.activateMenu(menu_View);
api.wait(200);

//checks the fullscreen states
var actionMenuPanels = ui.findActionInMenu(menu_View, "Panels");
checkAction(actionMenuPanels, true, false, false);
assert(ui.isSubMenuInMenu(menu_View, actionMenuPanels));

var subMenuPanels = ui.findMenuContainedByAction(actionMenuPanels);
assert(subMenuPanels != null);

function expectPanelsAction(actionName, isChecked) {
    var action = ui.findActionInMenu(subMenuPanels, actionName);
    checkAction(action, true, true, isChecked);
    expect(!ui.isSubMenuInMenu(subMenuPanels, action));
}

expectPanelsAction("Mini Map", false);
expectPanelsAction("Palette", true);
expectPanelsAction("Logical Model Explorer", false);
expectPanelsAction("Errors", false);
expectPanelsAction("Graphical Model Explorer", false);
expectPanelsAction("Property Editor", false);
expectPanelsAction("File Toolbar", true);
expectPanelsAction("Edit Toolbar", true);
expectPanelsAction("View Toolbar", true);
expectPanelsAction("Interpreter Toolbar", false);
expectPanelsAction("Generators Toolbar", false);

utils.activateMenuAction(menu_View, actionFullscreen);
api.wait(300);

utils.activateMenu(menu_View);
api.wait(200);

//checks the primordial states
actionMenuPanels = ui.findActionInMenu(menu_View, "Panels");
checkAction(actionMenuPanels, true, false, false);
assert(ui.isSubMenuInMenu(menu_View, actionMenuPanels));

subMenuPanels = ui.findMenuContainedByAction(actionMenuPanels);
assert(subMenuPanels != null);

expectPanelsAction("Mini Map", true);
expectPanelsAction("Palette", true);
expectPanelsAction("Logical Model Explorer", true);
expectPanelsAction("Errors", false);
expectPanelsAction("Graphical Model Explorer", true);
expectPanelsAction("Property Editor", true);
expectPanelsAction("File Toolbar", true);
expectPanelsAction("Edit Toolbar", true);
expectPanelsAction("View Toolbar", true);
expectPanelsAction("Interpreter Toolbar", false);
expectPanelsAction("Generators Toolbar", false);

// try to turn on errors dockwidget and turn off palette dockwidget
utils.activateMenuAction(menu_View, actionMenuPanels);
assert(subMenuPanels != null);
var actionErrors = ui.findActionInMenu(subMenuPanels, "Errors");
assert(actionErrors != null);
utils.activateMenuAction(subMenuPanels, actionErrors);
utils.activateMenu(menu_View);
api.wait(200);
utils.activateMenuAction(menu_View, actionMenuPanels);
var actionPalette = ui.findActionInMenu(subMenuPanels, "Palette");
utils.activateMenuAction(subMenuPanels, actionPalette);

// changes the fullscreen mode and checks the "Error" action
utils.activateMenu(menu_View);
utils.activateMenuAction(menu_View, actionFullscreen);
api.wait(300);

utils.activateMenu(menu_View);
api.wait(200);

// changes the second time
utils.activateMenuAction(menu_View, actionFullscreen);
api.wait(300);

utils.activateMenu(menu_View);
api.wait(200);

// checks the error state
// need repeatedly call getMenuContainedByAction because there the necessary signal is emitted
subMenuPanels = ui.findMenuContainedByAction(actionMenuPanels);
assert(subMenuPanels != null);

expectPanelsAction("Errors", true);
expectPanelsAction("Palette", false);
