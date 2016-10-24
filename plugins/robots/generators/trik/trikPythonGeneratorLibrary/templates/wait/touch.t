while self.brick.sensor("@@PORT@@").read() <= 0:
  self.script.wait(10);
