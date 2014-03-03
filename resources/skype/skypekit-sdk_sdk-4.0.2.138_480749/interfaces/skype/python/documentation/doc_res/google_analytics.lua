
require "lfs";

path 					= "..\\html\\";

-----------------------------------------------------------------------------------
-- Fixing Google Analytics

local GoogleAnalyticsSnippet = [[
<script type="text/javascript">

  var _gaq = _gaq || [];
  _gaq.push(['_setAccount', 'UA-1425156-1']);
  _gaq.push(['_trackPageview']);

  (function() {
    var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
    ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
    var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
  })();

</script>
]];

SnippetsInjected = 0;

function InjectGoogleAnalyticsSnippet (filename)

	inputfile 	= assert (io.open(filename, "r"));
	local content = inputfile:read("*a");
	inputfile:close();
	
	if content:find("</body>") then
		local replacements = 0;	
		
		content, replacements = content:gsub("</body>", "\n" .. GoogleAnalyticsSnippet .. "\n</body>", 1);
		SnippetsInjected = SnippetsInjected + replacements;
		
		outputfile 	= assert (io.open(filename, "w"));	
		outputfile:write(content);
		outputfile:close();	
	end;
end;


function FixGoogleAnalytics (current_path)

	local pattern = ".htm";

    for filename in lfs.dir(current_path) do
        if filename ~= "." and filename ~= ".." then
		
			--print(current_path .. filename);
			local attr = lfs.attributes (current_path .. filename)
			assert (type(attr) == "table")
			if attr.mode == "directory" then
				FixGoogleAnalytics (current_path .. filename .. '\\')
			else
				if string.find(filename, pattern) then
					print(filename);
					InjectGoogleAnalyticsSnippet(current_path .. filename);
				end;
			end;
        end;
    end;
end;

print("Adding Google Analytics snippets to javadoc pages");
FixGoogleAnalytics(path);
print(SnippetsInjected .. " google analytics snippets injected");