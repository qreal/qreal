while not (self.brick.sensor("@@PORT@@").read() @@SIGN@@ @@DISTANCE@@):
  self.script.wait(10)
