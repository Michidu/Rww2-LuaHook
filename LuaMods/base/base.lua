-- Check if global state exists
if not _G then
	return
end

local _G = _G
local pairs = pairs
local tostring = tostring

-- Improve 'log' function a little

local orig_log = log
_G.log = function( ... )
	if ( select('#', ...) == 0 ) then
		return
	end
	
	local str = ''
	local args = {...}
	
	for _, arg in pairs(args) do
		str = str .. tostring(arg) .. ' '
	end
	
	orig_log(str..'\n')
end

_G.void = function() end

-- Init

dofile("LuaMods/base/__require.lua")
dofile("LuaMods/base/pubinfloopv2.lua")

--[[
  Backuper script for easy backing up and restoring functions.
  
  (Backuper) Backuper:new('backuper_name') -- Initate new backuper for functions. Function returns new backuper.
  
  (function) Backuper:backuper('function_string') -- Store new original function into backuper, once function is stored here, 
  It cannot be overriden. As argument use function string. Function returns first function being stored here.
  
  Backuper:restore('function_string') -- Restore original function, that was stored here.
  
  Backuper:restore_all() -- Restores all original functions stored in backuper.
  new_function) Backuper:hijack('function_string', new_function) -- Backups function and then replace it by new_function. 
  Just note, that new_function will always receive original function as 1st argument.
  
  Backuper:add_clbk( 'function_string', new_function, id, pos ) -- Adds callback function, 
  this being executed before function being called (pos == 1) or after call (pos == 2). You can add multiple callbacks and all of them will be executed. 
  id can be any, it will be used in order to remove callback
  
  Backuper:remove_clbk( 'function_string', [id, pos] ) -- Performs operation, depending on id and pos arguments. 
  If pos is nil, this will remove before and after callbacks by id. If id is nil, this will remove all callbacks, depending on pos. 
  If both id and pos are nil, this will be equivalent to Backuper:restore()
]]

local backuper = require("LuaMods/base/backuper.lua"):new("backuper")
_G.backuper = backuper

-- Load keybinds
do
	local KeyInputCls = require('LuaMods/base/kbinput')

	local KeyInput
	if ( KeyInputCls ) then		
		KeyInput = KeyInputCls:new()
		_G.KeyInput = KeyInput
	end

	if (KeyInput) then
		KeyInput:run_updating(true)
		
		local keyboard_configuration = require("LuaMods/keyconfig")
		if (keyboard_configuration) then
			KeyInput.keys = keyboard_configuration
			KeyInput:help_setup()
		else
			log('{base.lua}', 'Failed to load keyconfig!')
		end
	else
		log('{base.lua}', 'Failed to init KeyInput!')
	end
end
