package.path = package.path .. ';../?.lua;../../../sid/?.lua;'

if not arg or #arg < 2 then
  print 
[[lua binpsnif.lua <sidinterface> <logpath>  [-server]
you must have sidinterface in your LUA_PATH
for example
cd ~/skypekit/sid
export LUA_PATH="$LUA_PATH;`pwd`/?.lua"
cd ../interfaces/skype
export LUA_PATH="$LUA_PATH;`pwd`/?.lua"
cd -
alias binpsnif="lua `pwd`/binpsnif.lua Skype"
cd ../interfaces/skype/cpp_embedded/build
./skypekitclient -t token.pub -r s
binpsnif s > s.txt
or
if logged from runtime side
./skypekit -r s 
binpsnif s -server > s.txt

if you want to look at the stats only
binpsnif s -server > /dev/null

luajit 2.0 from luajit.org can bring some speed improvment
]]
  return
end

require(arg[1]) -- load SID model

--
-- locate definition
--
local sid_root = nil
for k,v in pairs(_G) do
  if is(v).a(AST.Sid) then sid_root = v; break end
end  

local memoized = {  }
function AST.Sid:ast_index(memoized)
  memoized[0] = { name = self.ast_name, events = {}, actions = {} }
  for i=1,#self do self[i]:ast_index(memoized[0]) end
end
function AST.Namespace:ast_index(memoized)
  for i=1,#self do self[i]:ast_index(memoized) end
end
function AST.Module:ast_index(foo)
  local mod = { name = self.ast_name, events = {}, actions = {}, props = {} }
  memoized[self.ast_key] = mod
  for i=1,#self do self[i]:ast_index(mod) end
end
function AST.Action:ast_index(memoized)
  memoized.actions[self.ast_tag] = { name = self.ast_name, action = self }
end
function AST.Property:ast_index(memoized)
  local _, t = AST.ast_actual_type(AST.ast_scope(self), self.type)
  memoized.props[self.ast_key] = { name = self.ast_name, type = t }
end
function AST.Event:ast_index(memoized)
  memoized.events[self.ast_tag] = { name = self.ast_name, event = self }
end
function AST.Node:ast_index(memoized) -- fallback
end
sid_root:ast_index(memoized)

local format = string.format
local function format_byte(byte, idx)
  if #byte < idx then return '       ' end
  local n = byte:byte(idx)
  local c = string.char(n)
  if n < 32 or n > 126 then c = ' ' end
  return format('%s(%03d:%2X)', c, n, n)
end

local function format_name(name)
  return format('%.31s', name:sub(-31))
end

local function modid2name(modid)
end

local function actid2name(modid, actid)
  local info = memoized[modid] and memoized[modid].actions[actid]
  if not info then return format_name('unkown '..actid) end
  return format_name(memoized[modid].name..'.'..info.name), info.action
end

local function evtid2name(modid, evtid)
  local info = memoized[modid] and memoized[modid].events[evtid]
  if not info then return format_name('unkown '..evtid) end
  return format_name(memoized[modid].name..'.'..info.name), info.event
end

local function propid2name(modid, propid)
  if modid == -1 then
    local candidates, sep = '', ''
    for m,v in pairs(memoized) do
      if m >  0 and v.props[propid] then
        candidates = candidates..sep..v.name..'.'..v.props[propid].name
        sep = '|'
      end
    end
    return format_name(candidates ~= '' and candidates or '?')
  end
  return format_name(memoized[modid] and (memoized[modid].name..'.'..(memoized[modid].props[propid] and memoized[modid].props[propid].name or '?')) or 'unknown')
end

local function fieldid2name(action, fieldid)
  if action then 
    for i=1,#action do
      local f = action[i]
      if f.ast_tag == fieldid then return format_name(f.ast_name),f end 
    end
  end
  return fieldid,nil
end

local function enum2name(modid, propid,val)
  local t = memoized[modid] and memoized[modid].props[propid] and memoized[modid].props[propid].type or nil
  if t then
    for i=1,#t do
      local v=t[i]
      if val == v.ast_key then return format_name(v.ast_name) end
    end
  end
  return val
end


local function enumparm2name(field,val)
  if field then
    local k,t = AST.ast_actual_type(AST.ast_scope(field), field.type)
    if k == "PROPKEY" then return propid2name(-1,math.floor(val/4)) end
    for i=1,#t do
      local v=t[i]
      if val == v.ast_key then return format_name(v.ast_name) end
    end
  end
  return val
end

--
-- Buffered input
--

local ceil = math.ceil
local function read(self, size)
  local buffer, dest = self.buffer, self.dest
  while size > #buffer and self.log do
    buffer = buffer..(self.log:read(ceil((size-#buffer)/4096)*4096) or '')
    if (size > #buffer) then self:nextlog() end
  end
  self.buffer = buffer:sub(size+1)
  self.read_count = self.read_count + size
  for i = 1,size do dest[#dest+1] = self.sep..format_byte(buffer,i); self.sep = '\n' end
  return buffer:sub(1, size)
end

--
-- Protocol parsing
--

local function read_uint(stream)
  local i, v, dest = 0, 0, stream.dest
  repeat
    local byte = stream:read(1):byte()
    v = v + (byte%2^7)* 2^(7*i)
    i=i+1
  until byte < 2^7
  dest[#dest+1] = v
  return v
end

local modf = math.modf
local function read_int(stream)
  local n,s = modf(stream:read_uint()/2)
  local dest = stream.dest
  if s == 0 then
    dest[#dest] = n
    return n
  end
  dest[#dest] = -n
  return -n
end

local function read_binary(stream)
  local size = stream:read_uint()
  if size == 0 then return '' end
  local dest = stream.dest
  local curr = #dest
  local v = stream:read(size)
  --             'c(000:AA)'
  dest[curr+1] = '\nHide Bin '  
  for i=curr+2,#dest do dest[i] = nil end
end

local function read_string_compact(stream)
  local size = stream:read_uint()
  if size == 0 then return '' end
  local dest = stream.dest
  local curr = #dest
  local v = stream:read(size)
  --             'c(000:AA)'
  dest[curr+1] = '\nPack Str '..format_name(v):gsub('[^ -~]',' ')  
  for i=curr+2,#dest do dest[i] = nil end
end

local function read_string(stream)
  local size = stream:read_uint()
  if size == 0 then return '' end
  return stream:read(size)
end

local function read_list(stream)
  local kind = stream:read(1)
  while kind ~= ']' do
    stream[kind](stream)
    kind = stream:read(1)
  end
end

local function read_parameters(stream,action)
  local kind, dest, f, v = stream:read(1), stream.dest
  while kind ~= 'z' do
    if kind ~= 'N' then 
      f = stream:read_uint() -- field id
      dest[#dest], f = fieldid2name(action,f)
      if not stream[kind] then return false end
      v = stream[kind](stream)
      if kind == 'e' or kind == 'b' then dest[#dest] = enumparm2name(f,v) end
    end
    kind = stream:read(1)
  end
  return true
end

local function read_nothing(stream) end

local function add_occurance(freq, name)
  if freq[name] then 
    freq[name] = freq[name] + 1
  else 
    freq[#freq+1] = name
    freq[name] = 1
  end
end

local function read_get_response(stream, freq)
    local continue_sign, dest, v = ',', stream.dest
    while continue_sign == ',' do
      local modid = stream:read_uint()
      while continue_sign == ',' do
        local oid = stream:read_uint()
        local kind = stream:read(1)
        while kind ~= ']' do
          local propid = stream:read_uint()
          local name =  propid2name(modid,propid)
          if freq then add_occurance(freq,name) end
          dest[#dest] = name
          v = stream[kind](stream)
          if kind == 'e' or kind == 'b' then dest[#dest] = enum2name(modid,propid,v) end
          kind = stream:read(1) -- ] finish the list
        end
        if kind ~= ']' then return false end
        continue_sign = stream:read(1)
      end
      if continue_sign ~= ']' then return false end
      continue_sign = stream:read(1)
    end
    if continue_sign == ']' then
    return stream:read(1) == 'z'
  end
  return false
end

local function read_get_request(stream)
  local continue_sign, dest = ',', stream.dest
  while continue_sign == ',' do
    while continue_sign == ',' do -- read propids
      local propid = stream:read_uint()
      dest[#dest] = propid2name(-1,propid)
      continue_sign = stream:read(1)
    end
    if continue_sign ~= ']' then return false end
    local modid = stream:read_uint()
    dest[#dest] = modid2name(modid)
    continue_sign = ','
    while continue_sign == ',' do -- read oids
      local oid = stream:read_uint()
      continue_sign = stream:read(1)
    end
    if continue_sign ~= ']' then return false end
    continue_sign = stream:read(1)
  end
  if continue_sign == ']' then
    return stream:read(1) == 'z'
  end
  return false
end

--
--
--

LogParserBase = class{
  read = read,
  read_parameters = read_parameters,
  read_get_response = read_get_response,
  read_string = read_string,
  read_uint = read_uint,
  read_int = read_int,

  --
  -- type driven parsing
  --

  N     = read_nothing,
  i     = read_int,
  e     = read_uint,
  O     = read_uint,
  u     = read_uint,
  U     = read_uint,
  T     = read_nothing,
  F     = read_nothing,
  S     = read_string_compact, -- or read_string,
  X     = read_string_compact, -- read_string,
  f     = read_string_compact, -- read_string,
  B     = read_binary, -- or read_string if we want to display the binary
  ['['] = read_list,
  ['{'] = read_parameters,
}

function LogParserBase:new(path, pending, direction)
  self.getcount = 0
  self.buffer = ''
  self.sep = ''
  self.pending = pending
  self.name = path..'_log_'..direction..'.1'
  self.log  = assert(io.open(self.name, "rb"), "can't open log file"..self.name)
  self.dest = {}
  self.stats = {}
  self.read_count = 0
end

function LogParserBase:nextlog(dir)
  self.log:close()
  local name = self.name
  self.name = name:sub(1,-2)..(tonumber(name:sub(-1))+1)
  self.log = io.open(self.name, "rb")
end

local concat = table.concat
function LogParserBase:read_payload()
  local header, kind, id
  if not self.resync then
    self.dest = {}
    header = self:read(2)
  else
    self.dest = self.resync
    header = self.resync.c .. self:read(1)
  end
  if self[header] then 
    self.resync = nil
    local stats = self.stats[header] or { num = 0, vol = 0, dist = {} }
    self.stats[header] = stats
    header, kind, id = self[header](self) 
    header = concat(header)
    stats.num = stats.num + 1
    stats.vol = stats.vol + self.read_count
    stats.dist[#stats.dist+1] = self.read_count
    self.read_count = 0
  elseif #header == 2 then
    self.resync = { self.dest[2], c = header:sub(2,2) }
    header = self.dest[1] .. 'ignored'
    self.read_count = 1
  end
  return header, kind, id 
end

RspLogParser = class{
  skip_handshake = function (self)
    local rsp = self:read(2)
    self.dest = {}
    return "HS "..rsp
  end,

  Zr = function(self)
    local rid, dest   = self:read_uint(), self.dest
    dest[#dest] = 'rid:'..rid
    self:read_parameters(self.pending[rid] and self.pending[rid].response)
    return dest, 'r', rid
  end,

  Zg = function(self)
    local getcount = self.getcount
    self:read_get_response()
    self.getcount = getcount + 1
    return self.dest, 'g', getcount
  end,

  ZE = function(self)
    local modid, dest = self:read_uint(), self.dest
    dest[#dest] = modid2name(modid)
    local evtid = self:read_uint()
    local name, event= evtid2name(modid, evtid)
    local freq = self.stats.ZE.freq or {}
    self.stats.ZE.freq = freq
    add_occurance(freq,name)
    dest[#dest] = name
    self:read_parameters(event)
    return dest, 'E'
  end,

  ZC = function(self)
    local freq = self.stats.ZC.freq or {}
    self.stats.ZC.freq = freq
    self:read_get_response(freq)
    return self.dest, 'C' 
  end,
}:is_a(LogParserBase)

function RspLogParser:new(path, pending, ext)
  LogParserBase.new(self, path, pending, ext)
end

ReqLogParser = class{
  read_get_request = read_get_request,

  skip_handshake = function (self)
    local hsdata_sz, hs = tonumber(self:read(8), 16), ""
    assert(hsdata_sz, 'probably missing -server in command line if the logs were recorded from runtime side')
    if hsdata_sz > 0 then hs = self:read(hsdata_sz) end
    self.dest = {}
    return "HS "..hs
  end,

  ZR = function(self)
    local dest  = self.dest
    local modid = self:read_uint()
    dest[#dest] = modid2name(modid)
    local actid = self:read_uint()
    local name, action = actid2name(modid, actid)
    dest[#dest] = name
    local rid   = self:read_uint()
    dest[#dest] = 'rid:'..rid
    self:read_parameters(action and action.request)
    self.pending[rid] = { response = action and action.response }
    return dest, 'R', rid
  end,

  ZG = function(self) 
    local getcount = self.getcount
    self:read_get_request() 
    self.getcount = getcount+1
    return self.dest, 'G', self.getcount
  end,
}:is_a(LogParserBase)

function ReqLogParser:new(path, pending, ext)
  LogParserBase.new(self, path, pending, ext)
end

local pending = {}
local log_out = arg[3] and ReqLogParser (arg[2], pending, 'in') or ReqLogParser (arg[2], pending, 'out')
local log_in  = arg[3] and RspLogParser(arg[2], pending, 'out') or RspLogParser (arg[2], pending, 'in')

local pending, spaces = arg[3] and log_in.pending or log_out.pending, string.rep(' ', 20)
local payload_out, kind_out, count_out
local payload_in, kind_in, count_in = nil, nil, -1
local stalled = true
local nok = true
local RCMD, GCMD = 'r', 'g'
-- logical ordering as we haven't the actual timing...
io.write(log_in:skip_handshake(), '\n')
io.write(spaces, log_out:skip_handshake(), '\n')
payload_in, kind_in, count_in = log_in:read_payload()
repeat
  payload_out, kind_out, count_out = log_out:read_payload()
--  nok, payload_out, kind_out, count_out = pcall(log_out.read_payload, log_out)
--  if not nok then print('ERR out:', table.concat(log_out.dest):gsub('\n','\n'..spaces)); payload_out = '' end
  local s = payload_out:gsub('\n','\n'..spaces)
  io.write(spaces, s, '\n')
  repeat
    nok = false
    if not stalled then
      io.write(payload_in, '\n')
      if kind_in == RCMD then pending[count_in] = nil end
      payload_in, kind_in, count_in = log_in:read_payload()
--      nok, payload_in, kind_in, count_in = pcall(log_in.read_payload, log_in)
--      if not nok then print('ERR in:', table.concat(log_in.dest)); io.write(payload_in, '\n');  end
    end
    stalled = not nok and (payload_in == '' or (kind_in == GCMD and count_in >= log_out.getcount) or (kind_in == RCMD and not pending[count_in]))
  until stalled or nok
until (payload_in == '' and (payload_out == '' or payload_out == '\n'))


--
-- compute stats
--

function nciles(n, dist, total)
  local ciles, decpop, indexes = {}, total/n, {}
  for i=1,#dist do indexes[i] = i end 
  table.sort(indexes, function(a,b) return dist[a] < dist[b] end)
  local bdec, i, lim, sum, rc, edec = dist[indexes[1]], 1, decpop, 0, 0
  for c=1,n do
    while sum < lim and i < #dist do
      edec = dist[indexes[i]]
      sum = sum + edec
      i = i + 1
      rc = rc + 1 
    end
    ciles[c] = { bdec = bdec , edec = edec, rc = math.ceil(rc*100/#dist) }
    bdec = edec
    lim  = lim + decpop
  end
  return ciles
end

function print_stats(log, num_cile, num_freq)
  num_cile = num_cile or 5 
  num_freq = num_freq or 5 
  for req,stats in pairs(log.stats) do
    io.stderr:write(req, ' num=',stats.num,',\n   vol=',stats.vol, ',\n  ')
    for d,c in ipairs(nciles(num_cile,stats.dist,stats.vol)) do 
      io.stderr:write(' d', d, ':', c.bdec, '-', c.edec, '(', c.rc, '%)') 
    end
    if stats.freq then
      io.stderr:write(',\n')
      local freq, num, sum = stats.freq, #stats.freq, 0
      for i=1,num do sum = sum + freq[freq[i]] end
      table.sort(freq, function (a, b) return freq[a] > freq[b] end)
      for i=1,math.min(num_freq,num) do 
        local name = freq[i]
        io.stderr:write('    ', name,' (',math.floor((freq[name]/sum)*100),'%)\n' ) 
      end
    else
      io.stderr:write('\n')
    end 
    io.stderr:write('\n')
  end
end

io.stderr:write('sizes of requests by decile of request kind bandwidth (ie vol/#decile)\n',
                  'and % of requests included in the decile and the previous ones\n\n')
print_stats(log_in)
print_stats(log_out)

