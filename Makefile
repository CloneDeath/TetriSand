#
# A Makefile that compiles all .c and .s files in "src" and "res"
# subdirectories and places the output in a "obj" subdirectory
#

# If you move this project you can change the directory
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
GBDK_HOME = /home/nicholas/Applications/gbdk/
#EMULATOR = mgba-qt
EMULATOR = /home/nicholas/Applications/Emulicious/Emulicious.jar

LCC = $(GBDK_HOME)bin/lcc

# You can set flags for LCC here
# https://gbdk-2020.github.io/gbdk-2020/docs/api/docs_toolchain_settings.html
# -Wm-yc            GameBoy Color compatible
# -Wm-yC            GameBoy Color only
LCCFLAGS = -Wm-yc

LCCFLAGS += -Wm-yn"TetriSand" # -yn name cartridge name

# https://gbdk-2020.github.io/gbdk-2020/docs/api/docs_rombanking_mbcs.html#autotoc_md91
LCCFLAGS += -Wm-yoA # -Wm-yo<N> where <N> is the number of ROM banks. 2, 4, 8, 16, 32, 64, 128, 256, 512
LCCFLAGS += -Wm-ya1 # -Wm-ya<N> where <N> is the number of RAM banks. 2, 4, 8, 16, 32
LCCFLAGS += -Wm-yt0x02 # -Wm-yt<N> where <N> is the type of MBC cartridge (0x02: ROM+MBC1+RAM)

# https://gbdk-2020.github.io/gbdk-2020/docs/api/docs_rombanking_mbcs.html#autotoc_md103
LCCFLAGS += -autobank -Wb-ext=.rel

# -Wf--debug : Enable Debug
# -Wl-y : Enable CDB file generation for debugging
# -Wl-w -Wl-m : Enable "wide maps" for Emulicious (to get variable names)
LCCFLAGS += -Wf--debug -Wl-y -Wl-w -Wl-m

LCCFLAGS += -DSKIP_FIND_FAMILIES
#LCCFLAGS += -DSKIP_SLIDE_SAND

# You can set the name of the .gb ROM file here
PROJECTNAME    = TetriSand

SRCDIR      = src
OBJDIR      = obj
RESDIR      = res
TESTDIR     = test
BINS	    = $(OBJDIR)/$(PROJECTNAME).gb
TESTS		= $(OBJDIR)/test.gb
CSOURCES    = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.c)))
ASMSOURCES  = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.s)))
TSOURCES    = $(foreach dir,$(TESTDIR),$(notdir $(wildcard $(dir)/*.c)))
OBJS        = $(CSOURCES:%.c=$(OBJDIR)/%.o) $(ASMSOURCES:%.s=$(OBJDIR)/%.o)
TOBJS       = $(TSOURCES:%.c=$(OBJDIR)/%.o)

all: prepare $(BINS) $(TESTS)

prepare:
	mkdir -p $(OBJDIR)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed y/\\//\\\\/ | grep -v make >> compile.bat

# Compile .c files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Compile .c files in "res/" to .o object files
$(OBJDIR)/%.o:	$(RESDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Compile .c files in "test/" to .o object files
$(OBJDIR)/%.o:	$(TESTDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Compile .s assembly files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.s
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# If needed, compile .c files in "src/" to .s assembly files
# (not required if .c is compiled directly to .o)
$(OBJDIR)/%.s:	$(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -S -o $@ $<

# Link the compiled object files into a .gb ROM file
$(BINS): $(OBJS)
	$(LCC) $(LCCFLAGS) -o $(BINS) $(OBJS)

# Link the compiled test and src object files into a test.gb ROM file
$(TESTS): $(TOBJS) $(OBJS)
	$(LCC) $(LCCFLAGS) -o $(TESTS) $(TOBJS) $(filter-out $(OBJDIR)/main.o, $(OBJS))

clean:
	rm -f  $(OBJDIR)/*.*

run: all
	$(EMULATOR) $(OBJDIR)/$(PROJECTNAME).gb

test: all
	$(EMULATOR) $(OBJDIR)/test.gb
