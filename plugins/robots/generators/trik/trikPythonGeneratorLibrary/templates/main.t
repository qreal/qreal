#!/usr/bin/env python

import signal
import subprocess
import sys
import time

import trikControl
from PyQt4 import QtGui, QtCore
from script import script


class Program():
  __interpretation_started_timestamp__ = time.time() * 1000

  app = QtGui.QApplication(sys.argv)
  brick = trikControl.trikControl.BrickFactory.create("/home/root/trik/", "/home/root/trik/media/")
  script = script()

  @@VARIABLES@@

  @@SUBPROGRAMS_FORWARDING@@

  @@THREADS_FORWARDING@@

  @@SUBPROGRAMS@@

  @@THREADS@@

  def sigint_handler(self):
    self.app.quit()

  def execMain(self):
    signal.signal(signal.SIGINT, self.sigint_handler)

  @@MAIN_CODE@@

    sys.exit(self.app.exec_())


def main():
  program = Program()
  program.execMain()

if __name__ == '__main__':
  main()
