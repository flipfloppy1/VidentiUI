
time = time + VUI_dt;

function tint(button, targetKey)

if (VUI_mouse.x >= button.position.x) and (VUI_mouse.x <= button.position.x + button.dimensions.x) and (VUI_mouse.y >= button.position.y) and (VUI_mouse.y <= button.position.y + button.dimensions.y) then
    if (VUI_mouse.down) then
        button.color = { r = 100, g = 100, b = 100, a = 255 }
        button.clicked = false;
    else 
        if (VUI_mouse.onRelease) then
            button.clicked = true;
        else
            button.color = { r = 200, g = 220, b = 255, a = 255 }
            button.clicked = false;
        end
    end
else
	if (VUI_key[targetKey].down) then
		button.color = {r=100,g=100,b=100,a=255};
		button.clicked = false;
	else
		if (VUI_key[targetKey].onRelease) then
			button.clicked = true;
		else
			button.color = {r=255,g=255,b=255,a=255}
		end
	end
end


return button
end

ui.buttons["1"] = tint(ui.buttons["1"], "P");
ui.buttons["2"] = tint(ui.buttons["2"], "S");
ui.buttons["3"] = tint(ui.buttons["3"], "Q");

if (ui.buttons["1"].clicked) then
    VUI_nextScript = "resources/play.lua"
    ui.buttons["1"].clicked = false
end

if (ui.buttons["2"].clicked) then
    VUI_nextScript = "resources/update.lua"
    ui.buttons["2"].clicked = false
end

if (ui.buttons["3"].clicked) then
    VUI_nextScript = "resources/quit.lua"
    ui.buttons["3"].clicked = false
end

ui.buttons.timeText.text = "Time played: " .. math.floor(time) .. "s"
