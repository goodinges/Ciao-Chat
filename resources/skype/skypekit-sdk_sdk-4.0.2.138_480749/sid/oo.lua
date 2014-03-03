-- OO model :
-- declare a class
-- P1, P2 = class{}, class{ n = 3 }
-- P3 = class():is_a(P2)
-- C = class{}[:is_a(P1, P3, ...)]
-- Optional constructor function C.new(o[, v1, v2, ...])
-- [ex. function C:new(a1, a2) P1.new(self, a1)  P3.new(self, a2) end
-- instanciate a class
-- i = C()   i = C{ m1 = v1, m2 = v2 }   i = C({}, v1, v2) 
-- when new is defined
-- check instance type i:is_a() == C  i:is_a(P1) returns true/false
local getmetatable, setmetatable = getmetatable, setmetatable
local function search(k, superclasses)
  for i=1, #superclasses do 
    local v = superclasses[i][k]
    if v then return v end
  end
  return nil
end
local function no_derive(p) return false end
local function derive(p, plist)
  for i=1, #plist do
    if plist[i] == p or plist[i].__derive(p) then return true end
  end
  return false
end
local function inherit(c, ...)
  local m, superclasses = getmetatable(c), {...}
  m.__index  = function (t, k) return search(k, superclasses) end
  c.__derive = function (p) return derive(p, superclasses) end
  c.is_a = nil
  return c
end
function check_class(o, class)
  local c = getmetatable(o)
  if class == nil then return false end
  return type(c) == "table"  and (class == c or c.__derive(class))
end
function constructor(c, o, ...)
  if type(o) ~= "table" and type(o) ~= "nil" then 
    local p = o
    o = setmetatable({}, c)
    o.is_a = check_class
    if type(c.new) == "function" then o:new(p, ...) end
  else
    o = setmetatable(o or {}, c)
    o.is_a = check_class
    if type(c.new) == "function" then o:new(...) end
  end
  return o
end
function class(c)
  if c == nil then c = {} end
  setmetatable (c, {__call = constructor})
  c.__derive = no_derive
  c.is_a     = inherit
  c.__index  = function (t,k) return c[k] end -- local v = c[k] ; t[k] = v ; return v end
  return c
end
function is(o) -- typechecking usage is(variable).a(class)
  return { a = function(t) return (type(t) == "string" and type(o) == t) or (type(o) == "table"  and o.is_a == check_class and o:is_a(t)) end }
end
