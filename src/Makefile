TOPLEVEL=..

include Make.inc

PROGRAMS=xfs-cli xfs-rcopy xfs-fuse
PROGRAMS:=$(addprefix $(BINS)/, $(PROGRAMS))

all: $(PROGRAMS)

$(LIBS):
	mkdir -p $(LIBS)

$(BINS):
	mkdir -p $(BINS)

$(OBJECTS):
	mkdir -p $(OBJECTS)

$(LIBS)/libxfs.a: $(LIBS)
	$(MAKE) -C xfsprogs

$(OBJECTS)/xfsutil.o: $(OBJECTS)
	$(MAKE) -C xfsutil

$(BINS)/xfs-cli: $(LIBS)/libxfs.a $(OBJECTS)/xfsutil.o $(BINS)
	$(MAKE) -C cli xfs-cli

$(BINS)/xfs-rcopy: $(LIBS)/libxfs.a $(OBJECTS)/xfsutil.o $(BINS)
	$(MAKE) -C cli xfs-rcopy

$(BINS)/xfs-fuse: $(LIBS)/libxfs.a $(OBJECTS)/xfsutil.o $(BINS)
	$(MAKE) -C fuse

clean:
	rm -rf $(BINS)/*
	rm -rf $(LIBS)/*
	rm -rf $(OBJECTS)/*
	$(MAKE) -C cli clean
	$(MAKE) -C fuse clean
	$(MAKE) -C xfsutil clean
	$(MAKE) -C xfsprogs clean
