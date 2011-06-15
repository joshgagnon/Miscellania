#!/usr/bin/python
from diff_match_patch import diff_match_patch, patch_obj
from mod_python import apache
from mod_python import Session
import sys
diff = diff_match_patch();

def index(req, wall=""):	
	if(wall == ''):
		req.headers_out['location'] = 'http://172.21.21.39/scratch_wall.py?wall=random'
		req.status = apache.HTTP_MOVED_TEMPORARILY
		return	
	session = Session.Session(req)
	#try:
	session['wall'] = wall
	session['updates']=1
	session['text'] =''

	open('/home/josh/scratch_wall/walls/%s' % session['wall'],'a').close()
	req.content_type = "text/html"
	with open('/home/josh/scratch_wall/scratch_wall.html', 'r') as main:
		req.write(main.read())
	session.save()
	

def pull(req):
	session = Session.Session(req)
	patch = ''
	with open('/home/josh/scratch_wall/walls/%s' % session['wall'],'r') as wall:
		session['updates'] += 1
		text = wall.read()
		patch = diff.patch_toText(diff.patch_make(session['text'],text))	
		session['text'] = text		
	session.save()
	req.write( patch )
	

def push(req):
	session = Session.Session(req)
	session['updates'] = 1
	session['text'] = diff.patch_apply(diff.patch_fromText(req.form['delta']),session['text'])[0]
	with open('/home/josh/scratch_wall/walls/%s' % session['wall'],'w') as wall:
		wall.write( session['text']);
	session.save()





