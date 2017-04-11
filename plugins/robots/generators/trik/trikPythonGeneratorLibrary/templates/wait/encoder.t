while not (self.brick.encoder("@@PORT@@").read() @@SIGN@@ @@TACHO_LIMIT@@):
  self.script.wait(10)
