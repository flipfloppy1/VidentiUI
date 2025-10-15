![VUI Logo](VUILogo.png)

### A UI library targeting real-time applications, with elements specified using Lua.

> [!IMPORTANT]
> VidentiUI is no longer actively developed. While it was a fun project and
> I learned a lot from it, if I need something like it later I'll probably
> use a library with a proper layouting system, like [Clay](https://github.com/nicbarker/clay).

Features:

* Custom UI Element types and reactivity built in lua, including:
	* Querying for key presses, mouse clicks and mouse position in Lua.
	* Editing properties of UI elements through scripts, such as color, texture, position and dimensions.
* Support for custom rendering backends and wide cross-platform support

Goals:

* Fast
* Reliable (i.e. prefers null objects and error logs rather than crashing)
* Ease of use and extensibility
* Suited to games, not websites
