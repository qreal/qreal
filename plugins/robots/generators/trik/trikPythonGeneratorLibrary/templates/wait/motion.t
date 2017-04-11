while self.brick.sensor("F1").read() == 0:
  self.script.wait(10)
