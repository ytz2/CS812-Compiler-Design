#!/usr/bin/env python

import os
import subprocess
import sys
import datetime

#isTfile=lambda infile: True if infile[-2:]=='.t' else False
def isTfile(infile):
	if infile[-2:]=='.t':
		return True
	else:
		return False

tdir=sys.argv[1] #test directory
cwd=os.getcwd() #current directory
                #path=cwd+'/'+tdir+'/'
tfiles=filter(isTfile,os.listdir(cwd+'/'+tdir))
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

    flog.write('LEXDBG:'+os.linesep)
    for eachline in p_lexdbg.stdout.readlines():
        flog.write(eachline)

    #test tc
    p_tc = subprocess.Popen('./tc -before -after <'+tdir+'/'+
                            eachfile+' >'+tdir+'/'+eachfile[0:-2]+'.s',
                            shell=True, stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT)

    flog.write(os.linesep+'tc -before -after:'+os.linesep)
    for eachline in p_tc.stdout.readlines():
        flog.write(eachline)


    os.chdir(cwd+'/'+tdir)
    out_tc = subprocess.Popen('gcc -m32 '+eachfile[0:-2]+'.s ../RTS.c -o '
                              +eachfile[0:-2],
                              shell=True, stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT)

    flog.write(os.linesep)
    flog.write('COMPILE+OUTPUT:'+os.linesep)
    for eachline in out_tc.stdout.readlines():
        flog.write(eachline)
 

    out_test = subprocess.Popen('./'+eachfile[0:-2],
                              shell=True, stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT)

    flog.write(os.linesep)
    for eachline in out_test.stdout.readlines():
        flog.write(eachline)
    flog.close()
    os.chdir(cwd)

subprocess.call(['make','clean'])
