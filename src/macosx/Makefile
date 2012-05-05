TOPLEVEL=../..

include ../Make.inc

all: $(BUILD)/fuse-xfs-$(VERSION).dmg

PROGRAMS=xfs-cli xfs-rcopy xfs-fuse mkfs.xfs
PROGRAMS:=$(addprefix $(BINS)/, $(PROGRAMS))

PACKAGING=Description.plist Info.plist.in Install_resources add_background.osa fuse-xfs.fs make-pkg.sh

$(BUILD)/fuse-xfs-$(VERSION).dmg: $(PROGRAMS) $(PACKAGING)
	sh make-pkg.sh $(VERSION) $(TOPLEVEL) $(BUILD) .

clean:

