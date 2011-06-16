x="""import os,re,gobject
from gtk import *
from subprocess import Popen, PIPE
df=re.compile('/dev/([^\s]+)\s+\d+\s+(\d+)\s+(\d+)\s+([^\s]+)\s+(.*)\\n')
dk=re.compile('\s+\d+\s+\d+\s+([^\s]+) \d+ \d+ (\d+) \d+ \d+ \d+ (\d+)')
c=lambda z:'%dB'%z if z<1024 else ['%.1f%s'%(float(z)/x[0],x[1]) for x in [(1<<40,'T'),(1<<30,'G'),(1<<20,'M'),(1<<10,'K')] if z>=x[0]][0]
class Vol(object):
   def __init__(s,vbox,dt, dp):
      s.u,s.r,s.w,s.label,s.lf = int(dt[0]),int(dp[0]),int(dp[1]),Label(" "+dt[3].replace('/media','')),Label(c(int(dt[1])*1024))
      s.rr,s.wr,s.size,s.hbox,s.bar,align=0,0,s.u+int(dt[1]),HBox(homogeneous=False,spacing=0),ProgressBar(),Alignment(yalign=0.8)
      s.bar.set_orientation(PROGRESS_RIGHT_TO_LEFT);
      s.bar.set_size_request(75,-1)
      s.bar.set_fraction(float(s.u)/s.size)
      s.bar.show()
      align.add(s.bar)
      align.show()
      s.lf.set_size_request(50,-1)
      s.lf.show()
      s.hbox.pack_start(s.lf,expand=False,fill=False)
      s.hbox.show()
      s.hbox.pack_start(align,expand=False,fill=False)
      s.label.show()
      s.hbox.pack_start(s.label,expand=False,fill=False)  
      s.bar.set_events(gdk.ENTER_NOTIFY_MASK | gdk.LEAVE_NOTIFY_MASK)
      vb.pack_start(s.hbox,expand=False,fill=False)    
   def update(s,dt, dp,time):
      s.u,s.r,s.w,s.wr,s.rr = int(dt[0]),int(dp[0]),int(dp[1]),s.wr*0.2+(int(dp[1])-s.w)*409600/time,s.rr*0.2+(int(dp[0])-s.r)*409600/time
      t = 'Read:%s/s  Write:%s/s' % (c(s.rr), c(s.wr))
      s.bar.set_tooltip_text(t)
      s.label.set_tooltip_text(t)
      s.lf.set_tooltip_text(t)
      s.lf.set_text(c(int(dt[1])*1024))
      s.bar.set_fraction(float(s.u)/s.size)
def update():
   global vols
   dfmat,dkd=df.findall(re.sub("1000\\n","1000",Popen(["df"],stdout=PIPE).communicate()[0])),dict((x[0],x[1:]) for x in dk.findall(open("/proc/diskstats","r").read()))
   st = set(zip(*dfmat)[0])
   dfmat= [x for x in dfmat if x[0] in st and st.remove(x[0]) == None]
   dmap = dict(dict((x,x) for x in os.listdir('/dev/')).items() + dict(('mapper/udisks-luks-uuid-'+x+'-uid1000',os.readlink('/dev/disk/by-uuid/'+x).replace('../../','')) for x in os.listdir('/dev/disk/by-uuid')).items())
   vols = dict(vols.items()+([(x[0],Vol(vb,x[1:],dkd[dmap[x[0]]])) for x in dfmat if x[0] not in vols]))
   for vol in dfmat:
      vols[vol[0]].update(vol[1:],dkd[dmap[vol[0]]],rate)
   for k in [k for k in vols.keys() if k not in [x[0] for x in dfmat]]:
      vb.remove(vols[k].hbox)
      del vols[k]
      w.resize(*w.size_request())
   return True
w,rate,vols,vb=Window(WINDOW_TOPLEVEL),1000,{},VBox(False,spacing=1)
w.set_border_width(10)
w.set_title('Storage')
w.resize(100,50);
w.set_position(WIN_POS_MOUSE)
w.set_resizable(False)
update()
w.add(vb)
vb.show()
w.show()    
timer = gobject.timeout_add(rate,update)
w.connect("destroy",main_quit)
main()
gobject.source_remove(timer)"""

import base64
import zlib
x = base64.b64encode((zlib.compress(x,9)))
print len(x)
print x
print zlib.decompress(base64.b64decode(x))
exec(zlib.decompress(base64.b64decode(x)))
