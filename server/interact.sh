#!/bin/sh

OBSERVATIONS='
{"command": ["observe_property", 1, "volume"]}
{ "command": ["observe_property", 2, "percent-pos"] }
{"command": ["observe_property", 3, "path"]}
{"command": ["observe_property", 4, "pause"]}
{ "command": ["observe_property", 5, "idle-active"] }
'

CMD="(echo '$OBSERVATIONS'; cat - ) | nc -U /tmp/mpvsocket | grep 'property-change'"
rlwrap sh -c "$CMD"
