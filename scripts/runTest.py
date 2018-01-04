#!/usr/bin/python3

import os


if __name__ == "__main__":
    path = "../Benchmark/"
    l = os.listdir (path);
    l.sort();

    for f in l:
        if f[0] == 'i':
            print (path + f)
            os.system ("../airSche1.exe < " + path + f)




