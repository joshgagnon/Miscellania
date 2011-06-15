#!/usr/bin/python

import re
import sys
import os
from shutil import copy
from glob import glob

index = ' '.join(open(sys.argv[1]).read().split()).replace('%20','').replace('&amp;','&').replace('&ndash;','-').replace('&quot;',"'")

path = os.path.dirname(sys.argv[1]) + os.sep

row_pat = re.compile('<TD HEIGHT=23 ALIGN=LEFT><FONT SIZE=3>([^<]+)</FONT></TD> <[^>]+><[^>]+>(([^<]+)|(<BR>))</FONT></TD> <[^>]+><[^>]+>(([^<]+)|(<BR>))</FONT></TD> <[^>]+><[^>]+>([^<]+)</FONT></TD> <[^>]+><[^>]+>((<A HREF="([^<]+)">[^<]+</A>)|(<BR>))</FONT></TD> <[^>]+><[^>]+>((<A HREF="([^<]+)">[^<]+</A>)|(<BR>))</FONT></TD>')
rows = row_pat.findall(index)

for row in rows:
    title = row[0].rstrip()
    if(title[-5:] == ', The'):
        title = title = 'The '+title[:-5]
    elif(title[-3:] == ', A'):
        title = title = 'A '+title[:-3]
    subtitle = ''
    if(not row[1] == '<BR>'):
        subtitle = row[1].rstrip()
        if (subtitle[-5:] == ', The'):
            subtitle = title = 'The '+subtitle[:-5]
        elif(subtitle[-3:] == ', A'):
            subtitle = title = 'A '+subtitle[:-3]
    fulltitle = '%s : %s' % (title,subtitle)
    if(subtitle == ''):
        fulltitle = title
    fulltitle = fulltitle.replace('/','-')

    first_name,last_name,file_loc = row[4],row[7],row[10]
    fullname = '%s, %s' % (last_name, first_name)
    if(first_name == '<BR>'):
        fullname = last_name;
    fullname = fullname.rstrip()
    if(not row[12] == '<BR>'):
        file_loc = row[14]
    fulltitle += " - " + fullname # for calibre
    ext = os.path.splitext(file_loc)[1]
    output = 'clean'+os.sep+fullname+os.sep+fulltitle+ext
    if not os.path.exists('clean'+os.sep+fullname):
        os.mkdir('clean'+os.sep+fullname);
    if not os.path.exists(output):
        copy(path+file_loc, output)
  
    #find if there are pics,
    if ext == ".htm" or ext == ".html":
        for image_ext in ['*.png', '*.jpeg', '*.bmp', '*.jpg', '*.gif', '*.tiff']:
            query = path+os.path.dirname(file_loc)+os.sep+image_ext
            image_list = glob(query)
            if image_list:
                if not os.path.exists('clean'+os.sep+fullname+os.sep+'images'):
                    os.mkdir('clean'+os.sep+fullname+os.sep+'images')
                for image in image_list:
                    copy(image,'clean'+os.sep+fullname+os.sep+'images'+os.sep+os.path.basename(image))
        #now alter doc
        html = ''
        os.chmod(output,0600)
        with open(output,'r') as f:
            html = re.sub(r'src="([^"]+)"',r'src="images/\1"', f.read())
        with open(output,'w') as f:
            f.write(html)
