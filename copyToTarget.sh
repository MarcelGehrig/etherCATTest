#!/bin/bash

script="$(readlink -f $0)"
script_dir="$(dirname $script)"

build_dir="$script_dir/buildx86"
install_dir="$(dirname $script_dir)/installx86"

target_username="ubuntu"
target_hostname="pcwinf039-linux"
target_dir="/home/ubuntu/remote"
tmp_dir="/tmp/etherCatTest"

rm -rf $tmp_dir
mkdir -p $tmp_dir

## application
cp buildx86/ethercattest $tmp_dir
cp buildx86/eeros_ethercattest $tmp_dir

cp -r ../installx86 $tmp_dir


echo "copy to " "$target_dir"
scp -r $tmp_dir/* $target_username@$target_hostname:$target_dir
