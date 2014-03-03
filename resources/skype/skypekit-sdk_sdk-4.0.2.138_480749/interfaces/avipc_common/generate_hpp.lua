package.path = package.path .. ';../../sid/?.lua'

require 'generator'

function AST.Sid:generate_hpp(scope, generator)
	if self.comment then 
		generator:write("/** "..self.comment.." */\n")
	end 
	generator:write('struct '.. self.ast_name..'\n{\n'):indent()
	
	for _,v in ipairs(self) do
		v:generate_hpp(scope..'.'..v.ast_name, generator)
		generator:write('\n')
	end
	generator:undent():write('};\n\n')
end

local builtin2cpp = { 
	int						= { ["in"] = "int ",						out = "int &",					 decl = "int "	 	},
	uint					 = { ["in"] = "uint ",				 	out = "uint &",	 	decl = "uint "		},
	string				 = { ["in"] = "String &",			 	out = "String & ",			decl = "String "	},
	boolean				= { ["in"] = "bool ",				 	out = "bool & ",				decl = "bool "	 	},
	binary				 = { ["in"] = "Binary &",	out = "Binary& ",	decl = "Binary " 	},
}

local prefix = { ["in"] = "const Sid::", out = "Sid::", decl = "Sid::" }
local suffix = { ["in"] = "", out = "& ", decl = "" }
local prefix_list = { ["in"] = "Sid::List_", out = "Sid::List_", decl = "Sid::List_" }
local suffix_list = { ["in"] = "", out = "& ", decl = "" }

function cppify_scope(scope, t, dir)
	assert(type(t) == "string")
	-- drop namespace
	local tt = t:gsub('%.','::')
	for w in string.gfind(tt, '(%a+)::') do
		if AST.ast_actual_type(scope, w) == "namespace" then 
			tt = tt:gsub(w..'::', '')
		end
	end
	return tt
end

function get_type(scope, t, dir)
	local builtin = builtin2cpp[t]
	if builtin then 
		if t == "string" or t == "binary" then
			return prefix[dir]..builtin[dir]
		end
		return builtin[dir] 
	elseif AST.ast_actual_type(scope, t) == "module" then 
		return t..' '..suffix[dir]
	elseif AST.ast_actual_type(scope, t) == "struct" then 
		return t..' '..suffix[dir]
	elseif AST.ast_actual_type(scope, t) == "enum" then 
		return t..' '..suffix[dir]
	end
	return prefix_list[dir]..get_type(scope, t.of, "decl", no_builtin_module)..suffix_list[dir]
end

function get_field(scope, generator, field, sep)
	local res = ''
	if generator.generate_comments and field.comment then
		res = res .. '// ' .. field.comment .. '\n'
	end
	
	local t = get_type(scope, field.type, field.dir)
	res = res .. t
	
	local field_name = field.ast_name
	res = res .. field_name

	if sep then res = res .. ', ' end
	return res
end

function get_parameters(scope, generator, inparms, outparms)
	local res = ''
	local returned_field
	inparms = inparms or {}
	outparms = outparms or {}
	local nodefs = {}
	-- Parameter order:
	-- 1 in
	-- 2 out
	for p=1,#inparms do
		local field = inparms[p]
		field.dir = "in"
		nodefs[#nodefs+1] = field 
	end
	for p=1,#outparms do
		local field = outparms[p]
		field.dir = "out"
		if field.ast_decoration and field.ast_decoration.RETURN then
			returned_field = field
		else
			nodefs[#nodefs+1] = field
		end
	end
	for p=1,#nodefs do
		res = res .. get_field(scope, generator, nodefs[p], p < #nodefs)
	end
	return res, returned_field and get_type(scope, returned_field.type, "decl", false)..' ' or nil
end

function AST.Action:generate_hpp(scope, generator)
	local p, r = get_parameters(scope, generator, self.request, self.response)

	if self.request and self.request.comment then 
		generator:write("/** "..self.request.comment.." */\n")
	end 
	if self.response and self.response.comment then
		generator:write("/** "..self.response.comment.." */\n")
	end	
	if not r then
		r = 'void '
	end
	if (self.ast_name == "GetInterfaceVersion") then
		generator:write('virtual ', r, self.ast_name, '(', p, ')\n')
		generator:write('#ifndef SID_SKYPEKIT_SIDE\n{ version = 32; return true; }\n#else\n=0;\n#endif')
	else
		generator:write('virtual ', r, self.ast_name, '(', p, ') = 0;\n')
	end
	
end

function AST.Struct:generate_hpp(scope, generator)
	if self.comment then 
		generator:write("/** "..self.comment.." */\n")
	end
	generator:write('struct '..self.ast_name..'\n{\n')
		:indent()
		for _,v in ipairs(self) do
			if is(v).a(AST.Field) then
				local t = get_type(scope, v.type, "decl")
				generator:write(t..v.ast_name..';')
			else
				v:generate_hpp(scope..'.'..v.ast_name, generator)
			end
			generator:write('\n')
		end
	generator:undent()
		:write('};\n')
end

function AST.Module:generate_hpp(scope, generator)
	if self.comment then 
		generator:write("/** "..self.comment.." */\n")
	end
	generator:write('struct '..self.ast_name..'\n{\n')
		:indent()
		for _,v in ipairs(self) do
			if is(v).a(AST.Field) then
	local t = get_type(scope, v.type, "decl")
	generator:write(t..v.ast_name..';')
			else
			v:generate_hpp(scope..'.'..v.ast_name, generator)
			end
			generator:write('\n')
		end
	generator:undent()
		:write('};\n')
end

function AST.Property:generate_hpp(scope, generator)
	local t = get_type(scope, self.type, "in")
	generator:write(t..self.ast_name..';')
end

function AST.Enumerated:generate_hpp(scope,generator)
	if self.comment then 
		generator:write("/** "..self.comment.." */\n")
	end
	generator:write('enum '..self.ast_name..'\n{\n'):indent()
		for _,enum in ipairs(self) do 
			generator:write(enum.ast_name..'='..enum.ast_key..',\n')
		end
	generator:undent()
		:write('};\n')
end

