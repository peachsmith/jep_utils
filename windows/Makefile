#=============================================================================
# Set ARCH=x64 if building with x64 tools
#=============================================================================

CC=cl
LNK=link

# /D "_DEBUG"
!IF "$(DEBUG)" == "yes"
D_DEBUG=/DEBUG /D "_DEBUG"
D_LEAN=/Od /RTC1 /ZI /MDd /diagnostics:column
D_LNK=/INCREMENTAL /DEBUG:FULL
D_LNK_PDB=/PDB:".\jep_utils.pdb"
D_LNK_TEST_PDB=/PDB:".\test.pdb"
!ELSE
D_LEAN=/O2 /Oi /GL /MT /D "WIN32_LEAN_AND_MEAN"
D_LNK=/LTCG:incremental /OPT:REF /INCREMENTAL:NO /OPT:ICF /DEBUG:FULL
!ENDIF

!IF "$(ARCH)" == "x64"
D_WIN32=
D_ARCH=/MACHINE:X64
!ELSE
D_WIN32=/D "WIN32"
D_ARCH=/MACHINE:X86
!ENDIF

CC_FLAGS=/c $(D_LEAN) /GS /analyze- /W3 /WX- /Oy- /nologo /Zc:inline /Zc:forScope /fp:precise /Zc:wchar_t /I"..\include" /Gm- /D "_CRT_SECURE_NO_WARNINGS" $(D_WIN32) $(D_DEBUG) /D "_WINDOWS" /D "_USRDLL" /D "JEP_UTILS_EXPORTS" /D "_WINDLL" /D "_UNICODE" /D "UNICODE"
LNK_FLAGS=/OUT:".\jep_utils.dll" $(D_LNK) $(D_LNK_PDB) /NXCOMPAT /NOLOGO /DYNAMICBASE "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" /IMPLIB:".\jep_utils.lib" /MANIFEST /DLL $(D_ARCH) /SUBSYSTEM:WINDOWS /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /ManifestFile:".\jep_utils.dll.intermediate.manifest" /TLBID:1

TEST_CC_FLAGS=/c $(D_LEAN) /GS /W3 /WX- /Oy- /nologo /Zc:inline /Zc:forScope /fp:precise /Zc:wchar_t /I"..\include" /Gm- /D "_CRT_SECURE_NO_WARNINGS" $(D_WIN32) $(D_DEBUG) /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_UNICODE" /D "UNICODE"
TEST_LNK_FLAGS=/OUT:".\test.exe" $(D_LNK) $(D_LNK_TEST_PDB) /NXCOMPAT /NOLOGO /DYNAMICBASE "jep_utils.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" /MANIFEST $(D_ARCH) /SUBSYSTEM:CONSOLE /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /ManifestFile:".\test.exe.intermediate.manifest" /TLBID:1

OBJ=bitstring.obj byte_buffer.obj char_buffer.obj character.obj huffman.obj json.obj string.obj unicode.obj
SRC=..\src
TEST_SRC=..\tests

all: jep_utils.dll

jep_utils.dll: $(OBJ)
	$(LNK) $(LNK_FLAGS) $(OBJ)

bitstring.obj:
	$(CC) $(CC_FLAGS) $(SRC)\bitstring.c
	
byte_buffer.obj:
	$(CC) $(CC_FLAGS) $(SRC)\byte_buffer.c
	
char_buffer.obj:
	$(CC) $(CC_FLAGS) $(SRC)\char_buffer.c
	
character.obj:
	$(CC) $(CC_FLAGS) $(SRC)\character.c
	
huffman.obj:
	$(CC) $(CC_FLAGS) $(SRC)\huffman.c
	
json.obj:
	$(CC) $(CC_FLAGS) $(SRC)\json.c
	
string.obj:
	$(CC) $(CC_FLAGS) $(SRC)\string.c
	
unicode.obj:
	$(CC) $(CC_FLAGS) $(SRC)\unicode.c


test: test.exe

test.exe: main.obj
	$(LNK) $(TEST_LNK_FLAGS) main.obj

main.obj:
	$(CC) $(TEST_CC_FLAGS) $(TEST_SRC)\main.c

clean:
	del *.obj *.ilk *.pdb *.idb *.ipdb *.iobj *.exp *.manifest *.lib *.dll *.exe
