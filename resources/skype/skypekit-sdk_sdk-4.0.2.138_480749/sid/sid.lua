require 'oo'

AST = { errcount = 0, accept_aliases = {}, allnamed = {}, verbose = false  }

function AST.where(upstack)
  local loc = debug.getinfo((upstack or 0)+1, "Sl")
  return string.format('"%s":%d', loc.short_src, loc.currentline)
end

function AST.error(loc, ...)
  io.stderr:write('ERR (', loc , ') ', table.concat{...}, '\n')
  AST.errcount = AST.errcount + 1
end

function AST.parse_definition(kind, loc, definition, name, key, decorations)
  if type(definition) ~= "table" and not (AST.accept_aliases[AST[kind]] and type(definition) == "string") then
    AST.error(loc, 'missing definition for ', string.upper(kind))
    definition = {}
  end
  return AST[kind](definition, name, loc, key, decorations)
end

function AST.parse_named(kind, loc, name, key, decorations)
  if type(name) ~= "string" then
    AST.error(loc , 'missing name for ', string.upper(kind))
    if type(name) ~= "table" then os.exit(1) end
    return AST.parse_definition(kind, name, "missing", loc, key, decorations)
  end
  return function(definition) return AST.parse_definition(kind, loc, definition, name, key, decorations) end
end

function AST.parse_named_or_decorated(kind, loc, name, ...)
  local decorations = { ... }
  if #decorations == 0 then
    -- we may either have a single decoration in the name or the name
    -- a look ahead will tell it us (if a table, then we had name, or if a string then it was a decoration)
    if type(name) ~= "string" then
      AST.error(loc , 'missing name for ', string.upper(kind))
      os.exit(1) -- better not to try further
    end
    return function(name_or_def) -- look ahead function
      if type(name_or_def) == "string" then -- we had a decoration
        return function(definition) return AST.parse_definition(kind, loc, definition, name_or_def, nil, { [name] = true } ) end
      elseif type(name_or_def) == "table" then -- we had a name
        return AST.parse_definition(kind, loc, name_or_def, name)
      end
      AST.error(loc , 'missing name for ', string.upper(kind))
      os.exit(1) -- better not to try further
    end
  end
  -- we for sure have a decoration so we are expecting the name of the node
  table.insert(decorations, name, 1)
  for _,deco in ipairs{ ... } do
    if type(deco) ~= "string" then
      AST.error(loc , 'bad decoration for ', string.upper(kind))
    else
      decorations[deco] = true
    end
  end
  return function(name)
          if type(name) ~= "string" then
            AST.error(loc, 'missing name for ', string.upper(kind))
            name = "missing"
          end
          return AST.parse_named(kind, loc, name, nil, decorations)
        end
end

function AST.parse_tagged(kind, loc, key, ...)
  if type(key) ~= "number" and kind ~= "Module" then
    AST.error(loc , 'missing key for ', string.upper(kind))
  end
  local decorations = { }
  for _,deco in ipairs{ ... } do
    if type(deco) ~= "string" then
      AST.error(loc , 'bad decoration for ', string.upper(kind))
    else
      decorations[deco] = true
    end
  end
  return function(name)
          if type(name) ~= "string" then
            AST.error(loc, 'missing name for ', string.upper(kind))
            name = "missing"
          end
          return AST.parse_named(kind, loc, name, key, decorations)
        end
end

AST.Node       = class{ ast_loc = "unknown", ast_name = "unnamed", ast_parent = nil }
AST.Field      = class{}:is_a(AST.Node)
AST.Property   = class{}:is_a(AST.Node)
AST.PKProperty = class{}:is_a(AST.Node)
AST.Setupkey   = class{}:is_a(AST.Node)
AST.Scope      = class{}:is_a(AST.Node)
AST.Type       = class{}:is_a(AST.Node)
AST.Typedef    = class{}:is_a(AST.Type)
AST.accept_aliases[AST.Typedef]=true
getmetatable(AST.Typedef).__call = function(c, o, name, loc, foo, decorators) return constructor(AST.Typedef, { def = o }, name, loc, foo, decorators) end
AST.Enumerated = class{}:is_a(AST.Type, AST.Scope)
AST.Enum       = class{}:is_a(AST.Node)
AST.Bitmap     = class{}:is_a(AST.Enumerated)
AST.Module     = class{}:is_a(AST.Scope)
AST.Message    = class{}:is_a(AST.Scope)
AST.Namespace  = class{}:is_a(AST.Scope)
AST.Sid        = class{}:is_a(AST.Scope)
AST.Event      = class{}:is_a(AST.Message)
AST.Action     = class{}:is_a(AST.Node)
getmetatable(AST.Action).__call = function(c, o, name, loc, tag, decorations)
                                    action = constructor(AST.Action, { request = AST.Message(o, 'request', loc), skylibhpp = o.skylibhpp }, name, loc, tag, decorations)
                                    action[1] = action.request
                                    action.request.ast_parent = action
                                    return action
                                  end
function AST.Action:__sub(response)
  local loc = AST.where(2)
  if not self.response then
    self.response = AST.Message(response, 'response', loc)
    self[2] = self.response
    self.response.ast_parent = self
  else
    AST.error(loc , 'multiple responses for action ', self.ast_name)
  end
  return self
end

AST.List = class{}:is_a(AST.Type)

function AST.Node:new(name, loc, key, decorations)
  -- use both to avoid wondering which one to use in generators
  self.ast_key, self.ast_tag = key, key
  if AST.verbose then
    print("instantiating ", name, "in", loc, "with ", self.ast_key or "")
    -- removed table.tostring() utility per TPS nonesense, #SKYPEKIT-577
    -- decorations and "(".. table.tostring(decorations) ..")" or "")
  end
  local allnamed = AST.allnamed[name]
  if not allnamed then
    AST.allnamed[name] = { self }
  else
    allnamed[#allnamed+1] = self
  end
  self.ast_loc  = loc
  self.ast_name = name
  self.ast_decoration = decorations
  return self
end

function AST.Enumerated:new(name, loc, foo, decorations)
  AST.Scope.new(self, name, loc, foo, decorations)
  local check_dup_values = {}
  for i=1,#self do
    local v=self[i]
    if check_dup_values[v.ast_key] then
        AST.error(v.ast_loc , 'duplicate ',v.ast_key, ' used by ',v.ast_name,' and ',check_dup_values[v.ast_key].ast_name, ' in ', self.ast_name)
    else
      check_dup_values[v.ast_key] = v
    end
  end
  return self
end

function AST.Bitmap:new(name, loc, foo, decorations)
  AST.Enumerated.new(self, name, loc, foo, decorations)
  for i=1,#self do
    local field = self[i]
    if is(field).a(AST.Enum) then
      v =  field.ast_key
      while math.floor(v) == v and v ~= 1 do
        v = v / 2
      end
      if v ~= 1 then
        ok = false
        AST.error(field.ast_loc, field.ast_name,  " isn't a power of 2")
      end
    end
  end
  return self
end

function AST.Property:new(name, loc, key, decorations)
  local node = AST.Node.new(self, name, loc, key, decorations)
  local props = AST.builtin_type.PROPKEY
  props[#props+1] = node
  return node
end

function AST.Scope:new(name, loc, key, decorations)
  for _,v in ipairs(self) do
    if type(v) == "table" and type(v.ast_name) == "string" then
      if self[v.ast_name] then
        AST.error(v.ast_loc , 'duplicate ', v.ast_name, ' in ', name)
      end
      self[v.ast_name] = v
      v.ast_parent = self
    end
  end
  return AST.Node.new(self, name, loc, key, decorations)
end

function AST.List:new()
  local loc = AST.where(3)
  if type(self.of) ~= "string" then
    AST.error(loc , 'invalid element of list')
    self.of = "unknown_type"
  end
  return AST.Node.new(self, "", AST.where(3))
end

function FIELD(tag, ...)
  return AST.parse_tagged("Field", AST.where(2), tag, ...)
end
function PROPERTY(key, ...)
  return AST.parse_tagged("Property", AST.where(2), key, ...)
end
function PK_PROPERTY(key, ...)
  return AST.parse_tagged("PKProperty", AST.where(2), key, ...)
end
function SETUPKEY(name_or_decorator, ...)
  return AST.parse_named_or_decorated("Setupkey", AST.where(2), name_or_decorator, ...)
end
function ENUMERATED(name_or_decorator, ...)
  return AST.parse_named_or_decorated("Enumerated", AST.where(2), name_or_decorator, ...)
end
function ENUM(key, ...)
  return AST.parse_tagged("Enum", AST.where(2), key, ...)
end
function BITMAP(name_or_decorator, ...)
  return AST.parse_named_or_decorated("Bitmap", AST.where(2), name_or_decorator, ...)
end
function NAMESPACE(name_or_decorator, ...)
  return AST.parse_named_or_decorated("Namespace", AST.where(2), name_or_decorator, ...)
end
function MODULE(tag, ...)
  return AST.parse_tagged("Module", AST.where(2), tag, ...)
end
function EVENT(tag, ...)
  return AST.parse_tagged("Event", AST.where(2), tag, ...)
end
function ACTION(tag, ...)
  return AST.parse_tagged("Action", AST.where(2), tag, ...)
end
function TYPEDEF(name_or_decorator, ...)
  return AST.parse_named_or_decorated("Typedef", AST.where(2), name_or_decorator, ...)
end
function LIST(element)
  return AST.List{ of = element }
end
function SID(name)
  return AST.parse_named("Sid", AST.where(2), name)
end

function AST.ast_lookup(scope, name, type_filter)
  if type(name)== "string" then
    name = name:gsub('::','.'):gsub(':','.') -- some helper to use more flexible syntax...
    local ok, v = pcall(loadstring('return '..scope..'.'..name))
    if ok and v then
      if type_filter then
        if not is(v).a(type_filter) then return nil end -- or shall try upper scope?
      end
      return v, scope
    end
    -- try in upper scope...
    local reduced = scope:reverse():match('%.(.+)')
    if reduced then return AST.ast_lookup(reduced:reverse(), name) end
  end
  return nil
end

function AST.ast_scope(node)
  assert(is(node).a(AST.Node))
  while node.ast_parent do
    local ancester_name = AST.ast_scope(node.ast_parent)
    if ancester_name == "" then
      return node.ast_parent.ast_name
    end
    return ancester_name .. '.' .. node.ast_parent.ast_name
  end
  return ""
end

AST.builtin_type = {
  int        = true,
  uint       = true,
  boolean    = true,
  bool       = "boolean",
  string     = true,
  xml        = true,
  PROPKEY    = { ast_name = "PROPKEY" },
  OBJECTTYPE = { ast_name = "OBJECTTYPE" },
  metatag    = true,
  metadata   = true,
  binary     = true,
  opaque     = true, -- void* ...
  filename   = true,
  uint64     = true,
  timestamp  = "uint",
  Term       = true, -- ?
}

function AST.ast_actual_type(scope, typeref, inlist)
  inlist = inlist or false
  local reason = "dangling type reference"
  if AST.builtin_type[typeref] then
    if type(AST.builtin_type[typeref]) == "string" then
      return AST.builtin_type[typeref], typeref 
    else
      return typeref, typeref
    end
  end
  if is(typeref).a(AST.List) then
    reason = "list of list"
    if not inlist then
      local actual = AST.ast_actual_type(scope, typeref.of, typeref)
      if actual ~= "illegal" then return "list_"..actual, typeref end
      reason = "list of illegal type"
    end
  elseif is(typeref).a(AST.Bitmap) then     -- first Bitmap check, then Enumerated
    return "bitmap", typeref
  elseif is(typeref).a(AST.Enumerated) then
    return "enum", typeref
  elseif is(typeref).a(AST.Module) then
    return "module", typeref
  elseif is(typeref).a(AST.Namespace) then
    return "namespace", typeref
  elseif type(typeref) == "string" then
    local ref, scope = AST.ast_lookup(scope, typeref)
    if ref then return AST.ast_actual_type(scope, ref) end
  else
    reason = "illegal type construct"
  end
  return "illegal", reason
end

-- utils
function dump(o, scope, visited)
  visited = visited or { }
  scope = scope or ""
  if type(o) == "table" then
    if not visited[o] then
      visited[o] = true
      print(scope)
      for k,v in pairs(o) do
        if type(k) == "string" then
          dump(v, scope..'.'..k,visited)
        end
      end
    end
  else
    print(scope, " = ", o)
  end
end

--
-- helper functions
--

function AST.pairs(scope, filter)
  local is_enum_or_bitmap, visited, reserved, i, eon, sorted = is(scope).a(AST.Enum), {},{}, 0, 0, {}, {}
  for i=1,#scope do -- preserve node declaration order in scope
    if is(scope[i]).a(AST.Node) then
      visited[scope[i]]=true
      sorted[#sorted+1]=scope[i]
    else
      reserved[#reserved+1] = i
    end
  end
  eon = #sorted
  for i=1,#reserved do sorted[#sorted+1]=reserved[i] end
  reserved = nil
  for name,definition in pairs(scope) do
    if type(name) == "string" and not type(definition) == "function" and not name:match("ast_") then
      if (is_enum_or_bitmap and not visited[name]) or  (not is_enum_or_bitmap and not visited[definition]) then
        sorted[#sorted+1]=name
      end
    end
  end
  visited = nil
  i = 0
  return function () -- iterator...
    repeat
      i = i + 1
      if sorted[i] == nil then return nil end
    until not filter or is(scope[i]).a(filter)
    if i <= eon then -- AST.Node definitions
      return sorted[i].ast_name, sorted[i]
    else -- other definitions
      return sorted[i], scope[sorted[i]]
    end
  end
end

