# DDE Global Menu Service

Create the specific DBus (com.canonical.AppMenu.Registrar) for the global menu & gtk global menu dbus proxy

Most of the codes are from the KDE.

## How to install

* download the latest release
* unzip the release file
* cp `dde-globalmenu-service` to `/usr/bin/`
* cp `dde-globalmenu-service.desktop` to `~/.config/autostart/`
* edit `~/.gtkrc-2.0`, add `gtk-modules=appmenu-gtk-module`
* edit `~/.config/gtk-3.0/settings.ini`, add `gtk-modules=appmenu-gtk-module` in `[Settings]` section
* log out or reboot 
