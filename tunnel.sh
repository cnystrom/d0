#!/bin/sh
#
# Note: When using this script to tunnel NewI\O through ssh use
# "localhost" for hostname in the URL and it will automatically go
# to apps.newio.org. - Chris
#

ssh -l nio -L 32662:apps.newio.org:32662 apps.newio.org

