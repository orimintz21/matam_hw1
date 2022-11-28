CC = gcc
EXE_MATAMIKYA = matamikya
EXE_AMOUNT_SET_STR = amount_set_str
OBJS_MATAMIKYA = matamikya_order.o matamikya_product.o matamikya.o matamikya_print.o matamikya_tests.o matamikya_main.o matamikya_items.o
OBJS_AMOUNT_SET_STR = amount_set_str.o amount_set_str_main.o amount_set_str_tests.o
DEBUG_FLAG = -g
CFLAGS = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG $(DEBUG_FLAG)
LIB = -L. -lmtm -lm -las



$(EXE_MATAMIKYA): $(OBJS_MATAMIKYA) 
	$(CC) $(CFLAGS) $(OBJS_MATAMIKYA) $(LIB) -o $@
matamikya_order.o: matamikya_order.c set.h amount_set.h matamikya.h matamikya_items.h
	$(CC) -c $(CFLAGS) matamikya_order.c
matamikya_product.o: matamikya_product.c amount_set.h matamikya.h matamikya_product.h set.h matamikya_items.h
	$(CC) -c $(CFLAGS) matamikya_product.c
matamikya.o: matamikya.c matamikya_print.h set.h amount_set.h matamikya.h matamikya_order.h matamikya_product.h matamikya_items.h
	$(CC) -c $(CFLAGS) matamikya.c
matamikya_items.o: matamikya_items.c matamikya_items.h amount_set.h matamikya_order.h matamikya_product.h
	$(CC) -c $(CFLAGS) matamikya_items.c
matamikya_print.o: matamikya_print.c matamikya_print.h
	$(CC) -c $(CFLAGS) matamikya_print.c
matamikya_tests.o: tests/matamikya_tests.c tests/matamikya_tests.h matamikya.h
	$(CC) -c $(CFLAGS) tests/matamikya_tests.c
matamikya_main.o: tests/matamikya_main.c tests/matamikya_tests.h tests/test_utilities.h
	$(CC) -c $(CFLAGS) tests/matamikya_main.c


$(EXE_AMOUNT_SET_STR): $(OBJS_AMOUNT_SET_STR) 
	$(CC) $(CFLAGS) $(OBJS_AMOUNT_SET_STR)  -o $@

amount_set_str.o: amount_set_str.c amount_set_str.h
	$(CC) -c $(CFLAGS) amount_set_str.c
amount_set_str_main.o: amount_set_str_main.c amount_set_str.h amount_set_str_tests.h amount_set_str_tests.c
	$(CC) -c $(CFLAGS) amount_set_str_main.c
amount_set_str_tests.o: amount_set_str_tests.c amount_set_str.h
	$(CC) -c $(CFLAGS) amount_set_str_tests.c



clean:
	rm -f $(EXE_MATAMIKYA) $(OBJS_MATAMIKYA) $(EXE_AMOUNT_SET_STR) $(OBJS_AMOUNT_SET_STR) 
