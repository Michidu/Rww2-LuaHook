--[[ Keys table:
	Keyboard: a-z (lowered!), 0-9, left shift, right shift, left ctrl, right ctrl, space, f1-14 num 0-9, num +, num -,  num . , num *, 
	num enter, num lock, num /, home, insert, page up, page down
	Mouse: left_button, right_button, middle_button, x_button_1, x_button_2, x_button_3, x_button_4, x_button_5, wheel_up, wheel_down
	
	Possible table keys:
	ig_chat = true/false --If true, this will execute script and/or callback when you're typing something into chat, text input (broken)
	script = 'path_to_script' --Path to the script, that will be executed after key pressed
	handled_callback = 'path_to_script' --Path to the script, that dofile will return callback function, that will be executed when binded key pressed
	callback = function() .. methods .. end/direct_function --Function, that will be executed after key pressed
	no_stuck = true/false --If true, key will no longer repeat itself by holding key down long enough
	
	Configuration entry examples:
	--If you want to execute script file on keypress
	['left shift'] = { script = 'test.lua' },
	
	--If you want to execute function on keypress
	['z'] = { callback = function() ... end },
	
	--If you want to execute script, ignoring check if you're in chat and also never repeat script, if key holden.
	['x'] = { script = 'test.lua, ig_chat = true, no_stuck = true },
	
	** handled_callback note:
	--This way is recommended as it eliminates repeated and unnecessary script interpretation.
	--Though it have cons if you're developing some script, as it won't be reloaded anymore, unless you intepretate this script again
	
	--Comma after every entry is required, except if it is last entry in the table, where it isn't necessary.
]]

-- Return format: Keyconfig's table
return {
	['f5'] = { script = 'LuaMods/user_script.lua' }
}