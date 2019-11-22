       PROG   = matmult
    MODULOS   = matriz

           CC = gcc -std=c11 -g
         OBJS = $(addsuffix .o,$(MODULOS))

       LIKWID = /home/soft/likwid
 LIKWID_FLAGS = -I$(LIKWID)/include
  LIKWID_LIBS = -L$(LIKWID)/lib

   AVX_FLAGS = -march=native -mavx -O3 -falign-functions=32 -falign-loops=32 -fstrict-aliasing
   AVX_LOG_FLAGS = -fopt-info-vec -fopt-info-vec-missed 

       CFLAGS = $(LIKWID_FLAGS) 
       LFLAGS = $(LIKWID_LIBS) -lm

.PHONY: all clean limpa purge faxina distclean debug avx likwid

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

all: $(PROG)

debug:         CFLAGS += -DDEBUG

avx:           CFLAGS += $(AVX_FLAGS) $(AVX_LOG_FLAGS)
avx likwid:    CFLAGS += -DLIKWID_PERFMON
avx likwid:    LFLAGS += -llikwid

likwid avx debug: $(PROG)

$(PROG):  $(PROG).o

$(PROG): $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	@echo "Limpando ...."
	@rm -f *~ *.bak *.tmp

purge distclean:   clean
	@echo "Faxina ...."
	@rm -f  $(PROG) *.o core a.out
	@rm -f *.png marker.out *.log
