#import subprocess
import sys
import time

class Program():
  __interpretation_started_timestamp__ = time.time() * 1000

  @@VARIABLES@@

  @@SUBPROGRAMS_FORWARDING@@

  @@THREADS_FORWARDING@@

  @@SUBPROGRAMS@@

  @@THREADS@@

  def execMain(self):

  @@MAIN_CODE@@

def main():
  program = Program()
  program.execMain()

if __name__ == '__main__':
  main()
