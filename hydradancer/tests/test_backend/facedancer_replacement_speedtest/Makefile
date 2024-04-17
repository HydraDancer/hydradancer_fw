
# following warnings are inspired from https://github.com/RfidResearchGroup/proxmark3/blob/master/Makefile.defs

# basic warnings
WARNINGS = -Werror -Wno-comment -pedantic -Wall -Wno-error=unused-parameter
# some more warnings
WARNINGS += -Wbad-function-cast -Wredundant-decls -Wmissing-prototypes -Wchar-subscripts -Wshadow -Wundef -Wwrite-strings -Wunused -Wuninitialized -Wpointer-arith -Winline -Wformat -Wformat-security -Winit-self -Wmissing-include-dirs -Wnested-externs -Wmissing-declarations -Wempty-body -Wignored-qualifiers -Wmissing-field-initializers -Wtype-limits -Wcast-align -Wswitch-enum
# additionnal warnins only in recent gcc versions, warnings after -Wextra are a listing of known warnings in the latter
WARNINGS += -Wextra -Wclobbered -Wcast-function-type -Wimplicit-fallthrough=3 -Wmissing-parameter-type -Wold-style-declaration -Woverride-init -Wshift-negative-value -Wunused-but-set-parameter

# use make EXTRACFLAGS=1 to activate the following warnings
ifeq ($(EXTRACFLAGS), 1)
	WARNINGS += -Wunused-parameter -Wno-error=unused-parameter
	WARNINGS += -Wsign-compare -Wno-error=sign-compare
	WARNINGS += -Wconversion -Wno-error=conversion -Wno-error=sign-conversion -Wno-error=float-conversion
endif

all:
	gcc -O3 -g $(WARNINGS) -o facedancer_replacement_speedtest main.c -lusb-1.0