CC=vc
CFLAGS+= -c99 -+ -dontwarn=214
LDFLAGS += -lamiga -lauto

SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

EXE=TestSamples


# Debug build settings

DBGDIR = debug
DBGEXE = $(DBGDIR)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJECTS))
DBGCFLAGS = -g -O0 -DDEBUG
# for use with kprintf
#DBGCFLAGS = -g -O0 -DDEBUG -DKDEBUG
#DBGLDFLAGS = devkits:sdk/classic/NDK3.2R4/lib/debug.lib

# Release build settings

RELDIR = release
RELEXE = $(RELDIR)/$(EXE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJECTS))
RELCFLAGS = -O2 -DNDEBUG
RELLDFLAGS = 


.PHONY: all clean debug prep release remake

# Default build
all: prep debug release

#
# Debug rules
#
debug: $(DBGEXE)

$(DBGEXE): $(DBGOBJS)
	$(CC) $(CFLAGS) $(DBGCFLAGS) $(LDFLAGS) $(DBGLDFLAGS) -o $(DBGEXE) $^

$(DBGDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(DBGCFLAGS) -o $@ $<

#
# Release rules
#
release: $(RELEXE)

$(RELEXE): $(RELOBJS)
	$(CC) $(CFLAGS) $(RELCFLAGS) $(LDFLAGS) $(RELLDFLAGS) -o $(RELEXE) $^

$(RELDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(RELCFLAGS) -o $@ $<


# Other rules

prep:
	@-makedir $(DBGDIR) $(RELDIR) ALL

remake: clean all

clean:
	@-delete  $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS) ALL QUIET
