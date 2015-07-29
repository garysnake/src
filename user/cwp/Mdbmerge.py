#!/usr/bin/env python
'''
A program that can be used to merge separate SCons databases together.  Typically used when an SConstruct is split
into multiple pieces (e.g. on a cluster).

args:
    outdb - path of the output database

    strings - names of the databases to merge together
'''

try:
    import dbhash, pickle
except:
    raise Exception('must have dbhash module installed... see python docs')


import sys, os 
sys.stdout = sys.__stderr__

outname = None
dbs = []
for arg in sys.argv[1:]:
    if 'outdb' in arg:
        outname = arg.split('=')[1]
    else:
        if os.path.exists(arg):
            dbs.append(arg)
            print 'FOUND: %s' % arg
        else:
            print 'WARNING: %s was specified but not found' % arg
        
if not outname:
    raise Exception('must specify outdb=filename')


ndb = dbhash.open(outname,'n')

outerdb = {}
for db in dbs:
    print 'Merging... %s' % db
    opendb = dbhash.open(db,'r')
    keys = opendb.keys()
    for key in keys:
        print '...Key: %s' % key
        outerdb.setdefault(key,{})
        objects = pickle.loads(opendb[key])
        for obj in objects.keys():
            print '......Subkey: %s' % obj
            if outerdb[key].has_key(obj):
                print 'WARNING: key collision: %s dbase key: %s ' % (db,obj)
            else:
                outerdb[key][obj] = objects[obj]
    opendb.close()

print 'Writing out database...'
for key in outerdb.keys():
    ndb[key] = pickle.dumps(outerdb[key],0)

ndb.sync()
ndb.close()
