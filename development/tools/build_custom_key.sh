#!/bin/sh

#'/C=US/ST=California/L=Mountain View/O=Android/OU=Android/CN=Android/emailAddress=android@android.com'
CUSTOM_KEY="/C=CN/ST=GD/L=SZ/O=tinno/OU=tinno/CN=tinno/emailAddress=limi.zhang@tinno.com"
./make_key shared  $CUSTOM_KEY;
./make_key platform  $CUSTOM_KEY;
./make_key media  $CUSTOM_KEY;
./make_key releasekey  $CUSTOM_KEY;
