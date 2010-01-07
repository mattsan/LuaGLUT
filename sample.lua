sample =
{
  ctrl_c = 3,

  interval = 100,
  width    = 160,
  height   = 120,

  pos1 = { x = 10, y = 10 },
  vel1 = { x =  8, y = -2 },
  pos2 = { x = 75, y = 55 },
  vel2 = { x = -6, y =  4 }
}


function sample:keyboard(key, x, y)
  if key == self.ctrl_c then os.exit() end
end

function sample:display()
  glut.vertex2i(self.pos1.x, self.pos1.y)
  glut.vertex2i(self.pos2.x, self.pos2.y)
end

function sample:timer()
  self.pos1.x = self.pos1.x + self.vel1.x
  self.pos1.y = self.pos1.y + self.vel1.y
  self.pos2.x = self.pos2.x + self.vel2.x
  self.pos2.y = self.pos2.y + self.vel2.y
  if (self.pos1.x < 0) or (self.width  < self.pos1.x) then self.vel1.x = -self.vel1.x end
  if (self.pos1.y < 0) or (self.height < self.pos1.y) then self.vel1.y = -self.vel1.y end
  if (self.pos2.x < 0) or (self.width  < self.pos2.x) then self.vel2.x = -self.vel2.x end
  if (self.pos2.y < 0) or (self.height < self.pos2.y) then self.vel2.y = -self.vel2.y end
end

glut.main_loop(sample)
