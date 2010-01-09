sample =
{
  pos1 = { x=  10, y =  20 },
  vel1 = { x=   8, y =  -2 },
  pos2 = { x= 310, y = 230 },
  vel2 = { x=  -6, y =   4 },

  interval = 200,

  width  = 320,
  height = 240,

  ctrl_c = 3
}

function sample:display()
  gl.clear(gl.COLOR_BUFFER_BIT)

  gl.color3d(0.0, 0.0, 0.0)
  gl.primitive(gl.LINES, function()
    gl.vertex2i(self.pos1.x, self.pos1.y)
    gl.vertex2i(self.pos2.x, self.pos2.y)
  end)

  gl.flush()
end

function sample:reshape(w, h)
  gl.viewport(0, 0, w, h)
  gl.load_identity()
  gl.ortho(-0.5, w - 0.5, h - 0.5, -0.5, -1.0, 1.0)
  self.width  = w
  self.height = h
end

function sample:keyboard(key, x, y)
  if key == self.ctrl_c then
    os.exit()
  end
end

function sample:timer(value)
  self.pos1.x = self.pos1.x + self.vel1.x
  self.pos1.y = self.pos1.y + self.vel1.y
  self.pos2.x = self.pos2.x + self.vel2.x
  self.pos2.y = self.pos2.y + self.vel2.y
  if (self.pos1.x < 0) or (self.width  < self.pos1.x) then self.vel1.x = -self.vel1.x end
  if (self.pos1.y < 0) or (self.height < self.pos1.y) then self.vel1.y = -self.vel1.y end
  if (self.pos2.x < 0) or (self.width  < self.pos2.x) then self.vel2.x = -self.vel2.x end
  if (self.pos2.y < 0) or (self.height < self.pos2.y) then self.vel2.y = -self.vel2.y end

  gl.color3d(0.0, 0.0, 0.0)
  gl.primitive(gl.LINES, function()
    gl.vertex2i(self.pos1.x, self.pos1.y)
    gl.vertex2i(self.pos2.x, self.pos2.y)
  end)

  gl.flush()
end

glut.init_window_position(100, 100)
glut.init_window_size(sample.width, sample.height)
glut.init(arg)
glut.init_display_mode(glut.RGBA)
glut.create_window(arg[0])
gl.clear_color(1.0, 1.0, 1.0, 1.0)

glut.main_loop(sample)
