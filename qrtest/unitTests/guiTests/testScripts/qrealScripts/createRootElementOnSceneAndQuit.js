init();

var openInterpreted = api.ui().widget("QPushButton" ,"Open interpreted diagram");
assert(openInterpreted != null);
assert(api.ui().isEnabledAndVisible(openInterpreted));
clickDialogButtonLater("Select file with metamodel to open", "QPushButton", "Cancel", 1300);
leftClick(openInterpreted);
api.wait(3000);

openInterpreted = api.ui().getStartButton("Open interpreted diagram");
assert(openInterpreted != null);
assert(api.ui().isEnabledAndVisible(openInterpreted));
var createInterpreted = api.ui().getStartButton("Create interpreted diagram");
assert(createInterpreted != null);
assert(api.ui().isEnabledAndVisible(createInterpreted));
fillLineEditInExpectedDialog(mainWindow, "Enter the diagram name:", "", "qwerty", 500);
clickButtonLater("QPushButton", "OK", 1000);
leftClick(createInterpreted);
api.wait(1200);

var paletteDock = api.ui().widget("QDockWidget", "paletteDock");
assert(paletteDock != null);
var paletteTreeWidget = api.ui().widget("qReal::gui::PaletteTreeWidget" , "paletteTreeWidget");
assert(paletteTreeWidget != null);
var paletteViewPort = api.ui().viewPort(paletteTreeWidget);
assert(paletteViewPort != null);
activateContextMenuAction("Add Element", 200);
rightClick(paletteViewPort);
api.wait(200);

clickButtonInExpectedDialog(mainWindow, "ChooseTypeDialog", "QRadioButton", "edgeRadioButton", 200);
clickDialogButtonLater("ChooseTypeDialog", "QRadioButton", "nodeRadioButton", 400);
clickButtonLater("QPushButton", "OK", 600);
api.wait(800);

clickDialogButtonLater("AddNodeDialog", "QCheckBox", "checkBox", 200);
fillLineEditInExpectedDialog(mainWindow, "AddNodeDialog", "nameEdit", "rootNode", 400);
clickDialogButtonLater("AddNodeDialog", "QPushButton", "OK", 600);
api.wait(800);

var actionNew_Diagram = api.ui().widget("QToolButton", "actionNew_Diagram");
assert(actionNew_Diagram != null);
assert(api.ui().isEnabledAndVisible(actionNew_Diagram));
leftClick(actionNew_Diagram);
api.wait(500);

assert(api.scene().noEmpty(api.scene().currentSceneNodeList()));

var rootNode = api.palette().dragPaletteElement("qrm:/qwerty/qwerty/rootNode", 200, 150, 275);
assert (rootNode != "");
api.wait(500);

var sceneViewPort = api.ui().sceneViewport();
assert(sceneViewPort != null);
assert(!api.scene().noEmpty(api.scene().currentSceneNodeList()));

var menuFile = api.ui().getMenu("menu_File");
assert(menuFile != null);
api.ui().activateMenu(menuFile);
api.wait(100);
var actionQuit = api.ui().getActionInMenu(menuFile, "actionQuit");
checkAction(actionQuit, true, false, false);
assert(!api.ui().isSubMenuInMenu(menuFile, actionQuit));

clickDialogButtonLater("Save", "QPushButton", "Save", 1500);
clickDialogButtonLater("Select file to save current metamodel to", "QPushButton", "Cancel", 2250);
api.ui().activateMenuAction(menuFile, actionQuit);
api.wait(200);

api.ui().activateMenu(menuFile);
api.wait(100);
clickDialogButtonLater("Save", "QPushButton", "Cancel", 1500);
api.ui().activateMenuAction(menuFile, actionQuit);
api.wait(200);

quitWithoutSave();
