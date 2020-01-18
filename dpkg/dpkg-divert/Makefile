all: \
  whitecat_1.0_all.deb \
  whitecat_2.0_all.deb

whitecat_1.0_all.deb: whitecat1/DEBIAN/control
	dpkg -b whitecat1 .

whitecat_2.0_all.deb: whitecat2/DEBIAN/control
	dpkg -b whitecat2 .
