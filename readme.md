# DDE Global Menu Service

Create the specific DBus (com.canonical.AppMenu.Registrar) for the global menu & gtk global menu dbus proxy

Most of the codes are from the KDE.

## How to install

### For Deepin V20

Go to release page, download the latest deb package, and install the deb file.

### Build on UOS

* `sudo apt install git cmake g++ extra-cmake-modules libxcb-util-dev`
* `git clone https://github.com/SeptemberHX/dde-globalmenu-service.git`
* `cd dde-globalmenu-service-master`
* `mkdir build`
* `cd build`
* `cmake ..`
* `make -j8 package`
* Install the generated \*.deb file. Enable autostart from the application menu. 
* edit `~/.gtkrc-2.0`, add `gtk-modules=appmenu-gtk-module`
* edit `~/.config/gtk-3.0/settings.ini`, add `gtk-modules=appmenu-gtk-module` in `[Settings]` section
* log out or reboot 

### For other distributions

It needs to be built from source on your own.
