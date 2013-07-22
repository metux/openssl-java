
include ../siteconfig.mk

PREFIX?=/usr
SBINDIR?=$(PREFIX)/sbin
LIBDIR?=$(PREFIX)/lib
INCLUDEDIR=$(PREFIX)/include

BUILDDIR=tmp

LIBRARY_NAME=openssl-cni
LIBRARY_VERSION=1
LIBRARY_MINOR=0

STATIC_LIB=lib$(LIBRARY_NAME).a
SHARED_LIB=lib$(LIBRARY_NAME).so.$(LIBRARY_VERSION).$(LIBRARY_MINOR)

JAR_LIB=$(BUILDDIR)/openssl-cni.jar

CNI_JAVA=java/de/metux/openssl/OpenSSL.java
CNI_OBJECT=$(BUILDDIR)/OpenSSL.o
CNI_HEADER=$(BUILDDIR)/de_metux_openssl_OpenSSL.h
CNI_SRC=cpp/OpenSSL.cpp
CNI_CLASS=de.metux.openssl.OpenSSL
CNI_LIBS=$(OPENSSL_LIBS)
CNI_CFLAGS=$(OPENSSL_CFLAGS)

PKGCONFIG_FILE=openssl-cni.pc

JAVA_SRC=`find java -name "*.java"`

compile:	jar static shared pkgconfig

jar:		$(JAR_LIB)

static:		$(STATIC_LIB)

shared:		$(SHARED_LIB)

pkgconfig:	$(PKGCONFIG_FILE)

$(PKGCONFIG_FILE):	$(PKGCONFIG_FILE).in
	@cat $< \
		| sed -e 's~@PREFIX@~$(PREFIX)~'			\
		| sed -e 's~@LIBDIR@~$(LIBDIR)~'			\
		| sed -e 's~@EXEC_PREFIX@~$(EXEC_PREFIX)~'		\
		| sed -e 's~@INCLUDEDIR@~$(INCLUDDIR)~'			\
		| sed -e 's~@LIBRARY_NAME@~$(LIBRARY_NAME)~'		\
		| sed -e 's~@LIBRARY_VERSION@~$(LIBRARY_VERSION)~'	\
		| sed -e 's~@LIBRARY_MINOR@~$(LIBRARY_MINOR)~'		\
		| sed -e 's~@OPENSSL_LIBS@~$(OPENSSL_LIBS)~'		\
		| sed -e 's~@OPENSSL_CFLAGS@~$(OPENSSL_CFLAGS)~'	\
		> $@

$(JAR_LIB):
	@mkdir -p `dirname "$(JAR_LIB)"`
	@$(CJ) $(CJFLAGS) -C `find java -name "*.java"` -d classes
	@$(JAR) cf $(JAR_LIB) -C classes .

$(STATIC_LIB):	$(CNI_OBJECT)
	@$(AR) rcs $(STATIC_LIB) $(CNI_OBJECT)

$(SHARED_LIB):	$(CNI_OBJECT)
	@$(CC) -shared -Wl,-soname,lib$(LIBRARY_NAME).so.$(LIBRARY_VERSION) -o $(SHARED_LIB) $(CNI_OBJECT)

$(CNI_HEADER):	$(CNI_JAVA)
	@$(CJ) -C $(CNI_JAVA) -d $(BUILDDIR)/cni
	@$(CJH) -classpath $(BUILDDIR)/cni $(CNI_CLASS) -o $(CNI_HEADER)

$(CNI_OBJECT): $(CNI_HEADER)
	@$(CXX) -fpic -c $(CNI_SRC) -I$(BUILDDIR) -o $(CNI_OBJECT)

clean:
	@rm -Rf tmp classes $(CNI_OBJECT) $(STATIC_LIB) $(SHARED_LIB) $(PKGCONFIG_FILE)

policy:
	@for i in `find -name "*.java"` ; do \
		astyle --style=java --indent=tab --suffix=none --indent-switches < "$$i" > "$$i.tmp" 2>&1 | grep -ve "^unchanged" ; \
		mv "$$i.tmp" "$$i" ; \
	done

doc:
	@javadoc -d javadoc `find java -name "*.java"`

install-staticlib:	$(STATIC_LIB)
	@mkdir -p $(DESTDIR)/$(LIBDIR)
	@cp --preserve $(STATIC_LIB) $(DESTDIR)/$(LIBDIR)

install-sharedlib:	$(SHARED_LIB)
	@mkdir -p $(DESTDIR)/$(LIBDIR)
	@cp $(SHARED_LIB) $(DESTDIR)/$(LIBDIR)
	@cd $(DESTDIR)/$(LIBDIR) && ln -sf lib$(LIBRARY_NAME).so.$(LIBRARY_VERSION).$(LIBRARY_MINOR) lib$(LIBRARY_NAME).so.$(LIBRARY_VERSION)

install-include:	$(CNI_HEADER)
	@mkdir -p $(DESTDIR)/$(INCLUDEDIR)/openssl-java
	@cp --preserve $(CNI_HEADER) $(DESTDIR)/$(INCLUDEDIR)/openssl-java

install-pkgconfig:	$(PKGCONFIG_FILE)
	@mkdir -p $(DESTDIR)/$(PKGCONFIGDIR)
	@cp --preserve $(PKGCONFIG_FILE) $(DESTDIR)/$(PKGCONFIGDIR)

install-jar:		$(JAR_LIB)
	@mkdir -p $(DESTDIR)/$(LIBDIR)/java
	@cp --preserve $(JAR_LIB) $(DESTDIR)/$(LIBDIR)/java

install: install-staticlib install-sharedlib install-include install-pkgconfig install-jar
