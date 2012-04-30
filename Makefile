

CWD := $(shell pwd)

TESTAPP = APITestforUSBHID_SHT

TESTAPP_OBJ =   Rel_SHT/TestAP/APITest.o \
				Rel_SHT/TestAP/Connect.o \
				Rel_SHT/TestAP/Disconnect.o \
				Rel_SHT/TestAP/Command.o \
				Rel_SHT/TestAP/DLE_EOT.o \
				Rel_SHT/TestAP/Download.o \
				Rel_SHT/TestAP/USB_HID_UserInterface_Proc_SHT.o

STATIC_LIB = Rel_SHT/All_Lib/libPrtcl_USBHID_SHT.a

LIB_OBJS =  $(LIBPRTCL_OBJS) \
			$(LIBUSB_HID_INTERFACE_OBJS) \
			$(LIBCOLLECTLOGEX_OBJS)

LIBPRTCL = libPrtcl_USB_HID_SHT.so.1.0
LIBPRTCL_OBJS = Rel_SHT/All_Lib/Prtcl_USB_HID_SHT.o \
				Rel_SHT/All_Lib/Prtcl_USB_HID_FuncDef_SHT.o \
				Rel_SHT/All_Lib/ProperDataDef.o 

LIBUSB_HID_INTERFACE = libUSB_HID_Interface.so.1.0
LIBUSB_HID_INTERFACE_OBJS = Rel_SHT/All_Lib/USB_HID_Interface.o

LIBCOLLECTLOGEX = libCollectLogEx.so.1.0
LIBCOLLECTLOGEX_OBJS =  Rel_SHT/All_Lib/CollectLogEx.o \
						Rel_SHT/All_Lib/FileProc.o


#all: $(STATIC_LIB) $(LIBPRTCL) $(LIBUSB_HID_INTERFACE) $(LIBCOLLECTLOGEX)
all: $(TESTAPP)


$(LIB_OBJS): Rel_SHT.tar.gz
	bsdtar xzf Rel_SHT.tar.gz
	sed -i 's|USER_LIB_PATH = /usr/lib|USER_LIB_PATH = $(CWD)/Rel_SHT/All_Lib|' \
		Rel_SHT/All_Lib/Makefile
	sed -i 's|/sbin/ldconfig|#/sbin/ldconfig|' \
		Rel_SHT/All_Lib/Makefile
	sed -i 's|cp|#cp|' \
		Rel_SHT/All_Lib/Makefile


$(TESTAPP): $(STATIC_LIB)
	$(CXX) $(TESTAPP_OBJ) -L$(CWD)/Rel_SHT/All_Lib -o $(TESTAPP) $(STATIC_LIB) \
		-lpthread -lusb


$(STATIC_LIB): $(LIB_OBJS)
	make -C Rel_SHT/All_Lib


$(LIBPRTCL): $(LIBPRTCL_OBJS)
	$(CXX) -shared -W1,-soname,$(LIBPRTCL) -o $(LIBPRTCL) $(LIBPRTCL_OBJS)
	ln -s $(LIBPRTCL) $(LIBPRTCL:.1.0=)

$(LIBUSB_HID_INTERFACE): $(LIBUSB_HID_INTERFACE_OBJS)
	$(CXX) -shared -W1,-soname,$(LIBUSB_HID_INTERFACE) -o $(LIBUSB_HID_INTERFACE) $(LIBUSB_HID_INTERFACE_OBJS)
	ln -s $(LIBUSB_HID_INTERFACE) $(LIBUSB_HID_INTERFACE:.1.0=)

$(LIBCOLLECTLOGEX): $(LIBCOLLECTLOGEX_OBJS)
	$(CXX) -shared -W1,-soname,$(LIBCOLLECTLOGEX) -o $(LIBCOLLECTLOGEX) $(LIBCOLLECTLOGEX_OBJS)
	ln -s $(LIBCOLLECTLOGEX) $(LIBCOLLECTLOGEX:.1.0=)



clean: 
	rm -f -- $(LIBPRTCL) $(LIBPRTCL:.1.0=)
	rm -f -- $(LIBUSB_HID_INTERFACE) $(LIBUSB_HID_INTERFACE:.1.0=)
	rm -f -- $(LIBCOLLECTLOGEX) $(LIBCOLLECTLOGEX:.1.0=)
	rm -f -- $(STATIC_LIB)
	rm -rf -- Rel_SHT/

.PHONY: clean
