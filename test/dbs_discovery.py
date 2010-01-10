#!/usr/bin/env python

#===================================================================
# This python script is querying https://cmsweb.cern.ch/dbs_discovery/
# so to get the list of input files. It can be called interactively,
# or imported within a cmsRun config file. In the later case, one
# must call :
#   search(), to get the list of primary files
#   search2(), to get the list of eventual secondary files
# 
# The selection of files is configured thanks to shell
# environment variables: 
# 
#   DBS_RELEASE, for example CMSSW_2_2_0_pre1
#   DBS_SAMPLE, for example RelValSingleElectronPt35
#   DBS_COND , for example MC_31X_V2-v1
#   DBS_TIER , for example RECO
#   DBS_TIER_SECONDARY, for eventual secondary files
#
# In the three last variables, one can use wildcard *
#===================================================================


import httplib, urllib, urllib2, types, string, os, sys

def common_search(dbs_tier):

  if os.environ['DBS_RELEASE'] == "LOCAL":
    result = []
    for line in  open('dbs_discovery.txt').readlines():
      line = line.strip()
      if line == "": continue
      if line.find(os.environ['DBS_SAMPLE'])== -1: continue
      if line.find(os.environ['DBS_COND'])== -1: continue
      if line.find(dbs_tier)== -1: continue
      result.append('file:'+line)
  else:
    url = "https://cmsweb.cern.ch:443/dbs_discovery/aSearch"
    if os.environ['DBS_RELEASE'] != "Any":
      input = "find file where release = " + os.environ['DBS_RELEASE']
    if os.environ['DBS_SAMPLE'] != "Any":
      input = input + " and primds = " + os.environ['DBS_SAMPLE']
    input = input + " and dataset like *" + os.environ['DBS_COND'] + "*" + dbs_tier + "*"
    final_input = urllib.quote(input) ;

    agent   = "Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)"
    ctypes  = "text/plain"
    headers = { 'User-Agent':agent, 'Accept':ctypes}
    params  = {'dbsInst':'cms_dbs_prod_global',
               'html':0,'caseSensitive':'on','_idx':0,'pagerStep':-1,
               'userInput':final_input,
               'xml':0,'details':0,'cff':0,'method':'dbsapi'}
    data    = urllib.urlencode(params,doseq=True)
    req     = urllib2.Request(url, data, headers)
    data    = ""

    try:
      response = urllib2.urlopen(req)
      data = response.read()
    except urllib2.HTTPError, e:
      if e.code==201:
        print e.headers       
        print e.msg
        pass
      else:
        raise e

    result = []
    for line in data.split("\n"):
      if line != "" and line[0] =="/":
        result.append(line)

  return result

def search():
  return common_search(os.environ['DBS_TIER'])

def search2():
  return common_search(os.environ['DBS_TIER_SECONDARY'])

if __name__ == "__main__":
  if not os.environ.has_key('DBS_TIER_SECONDARY'):
    os.environ['DBS_TIER_SECONDARY'] = ""
  if os.environ['DBS_TIER_SECONDARY'] == "":
    for file in search():
      print file
  else:
    for file in search():
      print "primary:"+file
    for file in search2():
      print "secondary:"+file

	
	
