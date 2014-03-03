mkdir m4
gtkdocize || exit 1
autoreconf -vfi
./configure --enable-gtk-doc

