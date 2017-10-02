--[[
    Patched Lua require method
    Authors of original version:
	<Mary>
	<JazzMan>
]]

if not orig__require then
	orig__require = require
	local orig__dofile = dofile
	local _G = _G
	local str_lower = string.lower
	local log = log
	local loadstring = loadstring
	local pcall = pcall
	local tostring = tostring
	local unpack = unpack
	local io_open = io.open

	local __require_pre = {} -- Callbacks executed before script required
	local __require_after = {} -- Callbacks executed after script required
	local __require_override = {} -- Overriden require requests

	local G = getfenv(0)
	G.__require_pre = __require_pre
	G.__require_after = __require_after
	G.__require_override = __require_override

	local was__required = {}

	local first_require_clbk
	first_require_clbk = function()
		if rawget(_G, '__first_require_clbk') then
			local exec = __first_require_clbk
			__first_require_clbk = nil
			exec()
		end
		
		first_require_clbk = function() end
	end

	local function exec_before_clbks( path )
		local before_clbk = __require_pre[path]
		if before_clbk then
			before_clbk()
		end
	end

	local function exec_after_clbks( path )
		local after_clbk = __require_after[path]
		if after_clbk then
			after_clbk()
		end
	end

	local exts = { '', '.lua', '.luac' }

	--[[
	Arguments:
		(in_path, type: string) : path to the required file, if file wasn't found, it calls original require function, if safe is false/nil
		[safe, type: boolean/anything] : If isn't false or nil, it doesn't call original require function and pcalls loaded chunk, in the result preventing crashes, when you require file from disk.
		[reload, type: boolean/anything] : If isn't false or nil, it ignores was__required check and loads script again. (So it acts like dofile)
	Returns:
		true or non-false/nil result
	]]
	function require( in_path, safe, reload )
		if in_path:find("lib/") then
			return orig__require(in_path)
		end
		
		log('Require ', in_path)
		
		-- Executing clbk on 1st require, function will void itself on execution, result doesn't matter
		first_require_clbk()
		
		local path = str_lower(in_path)
		
		-- Check if we were required already, if yes, return what returned dofile or true
		local __was_required = was__required[path]
		if (__was_required ~= nil and not reload) then
			return unpack( __was_required )
		end
		
		--------require function patch-------
		
		-- Executing clbks before require
		local before_clbk = __require_pre[path]
		if before_clbk then
			before_clbk()
		end
		
		-- Executing overriden clbks
		local override_clbk = __require_override[path]
		if override_clbk then
			return override_clbk()
		end
		
		local f
		local final_path
		local i = 1
		repeat
			final_path = path..exts[i]
			f = io_open(final_path, 'rb')
			i = i + 1
		until f or i > 3
		if not f then
			if safe then
				return log("Error: filename "..in_path.." wasn't found!\n")
			end
			local ret = orig__dofile( in_path )
			local after_clbk = __require_after[path]
			if after_clbk then
				after_clbk()
			end
			return ret
		end
			
		local exec, str_err = loadstring( f:read('*all'), final_path )
		f:close()
		if exec then
			if ( safe ) then
				-- Multiple return support
				local res = { pcall(exec) }
				if res[1] then
					res = { unpack( res, 2 ) }
					was__required[path] = res
					local after_clbk = __require_after[path]
					if after_clbk then
						after_clbk()
					end
					return unpack(res)
				else
					log('Error: '..res[2]..'\n')
				end
			else
				local res = { exec() }
				was__required[path] = res
				local after_clbk = __require_after[path]
				if after_clbk then
					after_clbk()
				end
				return unpack( res )
			end
		elseif str_err then
			log('Error: '..str_err..'\n')
		end
	end

	-- Clear old required stuff
	function reset_requires()
		was__required = {}
	end 
end