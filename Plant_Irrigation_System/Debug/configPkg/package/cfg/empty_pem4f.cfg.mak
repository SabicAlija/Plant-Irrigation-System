# invoke SourceDir generated makefile for empty.pem4f
empty.pem4f: .libraries,empty.pem4f
.libraries,empty.pem4f: package/cfg/empty_pem4f.xdl
	$(MAKE) -f C:\ti\Workspace\Plant_Irrigation_System\Irrigation_System/src/makefile.libs

clean::
	$(MAKE) -f C:\ti\Workspace\Plant_Irrigation_System\Irrigation_System/src/makefile.libs clean

