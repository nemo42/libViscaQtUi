libViscaQtUi
============

A QT UI for controlling a VISCA compatible camera using libVisca.

The development was tested using a Sony EVI-D100 and only the commands
supported by this camera was added to the UI.

Building
--------

This project depends on libvisca. First fetch and install libvisca:

```
git clone https://git.code.sf.net/p/libvisca/git libvisca
cd libvisca
autoreconf --install
./configure
make
sudo make install
```

To build libviscaqtui, clone the git repository and use `qmake` and `make` to
build.

```
git clone https://github.com/nemo43/libviscaqtui
cd libviscaqtui
qmake
make
```

TODO:
----
* Update UI with actual values from camera on start up
* Add and implement focus control
* Run slider updates on separate thread to free up UI
* Variable zoom/pan/tilt-speed
* Add One Push WB Trigger button
* Add serialport selection and configuration
* Add lookuptable for hex value vs. camera model/manufacturer and display in UI
