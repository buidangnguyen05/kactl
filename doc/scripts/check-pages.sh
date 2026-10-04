#!/usr/bin/env bash
# Fails if the built notebook has more pages than allowed (cover included).
# Reads the page count pdflatex writes to its log, so it needs no PDF tools.
# Usage: doc/scripts/check-pages.sh [log file]   (MAX_PAGES overrides the limit)
DIR=$(dirname "$0")/../..
LOG=${1:-$DIR/build/kactl.log}
MAX_PAGES=${MAX_PAGES:-26}

if [ ! -f "$LOG" ]; then
    echo "::error::$LOG not found; build the PDF first (make kactl)"
    exit 1
fi
pages=$(sed -n 's/^Output written on .* (\([0-9]*\) pages\{0,1\},.*/\1/p' "$LOG" | tail -n 1)
if [ -z "$pages" ]; then
    echo "::error::no page count in $LOG; did pdflatex finish?"
    exit 1
fi

if [ -n "$GITHUB_STEP_SUMMARY" ]; then
    echo "kactl.pdf: **$pages / $MAX_PAGES pages** (cover included)" >> "$GITHUB_STEP_SUMMARY"
fi
if [ "$pages" -gt "$MAX_PAGES" ]; then
    echo "::error::kactl.pdf has $pages pages, over the $MAX_PAGES-page limit (cover included)"
    exit 1
fi
echo "kactl.pdf has $pages pages (limit $MAX_PAGES, cover included)"
