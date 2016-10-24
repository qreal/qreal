while self.brick.sensor("@@PORT@@").read() <= 0:
  script.wait(10);
