#!/bin/sh

BRIDGE_NAME=mybr0
IP_ADDR=192.168.0.1
NETWORK=192.168.0.0/24

if [ $# -ge 1 ]; then
  BRIDGE_NAME=$1
fi
if [ $# -ge 2 ]; then
  ADDR=$2
fi
if [ $# -ge 3 ]; then
  NETWORK=$3
fi

echo "Bridge Name:" $BRIDGE_NAME
echo "IP Address :" $IP_ADDR
echo "Network    :" $NETWORK

brctl addbr $BRIDGE_NAME
ifconfig $BRIDGE_NAME up $IP_ADDR
iptables -t nat -A POSTROUTING -s $NETWORK ! -d $NETWORK -j MASQUERADE

