# Makefile
# Copyright (c) 2025 Jeffrey H. Johnson
# SPDX-License-Identifier: MIT-0
# vim: set noexpandtab cc=80 :

################################################################################
# Configuration

SHELL:=/bin/sh
RM?=rm -f

################################################################################
# Tool autodetection (requires GNU Make)

REUSE:=$(shell command -v reuse > /dev/null 2>&1 && \
               { printf '%s\n' "reuse lint -q || reuse lint"; } || \
               { printf '%s\n' "true"; })

LINT:=$(shell { test -x /opt/oracle/developerstudio12.6/bin/lint && \
                printf '%s\n' "/opt/oracle/developerstudio12.6/bin/lint"; } || \
              { test -x /opt/developerstudio12.6/bin/lint && \
                printf '%s\n' "/opt/developerstudio12.6/bin/lint"; } || \
              { printf '%s\n' "true"; })

CPPCHECK:=$(shell command -v cppcheck > /dev/null 2>&1 && \
                  { printf '%s\n' \
                    "cppcheck --force --check-level=exhaustive *.[ch]"; } || \
                  { printf '%s\n' "true"; })

SCANBUILD:=$(shell command -v scan-build > /dev/null 2>&1 && \
                  { printf '%s\n' \
                    "$(MAKE) clean && scan-build $(MAKE)"; } || \
                  { printf '%s\n' "true"; })

################################################################################
# Target: all

.PHONY: all
all: latindate

################################################################################
# Target: latindate

latindate: latindate.c

################################################################################
# Target :clean

.PHONY: clean
clean:
	$(RM) latindate

################################################################################
# Target: lint

.PHONY: lint
lint: latindate.c
	$(MAKE) clean
	@printf '%s\n' ""
	@test -z "$(REUSE)" && \
	 { printf '%s\n' "REUSE unset; skipping reuse..."; exit 0; } || \
	 { set -x; env $(REUSE); };
	@printf '%s\n' ""
	@test -z "$(CPPCHECK)" && \
	 { printf '%s\n' "CPPCHECK unset; skipping cppcheck..."; exit 0; } || \
	 { set -x; env $(CPPCHECK); };
	@printf '%s\n' ""
	@test -z "$(SCANBUILD)" && \
	 { printf '%s\n' "SCANBUILD unset; skipping analyzer ..."; exit 0; } || \
	 { set -x; env $(SCANBUILD); };
	@printf '%s\n' ""
	@test -z "$(LINT)" && \
	 { printf '%s\n' "LINT unset; skipping lint..."; exit 0; } || \
	 { set -x; env $(LINT) -fd $<; };
	@printf '%s\n' ""
	$(MAKE) clean

################################################################################
