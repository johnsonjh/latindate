# Makefile
# Copyright (c) 2025 Jeffrey H. Johnson
# SPDX-License-Identifier: MIT-0
# vim: set expandtab cc=80 :

################################################################################

RM?=rm -f

# lint target requires GNU make
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
	test -z "$(LINT)" && { exit 0; } || { env $(LINT) $<; };

################################################################################
