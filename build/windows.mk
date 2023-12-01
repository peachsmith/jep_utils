#=============================================================================
# Makefile for MSVC
#=============================================================================

CC=cl

# The include and lib directories must exist in the SDL2_HOME directory.
INCLUDE_DIR=/I..\include\jep_utils
LINK_DIR=

SRC=..\src\*.c
TEST_SRC=..\tests\*.c

DEBUG_CFLAGS=/JMC /permissive- /ifcOutput "./" /GS /W3 /Zc:wchar_t /ZI /Gm- /Od /sdl /Fd"vc143.pdb" /Zc:inline /fp:precise /D "_DEBUG" /D "JEP_UTILS_EXPORTS" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /errorReport:prompt /WX- /Zc:forScope /RTC1 /Gd /LDd /FC /EHsc /nologo /Fo"./" /Fp"x64\Debug\jep_utils.pch" /diagnostics:column 
DEBUG_LFLAGS=/OUT:"jep_utils.dll" /MANIFEST /NXCOMPAT /PDB:"jep_utils.pdb" /DYNAMICBASE /IMPLIB:"jep_utils.lib" /DEBUG /DLL /MACHINE:X64 /INCREMENTAL /SUBSYSTEM:WINDOWS /MANIFESTUAC:NO /ManifestFile:"jep_utils.dll.intermediate.manifest" /LTCGOUT:"jep_utils.iobj" /ERRORREPORT:PROMPT /ILK:"jep_utils.ilk" /NOLOGO /TLBID:1 

CFLAGS=/permissive- /ifcOutput "./" /GS /GL /W3 /Gy /Zc:wchar_t /Zi /Gm- /O2 /sdl /Zc:inline /fp:precise /D "NDEBUG" /D "JEP_UTILS_EXPORTS" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /errorReport:prompt /WX- /Zc:forScope /Gd /Oi /LD /FC /EHsc /nologo /Fo"./" /diagnostics:column 
LFLAGS=/OUT:"jep_utils.dll" /LTCG:incremental /NXCOMPAT /DYNAMICBASE /IMPLIB:"jep_utils.lib" /DEBUG /DLL /MACHINE:X64 /OPT:REF /SUBSYSTEM:WINDOWS /OPT:ICF /ERRORREPORT:PROMPT /NOLOGO /TLBID:1

release:
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $(SRC) /link $(LINK_DIR) $(LFLAGS)
	del *.obj
	del jep_utils.iobj
	del jep_utils.ipdb
	del *.pdb
	del jep_utils.exp

clean:
	del *.obj

debug:
	$(CC) $(DEBUG_CFLAGS) $(INCLUDE_DIR) $(SRC) /link $(LINK_DIR) $(DEBUG_LFLAGS)


all: release

dist:
	mkdir msvc\include\jep_utils
	mkdir msvc\lib
	copy jep_utils.dll msvc\lib
	copy jep_utils.lib msvc\lib
	copy ..\include\jep_utils\*.h msvc\include\jep_utils

test:
	$(CC) $(INCLUDE_DIR) /I..\tests $(TEST_SRC) /link "jep_utils.lib" -out:tests.exe
	del *.obj
	del *.pdb
