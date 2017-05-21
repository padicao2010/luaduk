LIBFLAG= -shared -fpic -llua

lduk.so:	lduk.c
	$(CC) -o $@ $(LIBFLAG) $(CFLAGS) lduk.c duktape.c

clean:
	$(RM) lduk.so
