#!/bin/sh
./my-fault-injection-tool read_EIO
./my-fault-injection-tool read_EINTR
./my-fault-injection-tool write_ENOSPC
./my-fault-injection-tool write_EIO
./my-fault-injection-tool select_ENOMEM
./my-fault-injection-tool malloc_ENOMEM
