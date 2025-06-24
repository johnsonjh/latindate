# Makefile
# Copyright (c) 2025 Jeffrey H. Johnson
# SPDX-License-Identifier: MIT-0
# vim: set noexpandtab cc=80 :

################################################################################

SHELL:=/bin/sh

RM?=rm -f

# LINT autodetection requires GNU make
LINT:=$(shell { test -x /opt/oracle/developerstudio12.6/bin/lint && \
                printf '%s\n' "/opt/oracle/developerstudio12.6/bin/lint"; } || \
              { test -x /opt/developerstudio12.6/bin/lint && \
                printf '%s\n' "/opt/developerstudio12.6/bin/lint"; } || \
              { printf '%s\n' "true"; })

################################################################################

.PHONY: all
all: latindate

################################################################################

latindate: latindate.c

################################################################################

.PHONY: clean
clean:
	$(RM) latindate

################################################################################

.PHONY: lint
lint: latindate.c
	@test -z "$(LINT)" && \
	{ printf '%s\n' "LINT unset; skipping lint..."; exit 0; } || \
	{ set -x; env $(LINT) -fd $<; };

################################################################################
