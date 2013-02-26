!include <win32.mak>

!if "$(TARGET_CPU)" == "x64"
BITNESS                         = 64
!else if "$(TARGET_CPU)" == "x86"
BITNESS                         = 32
!else
!error Unsupported processor.
!endif

EASYHOOK_ROOT                   =   EasyHook
EASYHOOK_LIB                    =   $(EASYHOOK_ROOT)\EasyHook$(BITNESS).lib
EASYHOOK_DLL                    =   $(EASYHOOK_ROOT)\EasyHook$(BITNESS).dll


FAULTRON_EXE_SRC                =   faultron.cpp
FAULTRON_EXE_OBJ                =   $(OUTDIR)\faultron.obj
FAULTRON_EXE_LIBS               =   $(EASYHOOK_LIB) kernel32.lib
#FAULTRON_EXE_RC                 =   faultron.rc

FAULTRON_DLL_SRC                =   faultrondll.cpp
FAULTRON_DLL_OBJ                =   $(OUTDIR)\faultrondll.obj
FAULTRON_DLL_LIBS               =   $(EASYHOOK_LIB) kernel32.lib user32.lib

all: $(OUTDIR) $(OUTDIR)\faultron$(BITNESS).dll $(OUTDIR)\faultron$(BITNESS).exe

$(OUTDIR):
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)
    copy $(EASYHOOK_DLL) $(OUTDIR)

.cpp{$(OUTDIR)}.obj:
  $(cc) $(cdebug) $(cflags) $(cvars) /WX /EHsc /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $**
    
$(OUTDIR)\faultron$(BITNESS).dll: $(FAULTRON_DLL_OBJ)
    $(link) $(linkdebug) $(dlllflags) -out:$(OUTDIR)\faultron$(BITNESS).dll $(FAULTRON_DLL_OBJ) $(FAULTRON_DLL_LIBS)


$(OUTDIR)\faultron$(BITNESS).exe: $(OUTDIR)\faultron$(BITNESS).dll $(FAULTRON_EXE_OBJ)
    $(link) $(conflags) -out:$(OUTDIR)\faultron$(BITNESS).exe $(FAULTRON_EXE_OBJ) $(FAULTRON_EXE_LIBS)

clean:
    $(CLEANUP)
