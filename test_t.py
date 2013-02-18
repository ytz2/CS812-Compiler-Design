#!/usr/bin/env python

import os
import subprocess
import sys
import datetime

isTfile=lambda infile: True if infile[-2:]=='.t' else False


tdir=sys.argv[1] #test directory
cwd=os.getcwd() #current directory

tfiles=filter(isTfile,os.listdir(os.getcwd()+'/'+tdir))
#now T program test files are listed here


make_err=subprocess.call(['make','lexdbg'])
make_err=subprocess.call(['make','tc'])

if make_err != 0:
    subprocess.call(['make','clean'])
    sys.exit("Make Failure\n")


# Now Try to test each file
for eachfile in tfiles:

    time=datetime.datetime.now().strftime('%Y%m%d%H%m%S')
    #test lexdbg
    flog=open(cwd+'/'+tdir+'/'+eachfile[0:-2]+
                  '_'+time+'.txt','w')
    p_lexdbg = subprocess.Popen('./lexdbg <'+tdir+
                                '/'+eachfile, shell=True,
                                stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT)
    
    for eachline in p_lexdbg.stdout.readlines():
        flog.write(eachline)

    #test tc
    p_tc = subprocess.Popen('./tc -before -after <'+tdir+'/'+
                            eachfile+' >'+tdir+'/'+eachfile[0:-2]+'.s',
                            shell=True, stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT)
    
    for eachline in p_tc.stdout.readlines():
        flog.write(eachline)

        
    flog.close()

subprocess.call(['make','clean'])
