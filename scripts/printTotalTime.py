#!/usr/bin/python3

import os
import time
import subprocess


if __name__ == "__main__":
    path = "../Benchmark/"
    l = os.listdir (path);
    l.sort();

    
    startTime = time.time()
    for f in l:
        if f[0] == 'i':
            programStartTime = time.time()
            output = subprocess.getoutput ("../airSche1.exe < " + path + f)
            print (f + " take time: " + str(time.time() - programStartTime))

    print ("total time: " + str(time.time() - startTime))





