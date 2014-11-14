// Constructor
function Component()
{
	// Executable names must be lower-case product name with hyphens instead of spaces
	installer.executableName = installer.value("ProductName").toLowerCase().replace(/\s/g, "-");
	installer.linkExtension = installer.value("os") === "win" ? ".lnk" : "";
	installer.execExtension = installer.value("os") === "win" ? ".exe" : "";
	installer.maintenanceName = "maintenance" + installer.execExtension;
	installer.shouldDeinstallPrevious = false;

	component.loaded.connect(this, Component.prototype.installerLoaded);
	installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
}

Component.prototype.createOperations = function()
{
	if (installer.shouldDeinstallPrevious) {
		component.addOperation("Execute", "@TargetDir@/" + installer.maintenanceName);
	}
	component.createOperations();
	component.addOperation("CreateShortcut"
			, "@TargetDir@/" + installer.executableName + installer.execExtension
			, "@StartMenuDir@/@ProductName@" + installer.linkExtension);
	component.addOperation("CreateShortcut"
			, "@TargetDir@/" + installer.maintenanceName
			, "@StartMenuDir@/Uninstall @ProductName@" + installer.linkExtension);
	if (installer.value("os") == "win") {
		component.addOperation("Execute"
				, "@TargetDir@/" + installer.executableName + installer.execExtension
				, "--clear-conf");
	} else {
		component.addOperation("Execute"
				, "bash"
				, "-c"
				, "LD_LIBRARY_PATH=@TargetDir@ @TargetDir@/" + installer.executableName + installer.execExtension + " --clear-conf");
	}
}

// Utility function like QString QDir::toNativeSeparators(const QString & pathName) [static]
var Dir = new function () {
	this.toNativeSeparator = function (path) {
		if (installer.value("os") == "win")
			return path.replace(/\//g, '\\');
		return path;
	}

	this.isRelative = function(path) {
		return (installer.value("os") == "win") ? (path.length < 2 || path[1] != ":") : (path[0] != "/");
	}

	this.isRoot = function(path) {
		return (installer.value("os") == "win") ? (path.lenght == 2 && path[1] == ":") : (path == "/");
	}

	this.isHome = function(path) {
		// TODO
		return false;
	}

	this.validatePath = function(path) {
		if (path.length == 0) {
			return qsTr("The installation path cannot be empty, please specify a valid folder.");
		}

		if (Dir.isRelative(path)) {
			return qsTr("The installation path cannot be relative, please specify an absolute path.");
		}

		var nativeTargetDir = Dir.toNativeSeparator(path);
		if (!installer.value("allowNonAsciiCharacters")) {
			for (var i = 0; i < nativeTargetDir.length; ++i) {
				if (nativeTargetDir[i] & 0xff80) {
					return qsTr("The path or installation directory contains non ASCII characters. This is currently not supported! Please choose a different path or installation directory.");
				}
			}
		}

		if (Dir.isRoot(path) || Dir.isHome(path)) {
			return qsTr("As the install directory is completely deleted, installing in ") 
					+ Dir.toNativeSeparator(path) + qsTr(" is forbidden.");
		}

		var ambiguousChars;
		if (installer.value("os") == "win") {
			// folder length (set by user) + maintenance tool name length (no extension) + extra padding
			if ((nativeTargetDir.length + installer.maintenanceName.length + 20) >= 260/*MAX_PATH*/) {
				return qsTr("The path you have entered is too long, please make sure to specify a valid path.");
			}

			if (nativeTargetDir[nativeTargetDir.length - 1] == '.') {
				return qsTr("The installation path must not end with '.', please specify a valid folder.");
			}

			ambiguousChars = "[\"~<>|?*!@#$%^&:,; ]|(\\\\CON)|(\\\\PRN)|(\\\\AUX)|(\\\\NUL)|(\\\\COM\\d)|(\\\\LPT\\d)";
		} else {
			ambiguousChars = "[~<>|?*!@#$%^&:,; \\\\]";
		}


		// check if there are not allowed characters in the target path
		if (nativeTargetDir.match(ambiguousChars)) {
			return qsTr("The installation path must contains spaces or other restricted characters, please specify a valid folder.");
		}

		return "";
	}
};

// Called as soon as the component was loaded
Component.prototype.installerLoaded = function()
{
	if (installer.addWizardPage(component, "TargetWidget", QInstaller.TargetDirectory)) {
		var widget = gui.pageWidgetByObjectName("DynamicTargetWidget");
		if (widget != null) {
			widget.label.text = qsTr("Please specify the folder where ") + installer.value("ProductName") + qsTr(" will be installed.");
			widget.targetDirectory.textChanged.connect(this, Component.prototype.targetChanged);
			widget.targetChooser.clicked.connect(this, Component.prototype.chooseTarget);

			widget.windowTitle = qsTr("Installation Folder");
			widget.targetDirectory.text = Dir.toNativeSeparator(installer.value("TargetDir"));
		}
	}
}

// Callback when one is clicking on the button to select where to install your application
Component.prototype.chooseTarget = function () {
	var widget = gui.pageWidgetByObjectName("DynamicTargetWidget");
	if (widget != null) {
		var newTarget = QFileDialog.getExistingDirectory("Choose your target directory.", widget.targetDirectory.text);
		if (newTarget != "") {
			widget.targetDirectory.text = Dir.toNativeSeparator(newTarget);
		}
	}
}

Component.prototype.targetChanged = function (text) {
	var widget = gui.pageWidgetByObjectName("DynamicTargetWidget");
	var error = Dir.validatePath(text);
	if (error.length) {
		widget.labelOverwrite.text = "<font color='red'>" + error + "</font>";
		widget.complete = false;
		return;
	}

	if (widget != null) {
		if (text != "") {
			widget.complete = true;
			installer.setValue("TargetDir", text);
			if (installer.fileExists(text + "/" + installer.maintenanceName)) {
				var warning = "<font color='green'>" + qsTranslate("installscript", "A previous installation exists in this folder. If you wish to continue, everything will be overwritten.") + "</font>";
				widget.labelOverwrite.text = warning;
				installer.shouldDeinstallPrevious = true;
			} else {
				widget.labelOverwrite.text = "";
				installer.shouldDeinstallPrevious = false;
			}
			return;
		}
		widget.complete = false;
	}
}
