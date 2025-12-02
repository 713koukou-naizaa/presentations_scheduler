#!/bin/bash

if ! make -j 4; then
  exit 1
fi

php prod_web_launcher.php