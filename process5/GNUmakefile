BASEPROGRAMS = timedwait-poll timedwait-block timedwait-blockvar \
	timedwait-alarm timedwait-selfpipe timedwait-selfpipe2 \
	timedwait-signalfd
PROGRAMS = $(BASEPROGRAMS)
all: $(PROGRAMS) $(DATAFILE)

variants = $(1) $(addsuffix -unsafe,$(1)) $(addsuffix -O0,$(1))

-include config.mk
ifndef DATAFILE
DATAFILE = data
endif

O ?= 3
COMPILER ?= gcc
include ../common/rules.mk
ifneq ($(DATAFILE),data)
CPPFLAGS += -DDATAFILE='"$(DATAFILE)"'
endif

UNSAFEFLAGS := -U_FORTIFY_SOURCE -fno-stack-protector $(if $(ISLINUX),-no-pie,)

%.o: %.cc $(BUILDSTAMP)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(O) $(DEPCFLAGS) -o $@ -c $<

%-unsafe.o: %.cc $(BUILDSTAMP)
	$(CXX) $(CPPFLAGS) $(UNSAFEFLAGS) $(CXXFLAGS) -O1 $(DEPCFLAGS) -o $@ -c $<

%-O0.o: %.cc $(BUILDSTAMP)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -O0 $(DEPCFLAGS) -o $@ -c $<

%-O1.o: %.cc $(BUILDSTAMP)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -O1 $(DEPCFLAGS) -o $@ -c $<


$(BASEPROGRAMS): %: %.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(O) -o $@ $^

$(addsuffix -O0,$(BASEPROGRAMS)): %: %.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -O0 -o $@ $^

$(addsuffix -unsafe,$(BASEPROGRAMS)): %: %.o hexdump.o
	$(CXX) $(CXXFLAGS) $(UNSAFEFLAGS) $(LDFLAGS) -O1 -o $@ $^


DATASIZE := $(shell wc -c $(DATAFILE) 2>/dev/null | awk '{ print $$1 }')
DATADEP := $(shell if test "$(DATASIZE)" -lt 51200000 2>/dev/null; then echo always; fi)

$(DATAFILE): $(DATADEP)
	yes 77777777777777777777777777777777777777777 | tr -d '\n' | head -c 51200000 > $@

ifneq ($(DATAFILE),data)
data: $(DATAFILE)
.PHONY: data
endif


clean:
	rm -f $(call variants,$(BASEPROGRAMS)) *.o data
	rm -rf $(DEPSDIR) *.dSYM

.PHONY: all clean
.PRECIOUS: %-O0.o %-O1.o %-unsafe.o
