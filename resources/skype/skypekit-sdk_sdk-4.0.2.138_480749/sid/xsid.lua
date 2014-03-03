require "sid"

AST.Struct      = class{}:is_a(AST.Type, AST.Scope) -- only fields in it, maybe typedefs...

function STRUCT(name_or_decorator, ...)
  return AST.parse_named_or_decorated("Struct", AST.where(2), name_or_decorator, ...)
end

local keep_ast_actual_type = AST.ast_actual_type
function AST.ast_actual_type(scope, typeref, inlist)
  local kind, reason = keep_ast_actual_type(scope, typeref, inlist)
  if kind == "illegal" then
    if reason == "illegal type construct" then
      local ref, scope = AST.ast_lookup(scope, typeref)
      if is(ref).a(AST.Struct) then 
         return "struct", ref 
      end
    end
  end 
  return kind, reason
end


