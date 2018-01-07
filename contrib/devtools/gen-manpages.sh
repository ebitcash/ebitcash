#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

EBITCASHD=${EBITCASHD:-$SRCDIR/ebitcashd}
EBITCASHCLI=${EBITCASHCLI:-$SRCDIR/ebitcash-cli}
EBITCASHTX=${EBITCASHTX:-$SRCDIR/ebitcash-tx}
EBITCASHQT=${EBITCASHQT:-$SRCDIR/qt/ebitcash-qt}

[ ! -x $EBITCASHD ] && echo "$EBITCASHD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
EBCVER=($($EBITCASHCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for ebitcashd if --version-string is not set,
# but has different outcomes for ebitcash-qt and ebitcash-cli.
echo "[COPYRIGHT]" > footer.h2m
$EBITCASHD --version | sed -n '1!p' >> footer.h2m

for cmd in $EBITCASHD $EBITCASHCLI $EBITCASHTX $EBITCASHQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${EBCVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${EBCVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
