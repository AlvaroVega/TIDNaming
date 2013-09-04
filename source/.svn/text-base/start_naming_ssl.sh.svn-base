#
#  Copyright (C) 2009 Telefonica I+D S.A.
#
#  This source file is a property of Telefonica I+D. You can not
#  use it for any purpose unless you have an explicit authorization
#  by owner.
#

#!/bin/sh
if [ -z "$TIDNAMING_HOME" ]; then
    echo "Environment variable TIDNAMING_HOME must be set"
    exit 1
fi

if [ ! -f $TIDNAMING_HOME/bin/ns_server_cert.pem ]; then
    echo "Generating new ns_server key and X509 certificate..."
    openssl genrsa -out ns_server_key.pem 1024
    openssl req -new -key ns_server_key.pem -x509 -out ns_server_cert.pem -days 1825
fi

echo "Starting TIDNaming daemon..."
$TIDNAMING_HOME/bin/ns_server \
    -ORB_ssl_private_key ns_server_key.pem \
    -ORB_ssl_certificate ns_server_cert.pem \
    -ORB_ssl_port 2345 \
    --ior ns.ior \
    -ORB_trace_level 5 \
    -ORB_trace_file ns_server.log
