#!/usr/bin/python3

import os
import subprocess


if __name__ == "__main__":
    path = "../Benchmark/"
    l = os.listdir (path);
    l.sort();

    for f in l:
        if f[0] == 'i':
            output = subprocess.getoutput ("../airSche2.exe < " + path + f)
            print (f + " " + output)




