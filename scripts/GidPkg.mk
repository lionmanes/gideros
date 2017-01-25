.PHONY: build.mac.pkg fetch.mac.pkg push.mac.pkg sync.mac.pkg clean.pkg build.all.pkg all full

fetchdoc:
	-wget --quiet --recursive --no-clobber --page-requisites --html-extension --convert-links --restrict-file-names=windows --domains docs.giderosmobile.com --no-parent http://docs.giderosmobile.com/
	rm -rf $(RELEASE)/Documentation
	cp -R docs.giderosmobile.com $(RELEASE)/Documentation
	-wget "http://docs.giderosmobile.com/reference/autocomplete.php" -O $(RELEASE)/Resources/gideros_annot.api
	 
build.mac.pkg:
	echo "\
	cd $(MAC_PATH);\
	make -f scripts/Makefile.gid;\
	exit;\
	" |	ssh $(MAC_HOST)
	
fetch.mac.pkg:
	echo "\
	cd $(MAC_PATH)/Build.Mac;\
	rm -f BuildMac.zip;\
	zip -r BuildMac.zip Sdk Players Templates All\\ Plugins;\
	exit;\
	" |	ssh $(MAC_HOST)
	scp -B $(MAC_HOST):$(MAC_PATH)/Build.Mac/BuildMac.zip $(RELEASE)/BuildMac.zip

push.mac.pkg:
	cd $(RELEASE);\
	rm -f BuildWin.zip;\
	zip -r BuildWin.zip Sdk Players Templates All\\ Plugins Resources Documentation;\
	scp -B BuildWin.zip $(MAC_HOST):$(MAC_PATH)/Build.Mac/BuildWin.zip 

bundle.mac.pkg:
	echo "\
	cd $(MAC_PATH);\
	make -f scripts/Makefile.gid bundle.installer;\
	exit;\
	" |	ssh $(MAC_HOST)

sync.mac.pkg: fetch.mac.pkg push.mac.pkg

clean.pkg: clean
	echo "\
	cd $(MAC_PATH);\
	git pull;\
	make -f scripts/Makefile.gid clean;\
	exit;\
	" |	ssh $(MAC_HOST)

%.subthr:
	$(MAKE) -j1 -f scripts/Makefile.gid $*
	
build.all.thrun : all.subthr build.mac.pkg.subthr fetchdoc

build.all.thr:
	$(MAKE) -j3 -f scripts/Makefile.gid build.all.thrun

bundle.all.thrun : bundle.installer.subthr bundle.mac.pkg.subthr fetchdoc

bundle.all.thr:
	$(MAKE) -j2 -f scripts/Makefile.gid bundle.all.thrun
	
all.pkg: start.pkg build.all.thr fetch.mac.pkg push.mac.pkg bundle.all.thr
	echo -n "Finished on "; date

start.pkg:
	echo -n "Starting on "; date

full: clean.pkg all.pkg
