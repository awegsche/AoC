#! /bin/bash
#
echo "make 2022 (non-GUI)"
cmake \
    --build /mnt/c/Users/andiw/projects/AoC/debug_build \
    --target 2022

echo "run 2022 (non-GUI)"
/mnt/c/Users/andiw/projects/AoC/debug_build/2022/2022
