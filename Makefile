
force:

submodules/jansson/configure:
	cd submodules/jansson && autoreconf -i

submodules/jansson/Makefile: submodules/jansson/configure
	cd submodules/jansson && ./configure --prefix $(abspath .)

lib/libjansson.dylib: submodules/jansson/Makefile force
	$(MAKE) -C submodules/jansson && $(MAKE) -C submodules/jansson install

jansson: lib/libjansson.dylib

submodules/libjwt/configure:
	cd submodules/libjwt && autoreconf -i

submodules/libjwt/Makefile: submodules/libjwt/configure jansson
	cd submodules/libjwt && PKG_CONFIG_PATH=":$(abspath lib/pkgconfig):$(subst $(eval) ,:,$(wildcard /usr/local/Cellar/openssl/*/lib/pkgconfig)):$(PKG_CONFIG_PATH)" \
		./configure --prefix $(abspath .)

lib/libjwt.dylib: submodules/libjwt/Makefile force
	$(MAKE) -C submodules/libjwt && $(MAKE) -C submodules/libjwt install

libjwt: lib/libjwt.dylib


