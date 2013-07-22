
SUBDIRS=cni

all:
	@for i in $(SUBDIRS) ; do $(MAKE) -C $$i ; done

install:
	@mkdir -p $(DESTDIR)
	@for i in $(SUBDIRS) ; do $(MAKE) DESTDIR=`cd $(DESTDIR) && pwd` -C $$i install ; done

clean:
	@for i in $(SUBDIRS) ; do $(MAKE) -C $$i clean ; done
