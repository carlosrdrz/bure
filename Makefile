CXX=g++ -g
CPPFLAGS=`sdl-config --cflags` `xml2-config --cflags`
LDLIBS=`sdl-config --libs` `xml2-config --libs` -lSDLmain -lSDL_image -lSDL_ttf -lSDL_net

SRC=$(wildcard *.cpp GUI/*.cpp)
OBJ=$(SRC:.cpp=.o)

.PHONY: clean install

bure: $(OBJ)
	$(CXX) $(CPPFLAGS) -o bure $(OBJ) $(LDLIBS)

clean:
	rm -f $(OBJ) bure

install:
	mkdir -m 755 -p $(DESTDIR)/usr/
	mkdir -m 755 -p $(DESTDIR)/usr/games/
	mkdir -m 755 -p $(DESTDIR)/usr/share/
	mkdir -m 755 -p $(DESTDIR)/usr/share/applications/
	mkdir -m 755 -p $(DESTDIR)/usr/share/pixmaps/
	mkdir -m 755 -p $(DESTDIR)/usr/share/bure
	install -m 755 bure $(DESTDIR)/usr/games/bure
	install -m 755 -d data $(DESTDIR)/usr/share/bure/data
	install -m 755 -d maps $(DESTDIR)/usr/share/bure/maps
	install -m 744 maps/mapa.tmx $(DESTDIR)/usr/share/bure/maps
	install -m 755 -d data/chars_avatar $(DESTDIR)/usr/share/bure/data/chars_avatar
	install -m 744 data/chars_avatar/*.png $(DESTDIR)/usr/share/bure/data/chars_avatar
	install -m 744 data/gui.png $(DESTDIR)/usr/share/bure/data
	install -m 744 data/background.jpg $(DESTDIR)/usr/share/bure/data
	install -m 744 data/pixel_font.ttf $(DESTDIR)/usr/share/bure/data
	install -m 744 data/tiles.png $(DESTDIR)/usr/share/bure/data
	install -m 744 data/bure.desktop $(DESTDIR)/usr/share/applications
	install -m 744 data/bure.xpm $(DESTDIR)/usr/share/pixmaps
