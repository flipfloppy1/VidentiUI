ui = { };
ui.buttons = { }
ui.bg = { }
ui.bg.bg = { position = { x = 0.0, y = 0.0 }, dimensions = { x = 1.0, y = 1.0 }, texture = "resources/textures/background.png" };

ui.buttons["1"] = { position = { y = 0.4 }, dimensions = { x = 0.175, y = 0.07 * VUI_winX / VUI_winY }, texture = "resources/textures/button.png", layer = 1 }
ui.buttons["2"] = { position = { y = 0.6 }, dimensions = { x = 0.175, y = 0.07 * VUI_winX / VUI_winY }, texture = "resources/textures/button.png", layer = 1 }
ui.buttons["3"] = { position = { y = 0.8 }, dimensions = { x = 0.175, y = 0.07 * VUI_winX / VUI_winY }, texture = "resources/textures/button.png", layer = 1 }
ui.buttons["1"].position.x = 0.5 - ui.buttons["1"].dimensions.x / 2
ui.buttons["2"].position.x = 0.5 - ui.buttons["2"].dimensions.x / 2
ui.buttons["3"].position.x = 0.5 - ui.buttons["3"].dimensions.x / 2
ui.buttons["1"].clicked = false;
ui.buttons["2"].clicked = false;
ui.buttons["3"].clicked = false;
ui.buttons["1"].font = "resources/fonts/CourierScreenplay.ttf";
ui.buttons["1"].text = "Play";
ui.buttons["1"].textPos = { x = 0.465, y = 0.463 };
ui.buttons["3"].text = "Quit";
ui.buttons["3"].font = "resources/fonts/CourierScreenplay.ttf";
ui.buttons["3"].textPos = {};
ui.buttons["3"].textPos.x = 0.465;
ui.buttons["3"].textPos.y = 0.863;
ui.buttons["3"].fontColor = { r=255,g=255,b=255,a=255 };
time = 0.0;
VUI_nextScript = "resources/update.lua";

ui.buttons.timeText = {};
ui.buttons.timeText.font = "resources/fonts/Bagnard.otf";
ui.buttons.timeText.textPos = { x = 0.02, y = 0.07 };
ui.buttons.timeText.color = { r = 255, g = 255, b = 255, a = 0};
ui.buttons.timeText.dimensions = { x = 0, y = 0 };
