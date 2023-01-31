#!/bin/bash
# . /usr/local/fbpackages/utils/ast-functions

/bin/echo -n "Setup sensor monitoring for EVB-NPCM845... "

FRUS="bmc"

exec /usr/local/bin/sensord $FRUS
