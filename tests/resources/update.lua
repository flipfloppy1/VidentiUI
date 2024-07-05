
time = time + VUI_dt;
ui.buttons.button1.color.r = (math.sin(time) + 1) * 127.5;
ui.buttons.button1.color.g = (math.cos(time) + 1) * 127.5;
ui.buttons.button1.color.b = (math.sin(time + 3.14159/2) + 1) * 127.5;
