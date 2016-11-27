#!/usr/bin/env bash

export COB_SCREEN_ESC=YES
export COB_SCREEN_EXCEPTIONS=YES
export COB_LIBRARY_PATH=~/bin
#export COB_PRE_LOAD=irs010:irs020:irs030:irs040:irs050:irs055:irs060:irs065:irs070:irs080:irs085:irs090:irsub1:irsub2:irsub3:irsub4:irsub5:irsubp
# above not needed but here as a reminder in case it needs to be installed
#
#export PATH=~/bin:.:/home/mcarter/perl5/bin:/home/mcarter/repos/cerbo/mython/scripts:.:/home/mcarter/repos/cerbo/mcacc/src:/home/mcarter/repos/cerbo/mcacc/scripts:/home/mcarter/repos/redact/docs/accts2016/data:/home/mcarter/scratch/youtube-dl:/home/mcarter/repos/cerbo/beancounter:/home/mcarter/repos/redact/docs/dotfiles/scripts:/home/mcarter/.local/bin:/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin
export ACAS_IRS=~/IRS
export ACAS_LEDGERS=~/ACAS
export ACAS_BIN=~/bin
prepath $ACAS_BIN
export TMPDIR=~/tmp
# the next one is an issue as ALL data files will go there regardless
#    so remarked out


