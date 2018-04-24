CC = gcc
CFLAGS = -O2 -Wall
LDFLAGS = -lm

PROGRAM = saw_wave saw_wave_sin saw_wave_cos \
	rect_wave rect_wave_sin rect_wave_cos \
	triangle_wave triangle_wave_sin triangle_wave_cos \
	white_noise white_noise_sin white_noise_envelop \
	mario_bgm coin jump

OBJS = util.o

.SUFFIXES: .c .o

all: $(PROGRAM)

saw_wave: $(OBJS) saw_wave.c
	$(CC) $(CFLAGS) -o $@ $^

saw_wave_sin: $(OBJS) saw_wave_sin.c
	$(CC) $(CFLAGS) -o $@ $^

saw_wave_cos: $(OBJS) saw_wave_cos.c
	$(CC) $(CFLAGS) -o $@ $^

rect_wave: $(OBJS) rect_wave.c
	$(CC) $(CFLAGS) -o $@ $^

rect_wave_sin: $(OBJS) rect_wave_sin.c
	$(CC) $(CFLAGS) -o $@ $^

rect_wave_cos: $(OBJS) rect_wave_cos.c
	$(CC) $(CFLAGS) -o $@ $^

triangle_wave: $(OBJS) triangle_wave.c
	$(CC) $(CFLAGS) -o $@ $^

triangle_wave_sin: $(OBJS) triangle_wave_sin.c
	$(CC) $(CFLAGS) -o $@ $^

triangle_wave_cos: $(OBJS) triangle_wave_cos.c
	$(CC) $(CFLAGS) -o $@ $^

white_noise: $(OBJS) white_noise.c
	$(CC) $(CFLAGS) -o $@ $^

white_noise_sin: $(OBJS) white_noise_sin.c
	$(CC) $(CFLAGS) -o $@ $^

white_noise_envelop: $(OBJS) white_noise_envelop.c
	$(CC) $(CFLAGS) -o $@ $^

mario_bgm: $(OBJS) mario_bgm.c
	$(CC) $(CFLAGS) -o $@ $^

coin: $(OBJS) coin.c
	$(CC) $(CFLAGS) -o $@ $^

jump: $(OBJS) jump.c
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) -c -MMD -MP $<

.PHONY: clean
clean:
	rm -f *.o *.d *.wav $(PROGRAM)

clean_wav:
	rm -f *.wav
