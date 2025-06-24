# Makefile
# Copyright (c) 2025 Jeffrey H. Johnson
# SPDX-License-Identifier: MIT-0

RM?=rm -f

.PHONY: all
all: latindate

latindate: latindate.c

.PHONY: clean
clean:
	$(RM) latindate
