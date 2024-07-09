
time = time + VUI_dt;

function tint(button)

    if (VUI_mouse.x >= button.position.x) and (VUI_mouse.x <= button.position.x + button.dimensions.x) and (VUI_mouse.y >= button.position.y) and (VUI_mouse.y <= button.position.y + button.dimensions.y) then
        if (VUI_mouse.down) then
            button.color = { r = 100, g = 100, b = 100, a = 255 }
        else
            button.color = { r = 200, g = 220, b = 255, a = 255 }
        end
    else
        button.color = { r = 255, g = 255, b = 255, a = 255 }
    end

    return button
end

ui.buttons.button1 = tint(ui.buttons.button1);
ui.buttons.button2 = tint(ui.buttons.button2);
ui.buttons.button3 = tint(ui.buttons.button3);
