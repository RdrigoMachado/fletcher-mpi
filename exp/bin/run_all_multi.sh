#!/bin/bash
./multi_conn_multi_files.sh &
wait
./multi_conn_single_file.sh &
