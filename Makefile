SRCS       = tftt.c network.c
CFLAGS     = -g -Wall 
LDADD      = -lgc
MAKEDEPEND = @echo "  DEP " $<; gcc -M $(CPPFLAGS) -o $(df).d $<
LDC        = @echo "  LD  " $@; gcc $(LDFLAGS) 
CCC        = @echo "  CC  " $@; gcc $(CFLAGS)
DEPDIR     = .deps

.PHONY: dep-init all clean

all: dep-init tftt
clean:
	rm -f tftt
	rm -f *.o
	rm -f *.P

tftt: $(SRCS:.c=.o)
	$(LDC) -o tftt $(SRCS:.c=.o) $(LDADD)

df = $(DEPDIR)/$(*F)

%.o : %.c
	@if [ ! -d $(DEPDIR) ]; then mkdir $(DEPDIR); fi;
	$(MAKEDEPEND); \
	cp $(df).d $(df).P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	    -e '/^$$/ d' -e 's/$$/ :/' < $(df).d >> $(df).P; \
	rm -f $(df).d
	$(CCC) -o $@ -c $<

-include $(SRCS:%.c=$(DEPDIR)/%.P)
