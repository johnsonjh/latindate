RM?=rm -f

.PHONY: all
all: latindate

latindate: latindate.c

.PHONY: clean
clean:
	$(RM) latindate
