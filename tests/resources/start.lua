ui = { };
ui.buttons = { }
ui.buttons.background = { position = { x = 0.0, y = 0.0 }, dimensions = { x = 1.0, y = 1.0 }, texture = "resources/textures/background.png" };

ui.buttons.button1 = { position = { y = 0.4 }, dimensions = { x = 0.175, y = 0.07 * VUI_winX / VUI_winY }, texture = "resources/textures/button.png", layer = 1 }
ui.buttons.button2 = { position = { y = 0.6 }, dimensions = { x = 0.175, y = 0.07 * VUI_winX / VUI_winY }, texture = "resources/textures/button.png", layer = 1 }
ui.buttons.button3 = { position = { y = 0.8 }, dimensions = { x = 0.175, y = 0.07 * VUI_winX / VUI_winY }, texture = "resources/textures/button.png", layer = 1 }
ui.buttons.button1.position.x = 0.5 - ui.buttons.button1.dimensions.x / 2
ui.buttons.button2.position.x = 0.5 - ui.buttons.button2.dimensions.x / 2
ui.buttons.button3.position.x = 0.5 - ui.buttons.button3.dimensions.x / 2
ui.buttons.button1.clicked = false;
ui.buttons.button2.clicked = false;
ui.buttons.button3.clicked = false;
time = 0.0;
VUI_nextScript = "resources/update.lua";
