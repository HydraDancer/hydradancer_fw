#!/usr/bin/env bash
# Based on https://github.com/RfidResearchGroup/proxmark3
# GNU General Public License v3.0

if [ "$1" = "--help" ] || [ "$1" = "-h" ] || [ "$1" = "" ]; then
    echo "To report a short string about the current version:"
    echo "   $0 --short"
    exit 0
fi

# Clear environment locale so that git will not use localized strings
export LC_ALL="C"
export LANG="C"

SHORT=false
if [ "$1" = "--short" ]; then
    SHORT=true
    shift
fi
FORCE=false
if [ "$1" = "--force" ]; then
    FORCE=true
    shift
fi
UNDECIDED=false
if [ "$1" = "--undecided" ]; then
    UNDECIDED=true
    shift
fi
VERSIONSRC="$1"

if ! $SHORT && [ "$VERSIONSRC" = "" ]; then
    echo "Error: $0 is missing its destination filename"
    exit 1
fi

if $SHORT && [ "$VERSIONSRC" != "" ]; then
    echo "Error: can't output a short string and generate file at the same time"
    exit 1
fi

# if you are making your own fork,  change this line to reflect your fork-name
fullgitinfo=""

if [ ! "$fullgitinfo" = "" ]; then
  fullgitinfo="$fullgitinfo/"
fi

# GIT status  0 = dirty,  1 = clean ,  2 = undecided
clean=2

# Do we have access to git command?
commandGIT=$(env git)

if [ "$commandGIT" != "" ]; then

    # now avoiding the "fatal: No names found, cannot describe anything." error by fallbacking to abbrev hash in such case
    gitversion=$(git describe --tags --always --dirty --long)
    gitbranch=$(git rev-parse --abbrev-ref HEAD)
    if $UNDECIDED; then
        if [ "$gitversion" != "${gitversion%-dirty}" ]; then
            clean=0
        else
            clean=1
        fi
    fi
    if [ "$gitbranch" != "" ] && [ "$gitversion" != "" ]; then
        fullgitinfo="${fullgitinfo}${gitbranch}/${gitversion}"
        # if FORCED_DATE present and properly formatted:
        case "$FORCED_DATE" in
        [0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]" "[0-9][0-9]:[0-9][0-9]:[0-9][0-9])
            ctime="$FORCED_DATE"
            ;;
        *)
            ctime="$(date '+%Y-%m-%d %H:%M:%S')"
            ;;
        esac
    else
        fullgitinfo="${fullgitinfo}main/release (git)"
    fi
else
    fullgitinfo="${fullgitinfo}main/release (no_git)"
    dl_time=$(stat --printf="%y" CMakeLists.txt)
    # POSIX way...
    ctime=${dl_time%.*}
fi
if $SHORT; then
    echo "$fullgitinfo"
    exit 0
fi

sha=$(
    hydradancerpath=$(dirname -- "$0")
    cd "$hydradancerpath" || exit
    # did we find the src?
    [ -f User/main.c ] || exit
    ls User/*.[ch]|sort|xargs sha256sum -t|sha256sum|cut -c -9
)

if [ "$sha" = "" ]; then
  sha="no sha256"
fi

REDO=true

echo $sha
if $REDO; then
    # use a tmp file to avoid concurrent call to mkversion to parse a half-written file.
    cat > "${VERSIONSRC}.tmp" <<EOF
#include <stdint.h>

uint8_t hydradancer_product_string_descriptor[] = {
	'H',
	0x00,
	'y',
	0x00,
	'd',
	0x00,
	'r',
	0x00,
	'a',
	0x00,
	'd',
	0x00,
	'a',
	0x00,
	'n',
	0x00,
	'c',
	0x00,
	'e',
	0x00,
	'r',
	0x00,
    ',',
    0x00,
    ' ',
    0x00,
$(for (( i=0; i<${#fullgitinfo}; i++ )); do echo "	'${fullgitinfo:$i:1}',";echo "	0x00,"; done;)
    ',',
    0x00,
    ' ',
    0x00,
$(for (( i=0; i<${#ctime}; i++ )); do echo "	'${ctime:$i:1}',";echo "	0x00,"; done;)
    ',',
    0x00,
    ' ',
    0x00,
$(for (( i=0; i<${#sha}; i++ )); do echo "	'${sha:$i:1}',";echo "	0x00,"; done;)
};
EOF

    mv "${VERSIONSRC}.tmp" "${VERSIONSRC}"
fi

