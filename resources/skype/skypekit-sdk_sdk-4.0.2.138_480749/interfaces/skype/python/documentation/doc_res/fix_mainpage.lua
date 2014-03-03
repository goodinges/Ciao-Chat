
local FileName = "../html/help.html";

x1 = [[<table width="100%%" cellpadding="0" cellspacing="0">
  <tr valign="top">
    <td width="100%%">&nbsp;</td>
    <td>
      <table cellpadding="0" cellspacing="0">]];
	  
x2 = [[</table>
    </td>
  </tr>
</table>]];

inputfile 	= assert (io.open(FileName, "r"));
local content = inputfile:read("*a");
inputfile:close();
local replacements = 0;	
content, found = content:gsub(x1, "");
content, found = content:gsub(x2, "");
outputfile 	= assert (io.open(FileName, "w"));	
outputfile:write(content);
outputfile:close();	
