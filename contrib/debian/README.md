
Debian
====================
This directory contains files used to package ebitcashd/ebitcash-qt
for Debian-based Linux systems. If you compile ebitcashd/ebitcash-qt yourself, there are some useful files here.

## ebitcash: URI support ##


ebitcash-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install ebitcash-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your ebitcash-qt binary to `/usr/bin`
and the `../../share/pixmaps/ebitcash128.png` to `/usr/share/pixmaps`

ebitcash-qt.protocol (KDE)

