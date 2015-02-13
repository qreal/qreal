#!/usr/bin/python

usage = ("Usage: build [machine_type] [productName] [version] <path/to/vdi/image> <path/to/shared/folder>\n"
	"Example: build Ubuntu_64 trikStudio 3.0.0-rc2 ~/Downloads/QReal_Ubuntu_64.vdi ~/qreal/share\n\n"
	"Creates or runs the virtual machine of the given in arg1 type (possible types are enumerated in 'machine_types.text' file next to this script). \n"
	"Recommended values: Ubuntu, Ubuntu_64 or Windows81 (VDI image files are already prepared for those OS types by developers).\n"
	"The virtual machine will build the version of the given in arg2 product. Currently only 'trikStudio' option is officially supported.\n"
	"The version (arg3) may be HEAD (then HEAD of the master branch will be built) or contain some version number.\nImportant: "
	"the version is obtained by the tag in the remote repository. The tag must consist of two parts separated by minus sign ('-'). "
	"The first part must be an id of the product (arg1 value) and the second part must be a version (arg3 value). For the above example the tag must be trikStudio-3.0.0-rc2.\n\n"
	"The script will create the virtual machine named QReal_<machine_type> if it does not exist yet. In that case arg4 must contain a path to VDI image prepared for version build.\n"
	"The image will be copied to VM files directory, so take care of enough disk space before build. Project developers have a link to prepared VDI images, also it may be published (see project wiki).\n"
	"arg5 must contain absolute or relative path to a folder that will be used by VM guest OS. In prepared VDI images shared folder name is hard-coded and must be 'share'.\n"
	"If you have a private key from autoupdates server and you plan to deploy builded version as the last there then private and public keys should be placed to the shared folder before build, "
	"otherwise deployment step will be ignored. As the result of the successfull build you will obtain installer files ready for usage placed in the shared folder.\n\n"
	"Feel free to report about all found problems, any help is much appreciated.\n")

import sys, datetime, time, os
from vboxapi import VirtualBoxManager

vbm = VirtualBoxManager(None, None);

# -----------------------------------------------------------------------------------------------

def showHelpAndExit():
	print usage;
	sys.exit();


def waitForGuestProperty(name, value, machine):
	while (machine.getGuestProperty(name)[0] != value):
		time.sleep(1);


def readProcessOutput(process):
	data = process.read(1, 65536, 0);
	if data and len(data) > 0:
		sys.stdout.write(data);
	error = process.read(2, 65536, 0);
	if error and len(error) > 0:
		sys.stderr.write(error);

def runProcess(path, arguments, session):
	guestSession = session.console.guest.createSession("qreal", "password", "", "");
	guestSession.waitFor(vbm.constants.GuestSessionWaitForFlag_Start, 0L);
	process = guestSession.processCreate(path, arguments, None, [vbm.constants.ProcessCreateFlag_WaitForStdOut, vbm.constants.ProcessCreateFlag_WaitForStdErr], 0L);
	print "Started with PID ", process.PID;

	try:
		while (process.status == vbm.constants.ProcessStatus_Starting) or (process.status == vbm.constants.ProcessStatus_Started) or (process.status == vbm.constants.ProcessStatus_Paused):
			readProcessOutput(process);
			time.sleep(0.05);
		readProcessOutput(process);

	except KeyboardInterrupt:
		print "Interrupted by user."


# -----------------------------------------------------------------------------------------------

box = vbm.vbox;
manager = vbm.mgr;
session = vbm.mgr.getSessionObject(box);

if len(sys.argv) < 4 or len(sys.argv) > 6:
	showHelpAndExit();

machineType = sys.argv[1];
machineName = "QReal_" + machineType;
productName = sys.argv[2];
version = "" if sys.argv[3] == "HEAD" else sys.argv[3];

existingMachines = filter(lambda m: m.name == machineName, box.getMachines());
initializationMode = not existingMachines;

machine = None;
if (initializationMode):
	if len(sys.argv) != 6:
		showHelpAndExit();
	vdiImageLocation = sys.argv[4];
	sharedFolderLocation = sys.argv[5];

	print "Creating new machine ", machineName;
	machine = box.createMachine(None, machineName, None, machineType, None);
	box.registerMachine(machine);

	machine.lockMachine(session, vbm.constants.LockType_Write);
	settings = session.machine;

	uniqueSuffix = str(time.time());
	# VirtualBox has known issue keeping memory of old files around. We must append unique suffix to workarraound that.
	targetDiskPath = os.path.dirname(machine.settingsFilePath) + os.path.sep + machineName + uniqueSuffix + ".vdi";

	print "Copying the given VDI image file to ", targetDiskPath, "... This step can take pretty long time.";
	hddorig = box.openMedium(vdiImageLocation, vbm.constants.DeviceType_HardDisk, vbm.constants.AccessMode_ReadOnly, 0);
	hddclone = box.createHardDisk("VDI", targetDiskPath);
	hddorig.cloneTo(hddclone, [vbm.constants.MediumVariant_Standard], None).waitForCompletion(-1);

	print "Attaching hard drive image to VM...";
	settings.addStorageController("IDE", vbm.constants.StorageBus_IDE);
	settings.attachDevice("IDE", 0, 0, vbm.constants.DeviceType_HardDisk, hddclone);

	print "Attaching shared folder...";
	settings.createSharedFolder("share", sharedFolderLocation, True, True);

	settings.memorySize = 1024; # 1Gb

	print "Saving settings...";
	settings.saveSettings();

else:
	print "Found existing VM ", machineName;
	machine = existingMachines[0];

if (session.state == vbm.constants.SessionState_Locked):
	session.unlockMachine();
machine.launchVMProcess(session, "gui", None).waitForCompletion(-1);

print "Waiting for VM to boot...";
waitForGuestProperty("OSBooted", "true", machine);

print "Machine was booted successfully";
time.sleep(5);

print "Running build script...";

bash = "/bin/bash" if machineType.startswith("Ubuntu") else "C:/Users/qreal/AppData/Local/GitHub/PortableGit_c2ba306e536fdf878271f7fe636a147ff37326ad/bin/bash.exe"; 
path = "/home/qreal/scripts/build.sh" if machineType.startswith("Ubuntu") else "c:/users/qreal/build.sh";
runProcess(bash, [path, productName, version], session);

session.console.powerDown();
