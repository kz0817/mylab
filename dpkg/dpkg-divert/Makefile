all: \
  whitecat_1.0_all.deb \
  whitecat_2.0_all.deb \
  whitecat-conf_1.0.0_all.deb \
  whitecat-conf_1.0.1_all.deb

whitecat_1.0_all.deb: whitecat1/DEBIAN/control
	fakeroot dpkg -b whitecat1 .

whitecat_2.0_all.deb: whitecat2/DEBIAN/control
	fakeroot dpkg -b whitecat2 .

whitecat-conf_1.0.0_all.deb: whitecat-conf100/DEBIAN/control
	fakeroot dpkg -b whitecat-conf100 .

whitecat-conf_1.0.1_all.deb: whitecat-conf101/DEBIAN/control
	fakeroot dpkg -b whitecat-conf101 .


.PHONE: clean
clean:
	rm -f *.deb
