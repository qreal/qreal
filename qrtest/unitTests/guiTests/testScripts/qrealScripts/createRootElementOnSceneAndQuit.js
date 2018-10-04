/* Copyright 2015-2016 QReal Research Group, CyberTech Labs Ltd.
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

var openInterpreted = ui.widget("QPushButton", "Open interpreted diagram");
assert(openInterpreted != null);
assert(utils.isEnabledAndVisible(openInterpreted));
clickButtonLater("Select file with metamodel to open", "Cancel", 1300);
// or invokeLater(utils, "clickButton", "Select file with metamodel to open", "Cancel", 1300);
leftClick(openInterpreted);
api.wait(3000);

openInterpreted = ui.findStartButton("Open interpreted diagram");
expect(openInterpreted != null);
expect(utils.isEnabledAndVisible(openInterpreted));
var createInterpreted = ui.findStartButton("Create interpreted diagram");
assert(createInterpreted != null);
assert(utils.isEnabledAndVisible(createInterpreted));

fillInputWidgetLater("Enter the diagram name:", "", "qwerty", 500)
// or invokeLater(utils, "fillInputWidget", "Enter the diagram name:", "", "qwerty", 500);
clickButtonLater("Enter the diagram name:", "OK", 1000);
leftClick(createInterpreted);
api.wait(1200);

var paletteDock = ui.widget("QDockWidget", "paletteDock");
assert(paletteDock != null);
var paletteTreeWidget = ui.widget("qReal::gui::PaletteTreeWidget" , "paletteTreeWidget");
assert(paletteTreeWidget != null);
var paletteViewPort = ui.viewPort(paletteTreeWidget);
assert(paletteViewPort != null);
activateContextMenuActionLater("Add Entity", 200);
// or invokeLater(utils, "activateContextMenuAction", "Add Entity", 200);
rightClick(paletteViewPort);
api.wait(200);

clickButtonLater("AddNodeDialog", "checkBox", 200);
fillInputWidgetLater("AddNodeDialog", "nameEdit", "rootNode", 400);
clickButtonLater("AddNodeDialog", "OK", 1000);
api.wait(1000);

var actionNew_Diagram = ui.widget("QToolButton", "actionNew_Diagram");
assert(actionNew_Diagram != null);
assert(utils.isEnabledAndVisible(actionNew_Diagram));
leftClick(actionNew_Diagram);
api.wait(500);

expect(utils.length(scene.currentSceneNodeList()) == 0);
var rootNode = palette.dragPaletteElement("qrm:/qwerty/qwerty/rootNode", 200, 150, 275);
api.wait(100);
assert(rootNode != "");
expect(utils.length(scene.currentSceneNodeList()) == 1);
var sceneViewPort = ui.sceneViewport();
assert(sceneViewPort != null);

var menuFile = ui.findMenu("menu_File");
assert(menuFile != null);
utils.activateMenu(menuFile);
api.wait(100);
var actionQuit = ui.findActionInMenu(menuFile, "actionQuit");
checkAction(actionQuit, true, false, false);
assert(!ui.isSubMenuInMenu(menuFile, actionQuit));

clickButtonLater("Save", "Save", 1500);
clickButtonLater("Select file to save current metamodel to", "Cancel", 2250);
utils.activateMenuAction(menuFile, actionQuit);
api.wait(200);

utils.activateMenu(menuFile);
api.wait(100);
clickButtonLater("Save", "Cancel", 1500);
utils.activateMenuAction(menuFile, actionQuit);
api.wait(200);

quitWithoutSave();
