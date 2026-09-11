# SFML Image Shower

## Introducing

This simple image viewer can easily show you images using the path of a folder contain pictures with extension (.png, .jpeg, .jpg).

This application is a learning project, it contain nothing new to the table, just me creating software with SFML library.

### Features

Fully featured UI! It still simple, but it makes the software easy to use. Powered by TGUI's library, it has simple and clean UI.

It can iterate through images after being loaded, and have the ability to create floating images as needed.

No lagging while loading the images, you can load the images and they'll be displayed immediately.

Using SFML to render the images, which is powerful library being used.

### How to use

#### UI

Use the UI to navigate for controls, the top-bar is auto hide, so you need to move your mouse up the window to show it.

In floating window, the top-right box for moving the floating window freely around, and the button-right for resizing it.

#### Shortcuts

##### Main window:

Left right arrows to go back and forth through the images, up arrow to select random image.

##### In floating window:

###### Image:

Hold left mouse button for moving the image within the window, Hold middle mouse button to rotate the image left and right

Mouse wheel for scalling the image.

###### Floating window:

Hold left-shift + left mouse button for moving the floating window. 

Left-shift + mouse wheel to scale the floating window horizontally. 

Left-ctrl + mouse wheel to scale the floating window vertically.

'H' for always-on-top feature. Making all windows appear above all other windows.

'space' in specific window to make the floating window transparent. "NOTE: This feature is not fully supported. Use at your own risk"

## Compile the code

This application has been compiled with Visual studio 2022, it's easy to compiler it with gcc and clang, no additional big library is used.

The project uses C++17.

### Libraries

This code need 

- SFML 3.0.2v library by Laurent Gomila
- stb_image.h by Sean Barrett
- Portable File Dialogs by Sam Hocevar
- TGUI by Texus Bruno Van de Velde
