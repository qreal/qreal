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

var actionNewProject = ui.widget("QToolButton", "actionNewProject");
assert(actionNewProject != null);
assert(utils.isEnabledAndVisible(actionNewProject));
closeExpectedDialog(mainWindow, "Create project", 500);
leftClick(actionNewProject);
api.wait(550);

var actionNew_Diagram = ui.widget("QToolButton", "actionNew_Diagram");
assert(actionNew_Diagram != null);
assert(utils.isEnabledAndVisible(actionNew_Diagram));
closeExpectedDialog(mainWindow, "Create diagram", 500);
leftClick(actionNew_Diagram);
api.wait(550);

var actionOpen = ui.widget("QToolButton", "actionOpen");
assert(actionOpen != null);
assert(utils.isEnabledAndVisible(actionOpen));
closeExpectedDialog(mainWindow, "Open existing project", 2000);
leftClick(actionOpen);
api.wait(2500);

var actionSave = ui.widget("QToolButton", "actionSave");
assert(actionSave != null);
assert(!utils.isEnabledAndVisible(actionSave));
leftClick(actionSave);
api.wait(150);

var actionUndo = ui.widget("QToolButton", "actionUndo");
assert(actionUndo != null);
assert(!utils.isEnabledAndVisible(actionUndo));
leftClick(actionUndo);
api.wait(250);

var actionRedo = ui.widget("QToolButton", "actionRedo");
assert(actionRedo != null);
assert(!utils.isEnabledAndVisible(actionRedo));
leftClick(actionRedo);
api.wait(250);

var actionZoom_In = ui.widget("QToolButton", "actionZoom_In");
assert(actionZoom_In != null);
assert(!utils.isEnabledAndVisible(actionZoom_In));
leftClick(actionZoom_In);
api.wait(250);

var actionZoom_Out = ui.widget("QToolButton", "actionZoom_Out");
assert(actionZoom_Out != null);
assert(!utils.isEnabledAndVisible(actionZoom_Out));
leftClick(actionZoom_Out);
api.wait(250);

var actionFullscreen = ui.widget("QToolButton", "actionFullscreen");
assert(actionFullscreen != null);
leftClick(actionFullscreen);
api.wait(250);


var menu_View = ui.getMenu("menu_View");
assert(menu_View != null);
utils.activateMenu(menu_View);
api.wait(200);

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
