from PyQt4 import QtGui, QtCore
import trikControl
import sys
import time

__interpretation_started_timestamp__ = time.time() * 1000;
app = QtGui.QApplication(sys.argv)
brick = trikControl.trikControl.BrickFactory.create("./trik/", "./trik/media/")
@@VARIABLES@@

@@SUBPROGRAMS_FORWARDING@@

@@THREADS_FORWARDING@@

@@SUBPROGRAMS@@

@@THREADS@@

def main():
@@MAIN_CODE@@

main()
