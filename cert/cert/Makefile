# The following path is for Ubuntu 18.04.
ORIG_OPENSSL_CNF ?= /etc/ssl/openssl.cnf


myserver.crt: myserver.csr priv.key openssl_san.cnf
	openssl x509 -req -days 3650 -sha256 -in $< -signkey $(word 2, $^) -extfile $(word 3, $^) -extensions SAN -out $@

openssl_san.cnf: $(ORIG_OPENSSL_CNF) openssl-san-conf-generator.sh
	./openssl-san-conf-generator.sh $< > $@

myserver.csr: priv.key
	openssl req -new -key $< -sha256 -subj "/C=JP/ST=Tokyo/L=Minato/O=MyHouse/CN=myserver" -out $@

priv.key:
	openssl genrsa -out $@ 2048

.PHONY: clean

clean:
	rm -fr priv.key myserver.csr openssl_san.cnf myserver.crt
