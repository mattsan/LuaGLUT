function find_if(items, cond)
  for key, item in pairs(items) do
    if cond(item) then
      return key
    end
  end
  return nil
end

-- class Point

Point = {}
Point.__index = Point

function Point:new(x, y)
  assert(x, "Point:new / x is nil")
  assert(y, "Point:new / y is nil")
  assert(type(x) == "number", "Point:new / x is not number")
  assert(type(y) == "number", "Point:new / y is not number")

  local new_instance =
  {
    x = x,
    y = y
  }

  setmetatable(new_instance, self)
  return new_instance
end

function Point:__add(other)
  assert(other, "Pont:__add / other is nil")
  assert(getmetatable(other) == Point, "Point:__add / other is not Point")

  return Point:new(self.x + other.x, self.y + other.y)
end

function Point:__tostring()
  return string.format("(%d, %d)", self.x, self.y)
end

setmetatable(Point, { __call = Point.new })

-- class Boundary

Boundary = {}
Boundary.__index = Boundary

function Boundary:new(lefttop, rightbottom)
  assert(lefttop, "Boundary:new / lettop is nil")
  assert(rightbottom, "Boundary:new / rightbottom is nil")
  assert(getmetatable(lefttop) == Point, "Boundary:new / leftop is not Point")
  assert(getmetatable(rightbottom) == Point, "Boundary:new / rightbottom is not Point")

  local new_instance =
  {
    lefttop_     = lefttop,
    rightbottom_ = rightbottom
  }

  setmetatable(new_instance, self)
  new_instance:rearrenges()
  return new_instance
end

function Boundary:rearrenges()
  if self.rightbottom_.x < self.lefttop_.x then
    self.rightbottom_.x, self.lefttop_.x = self.lefttop_.x, self.rightbottom_.x
  end
  if self.rightbottom_.y < self.lefttop_.y then
    self.rightbottom_.y, self.lefttop_.y = self.lefttop_.y, self.rightbottom_.y
  end
end

function Boundary:__add(other)
  return Boundary:new(self.lefttop + other.lefttop, self.rightbottom + other.rightbottom)
end

function Boundary:lefttop()
  return self.lefttop_
end

function Boundary:rightbottom()
  return self.rightbottom_
end

function Boundary:leftbottom()
  return Point(self.lefttop_.x, self.rightbottom_.y)
end

function Boundary:righttop()
  return Point(self.rightbottom_.x, self.lefttop_.y)
end

function Boundary:left()
  return self.lefttop_.x
end

function Boundary:right()
  return self.rightbottom_.x
end

function Boundary:top()
  return self.lefttop_.y
end

function Boundary:bottom()
  return self.rightbottom_.y
end

function Boundary:includes(point)
  assert(point, "Boundary:includes / point is nil")
  assert(getmetatable(point) == Point, "Boundary:includes / point is not Point")

  return (self.lefttop_.x <= point.x) and (point.x <= self.rightbottom_.x) and
         (self.lefttop_.y <= point.y) and (point.y <= self.rightbottom_.y)
end

function Boundary:overlaps_with(other)
  assert(other, "Boundary:overlaps_with / other is nil")
  assert(getmetatable(other) == Boundary, "Boundary:overlaps_with / other is not Boundary")

  return self:includes(other:lefttop())    or self:includes(other:righttop())    or
         self:includes(other:leftbottom()) or self:includes(other:rightbottom()) or
         other:includes(self:lefttop())    or other:includes(self:righttop())    or
         other:includes(self:leftbottom()) or other:includes(self:rightbottom())
end

function Boundary:__tostring()
  return tostring(self.lefttop_) .. "-" .. tostring(self.rightbottom_)
end

setmetatable(Boundary, { __call = Boundary.new })

-- class RGBColor

RGBColor = {}
RGBColor.__index = RGBColor

function RGBColor:new(r, g, b)
  local new_instance =
  {
    rgb = { r = r or 0, g = g or 0, b = b or 0 }
  }
  setmetatable(new_instance, self)
  return new_instance
end

function RGBColor:__tostring()
  return string.format("(%f, %f, %f)", self.rgb.r, self.rgb.g, self.rgb.b)
end

setmetatable(RGBColor, { __call = RGBColor.new })

-- class Text

Text =
{
  SCALE = 0.1,
  COLOR = RGBColor(1, 0, 0)
}

Text.__index = Text

function Text:new(position)
  assert(position, "Text:new / position is nil")
  assert(getmetatable(position) == Point, "Text:new / poistion is not Point")

  local new_instance =
  {
    position = position,
    text     = ""
  }
  setmetatable(new_instance, self)
  return new_instance
end

function Text:set(text)
  self.text = text
end

function Text:get()
  return text
end

function Text:draw()
  gl.color3dv(self.COLOR.rgb.r, self.COLOR.rgb.g, self.COLOR.rgb.b)
  gl.push_matrix()
  gl.translatef(self.position.x, self.position.y, 0)
  gl.scalef(self.SCALE, -self.SCALE, 0)
  for i = 1, #self.text do
    glut.stroke_character(glut.STROKE_ROMAN, string.byte(self.text, i))
  end
  gl.pop_matrix()
end

-- class ShapeObject

ShapeObject = {}
ShapeObject.__index = ShapeObject

function ShapeObject:new(position)
  local new_instance = {}
  setmetatable(new_instance, self)
  return new_instance
end

function ShapeObject:is_visible()
  return Game:screen():includes(self.position)
end

function ShapeObject:draw_with_color(color)
  gl.color3dv(color.rgb.r, color.rgb.g, color.rgb.b)
  gl.primitive(gl.POLYGON, function()
    for _, point in pairs(self.points) do
      gl.vertex2i(self.position.x + point.x, self.position.y + point.y)
    end
  end)
end

function ShapeObject:move_(delta)
  assert(delta, "ShapeObject:move_ / delta is nil")
  assert(getmetatable(delta) == Point, "ShapeObject:move_ / delta is not Point")

  self.position = self.position + delta
end

function ShapeObject:move(delta)
  assert(delta, "ShapeObject:move / delta is nil")
  assert(getmetatable(delta) == Point, "ShapeObject:move / delta is not Point")

  self:move_(delta)
end

-- class Bullet (inherited ShapeObject)

Bullet = {}
Bullet.__index = Bullet
setmetatable(Bullet, { __index = ShapeObject })

function Bullet:new(point, delta, color)
  local new_instance =
  {
    position = point or Point(0, 0),
    delta_y  = delta or 0,
    color    = color or RGBColor:new(),
    visible  = point and true or false,
    points   =
    {
      Point(-3,  -5),
      Point( 0, -10),
      Point( 3,  -5),
      Point( 0,   0)
    } 
  }
  setmetatable(new_instance, self)
  return new_instance
end

function Bullet:boundary()
  return Boundary(self.position + Point(-3, -10), self.position + Point(3, 0))
end

function Bullet:set_visible(visible)
  self.visible = visible
end

function Bullet:is_visible()
  return self.visible
end

function Bullet:move()
  if self.visible then
    self:move_(Point(0, self.delta_y))
    self.visible = Game:screen():includes(self.position)
  end
end

function Bullet:draw()
  if self.visible then
    self:draw_with_color(self.color)
  end
end

-- class Alien (inherited ShapeObject)

Alien = 
{
  BULLET_SPEED = 10,
  COLOR  = RGBColor:new(1, 0.5, 0),
  points =
  {
    Point( -5, -5),
    Point(  5, -5),
    Point( 10,  0),
    Point(  5,  5),
    Point(  2,  2),
    Point( -2,  2),
    Point( -5,  5),
    Point(-10,  0)
  }
}

Alien.__index = Alien

setmetatable(Alien, { __index = ShapeObject })

function Alien:new()
  local new_instance =
  {
    speed_x = 5,
    speed_y = 10,
    position = Point(20, 20),
  }

  setmetatable(new_instance, self)
  return new_instance
end

function Alien:boundary()
    return Boundary(self.position + Point(-10, -5), self.position + Point(10, 5))
end

function Alien:fire()
  return Bullet:new(self.position, self.BULLET_SPEED, self.COLOR)
end

function Alien:turn()
  self.speed_x = -self.speed_x
end

function Alien:move()
  self:move_(Point(self.speed_x, 0))
  screen = Game:screen()

  if (self.position.x < screen:left()) or (screen:right() < self.position.x) then
    self:turn()
    self:move_(Point(0, self.speed_y))
  end
end

function Alien:draw()
  self:draw_with_color(self.COLOR)
end

-- class Aliens

Aliens = {}
Aliens.__index = Aliens

function Aliens:new()
  local new_instance =
  {
    aliens  = {},
    bullets = {}
  }

  setmetatable(new_instance, self)
  return new_instance
end

function Aliens:add_alien()
  table.insert(self.aliens, Alien:new())
end

function Aliens:is_hit_by(object)
  if not object:is_visible() then
    return false
  end

  local key = find_if(self.aliens, function(item) return object:boundary():overlaps_with(item:boundary()) end)
  if not key then
    return false
  end

  self.aliens[key] = nil
  return true
end

function Aliens:hits(object)
  return find_if(self.bullets, function(item) return object:boundary():overlaps_with(item:boundary()) end)
end

function Aliens:clear_bullets()
  self.bullets = {}
end

function Aliens:draw()
  for _, alien in pairs(self.aliens) do alien:draw() end
  for _, bullet in pairs(self.bullets) do bullet:draw() end
end

function Aliens:move()
  for _, alien in pairs(self.aliens) do alien:move() end
  for _, bullet in pairs(self.bullets) do bullet:move() end

  for key, bullet in pairs(self.bullets) do
    if not bullet:is_visible() then
      self.bullets[key] = nil
    end
  end

  for _, alien in pairs(self.aliens) do
    if math.random(20) == 1 then
      table.insert(self.bullets, alien:fire())
    end
  end
end

-- class Ship (inherited ShapeObject)

Ship =
{
  DELTA = 10,
  COLOR = RGBColor(0, 0, 1),
  points =
  {
    Point(-10,   0),
    Point(  0, -10),
    Point( 10,   0),
    Point( 10,  10),
    Point(-10,  10)
  }
}

Ship.__index = Ship

setmetatable(Ship, { __index = ShapeObject })

function Ship:new(position)
  local new_instance =
  {
    position = position
  }
  setmetatable(new_instance, self)
  return new_instance
end

function Ship:boundary()
  return Boundary(self.position + Point(-10, 0), self.position + Point(10, 10))
end

function Ship:move_left()
  self:move(Point(-self.DELTA, 0))
end

function Ship:move_right()
  self:move(Point(self.DELTA, 0))
end

function Ship:move_up()
  self:move(Point(0, -self.DELTA))
end

function Ship:move_down()
  self:move(Point(0, self.DELTA))
end

function Ship:fire()
  return Bullet:new(self.position, -10, self.COLOR)
end

function Ship:draw()
  self:draw_with_color(self.COLOR)
end

-- class Game (singleton)

Game =
{
  INTERVAL = 100,

  SPACE  = 32,
  CTRL_C =  3,

  POINT_PER_ALIEN = 10
}

Game.width         = 320
Game.height        = 240
Game.my_ship       = Ship:new(Point(Game.width / 2, Game.height / 2))
Game.my_bullet     = Bullet:new()
Game.aliens        = Aliens:new()
Game.text          = Text:new(Point(10, 12))
Game.score         = 0
Game.left          = 3
Game.interval      = Game.INTERVAL -- timer interval
Game.timer_enabled = true          -- timer enable flag

function Game:draw()
  gl.clear(gl.COLOR_BUFFER_BIT)
  self.my_ship:draw()
  self.my_bullet:draw()
  self.aliens:draw()
  self.text:draw()
  glut.swap_buffers()
end

function Game:display() -- callback function of display
  self:draw()
end

function Game:reshape(w, h) -- callback function of reshape
  gl.viewport(0, 0, w, h)
  self.width  = w
  self.height = h
  gl.load_identity()
  gl.ortho(-0.5, w - 0.5, h - 0.5, -0.5, -1.0, 1.0)
end

function Game:keyboard(key, x, y) -- callback functionn of keyboard event
  if (key == self.SPACE) and not self.my_bullet:is_visible() then
    self.my_bullet = self.my_ship:fire()
  elseif key == self.CTRL_C then
    os.exit()
  end
end

function Game:special(key, x, y) -- callback function of spectial key event
  if key == glut.KEY_LEFT then
    self.my_ship:move_left()
  elseif key == glut.KEY_UP then
    self.my_ship:move_up()
  elseif key == glut.KEY_RIGHT then
    self.my_ship:move_right()
  elseif key == glut.KEY_DOWN then
    self.my_ship:move_down()
  end
end

function Game:timer(n) -- callback function of timer
  self.my_bullet:move()
  self.aliens:move()

  if math.random(20) == 1 then
    self.aliens:add_alien()
  end

  if self.aliens:is_hit_by(self.my_bullet) then
    self.score = self.score + self.POINT_PER_ALIEN
    self.my_bullet:set_visible(false)
  end

  local my_ship_hit = self.aliens:hits(self.my_ship)

  self.text:set(string.format("SCORE : %06d / LEFT : %d", self.score, self.left))

  local clear_color = my_ship_hit and RGBColor(1.0, 0.8, 0.8) or RGBColor(1.0, 1.0, 1.0)
  gl.clear_color(clear_color.rgb.r, clear_color.rgb.g, clear_color.rgb.b, 1.0)

  self:draw()

  self.interval = self.INTERVAL

  if my_ship_hit then
    if self.left == 0 then
      self.timer_enabled = false
      return
    end
    self.interval = self.INTERVAL * 10
    self.left = self.left - 1
    self.aliens:clear_bullets()
  end
end

function Game:screen()
  return Boundary(Point(0, 0), Point(self.width, self.height))
end

-- entry point

glut.init_window_position(100, 100)
glut.init_window_size(Game.width, Game.height)
glut.init(arg)
glut.init_display_mode(glut.RGBA + glut.DOUBLE)
glut.create_window("sample2")
gl.clear_color(1.0, 1.0, 1.0, 1.0)

glut.main_loop(Game)