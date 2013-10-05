# -*- coding: utf-8 -*-
import mincemeat
import glob
from stopwords import *

datasource = dict()
key = 0
for fname in glob.glob('hw3data/*'):
    with open(fname) as f:
        content = f.readlines()
        for line in content:
            datasource[key] = line
            key = key + 1
        
def mapfn(k, v):
    import string
    from stopwords import *
    info = v.split(':::')
    id = info[0]
    authorlist = info[1]
    title = info[2]
    title = title.lower()
    title.replace('-', ' ')
    title = title.translate(None, string.punctuation)
    words = title.split()
    logging.info("words: %s" % (words)) 
    wordsStopped = []   
    for w in words:
        if w not in allStopWords:
            wordsStopped.append(w)
    logging.info("wordsstopped: %s" % (wordsStopped,)) 
    for author in authorlist.split('::'):
            logging.info("author: %s" % (author,))
            for w in wordsStopped:
                yield author, w

def reducefn(k, vs):
    logging.info("vs received: %s" % (vs)) 
    kvs = dict()
    #vvs = [item for sublist in vs for item in sublist]
    for w in vs:
        if w in kvs:
            kvs[w] = kvs[w] + 1
        else:
            kvs[w] = 1
    return kvs


s = mincemeat.Server()
s.datasource = datasource
s.mapfn = mapfn
s.reducefn = reducefn

results = s.run_server(password="changeme")

#for w in results:
#	print w, results[w]
print results['Siegbert Drüe']
#print results
