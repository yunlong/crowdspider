
# top level makefile
MKDIR    = mkdir -p
INSTALL = /usr/bin/install -c 
LINK = ln -s
INSTALL_DIR = /usr/local/crowdspider

all: 
	for dir in libhtmlparser libspider build ; do \
		cd $$dir && make && cd ..; \
	done

clean:
	for dir in libhtmlparser libspider build; do \
		cd $$dir && make clean && cd ..; \
	done

install:  
	$(MKDIR) $(INSTALL_DIR)/bin
	$(MKDIR) $(INSTALL_DIR)/config
	$(MKDIR) $(INSTALL_DIR)/lib
	$(INSTALL) config/* $(INSTALL_DIR)/config
	$(INSTALL) libspider/lib* $(INSTALL_DIR)/lib
	$(INSTALL) libhtmlparser/lib* $(INSTALL_DIR)/lib


