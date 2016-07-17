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

function assertDialog(buttonName, dialogName, delay) {
    var action = ui.widget("QToolButton", buttonName);
    assert(action != null);
    assert(utils.isEnabledAndVisible(action));
    closeExpectedDialog(mainWindow, dialogName, delay);
    leftClick(action);
    api.wait(delay + 50);
}

assertDialog("actionNewProject", "Create project", 500);
assertDialog("actionNew_Diagram", "Create diagram", 500);
assertDialog("actionOpen", "Open existing project", 2000);

function assertDisabledButton(buttonName) {
    var action = ui.widget("QToolButton", buttonName);
    assert(action != null);
    assert(!utils.isEnabledAndVisible(action));
    leftClick(action);
    api.wait(250);
}

assertDisabledButton("actionSave");
assertDisabledButton("actionUndo");
assertDisabledButton("actionRedo");
assertDisabledButton("actionZoom_In");
assertDisabledButton("actionZoom_Out");

var actionFullscreen = ui.widget("QToolButton", "actionFullscreen");
assert(actionFullscreen != null);
leftClick(actionFullscreen);
api.wait(250);

var menu_View = ui.findMenu("menu_View");
assert(menu_View != null);
utils.activateMenu(menu_View);
api.wait(200);

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
expectPanelsAction("Generators Toolbar", true);
