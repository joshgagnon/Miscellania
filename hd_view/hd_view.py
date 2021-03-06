#see hd_tiny for canonical version
import os,re,pygtk,gtk,gobject
pygtk.require('2.0')
from subprocess import Popen, PIPE
import zlib

df_pat=re.compile('/dev/([^\s]+)\s+\d+\s+(\d+)\s+(\d+)\s+([^\s]+)\s+(.*)\n')
disk_pat=re.compile('\s+\d+\s+\d+\s+([^\s]+) \d+ \d+ (\d+) \d+ \d+ \d+ (\d+)')
s=lambda z:'%dB'%z if z<1024 else ['%.1f%s'%(float(z)/x[0],x[1]) for x in [(1<<40,'T'),(1<<30,'G'),(1<<20,'M'),(1<<10,'K')] if z>=x[0]][0]

class Volume(object):
   def __init__(self,vbox,df_tup, disk_tup):
      self.used,self.read,self.write,self.labtxt,self.mouse_on = int(df_tup[0]),int(disk_tup[0]),int(disk_tup[1]),df_tup[3].replace('/media',''),False
      self.read_rate, self.write_rate, self.size = 0,0,self.used + int(df_tup[1])
      self.hbox,self.bar = gtk.HBox(homogeneous=False,spacing=0),gtk.ProgressBar()
      self.bar.set_orientation(gtk.PROGRESS_RIGHT_TO_LEFT);
      self.bar.set_size_request(75,-1)
      self.bar.set_fraction(float(self.used)/self.size);
      self.bar.show()
      align = gtk.Alignment(yalign=0.8)
      align.add(self.bar)
      align.show()
      self.label_f = gtk.Label(s(int(df_tup[1])*1024))
      self.label_f.set_size_request(50,-1)
      self.label_f.show()
      self.hbox.pack_start(self.label_f,expand=False,fill=False)
      self.hbox.show()
      self.hbox.pack_start(align,expand=False,fill=False)
      self.label = gtk.Label(" "+self.labtxt)
      self.label.show()
      self.hbox.pack_start(self.label,expand=False,fill=False)
  
      self.bar.set_events(gtk.gdk.ENTER_NOTIFY_MASK | gtk.gdk.LEAVE_NOTIFY_MASK)
      self.bar.connect("enter_notify_event",self.mouse_change)
      self.bar.connect("leave_notify_event",self.mouse_change)
      vbox.pack_start(self.hbox,expand=False,fill=False)
      
 

   def mouse_change(self,widget,event):
      self.mouse_on = not self.mouse_on
      self.draw()

   def draw(self):
      self.bar.set_fraction(float(self.used)/self.size)
      #if self.mouse_on:
      #   self.label.set_text()
      #else:
      #   self.label.set_text(self.labtxt)

   def update(self,df_tup, disk_tup,time):
      self.read_rate = self.read_rate * 0.5 +(int(disk_tup[0])-self.read)*512 / time * 1000 * 0.5
      self.write_rate = self.write_rate * 0.5 +(int(disk_tup[1])-self.write)*512 / time * 1000 * 0.5
      self.used,self.read,self.write = int(df_tup[0]),int(disk_tup[0]),int(disk_tup[1])
      self.bar.set_tooltip_text('R:%s/S W:%s/S' % (s(self.read_rate), s(self.write_rate)))
      self.draw()

def update():
   global volumes
   dfmat=df_pat.findall(re.sub("1000\n","1000",Popen(["df"],stdout=PIPE).communicate()[0]))
   disk_dict = dict((x[0],x[1:]) for x in disk_pat.findall(open("/proc/diskstats","r").read()))
   dmap = dict(dict((x,x) for x in os.listdir('/dev/')).items() + dict(('mapper/udisks-luks-uuid-'+x+'-uid1000',os.readlink('/dev/disk/by-uuid/'+x).replace('../../','')) for x in os.listdir('/dev/disk/by-uuid')).items())
   volumes = dict(volumes.items()+([(x[0],Volume(vbox,x[1:],disk_dict[dmap[x[0]]])) for x in dfmat if x[0] not in volumes]))
   for vol in dfmat:
      volumes[vol[0]].update(vol[1:],disk_dict[dmap[vol[0]]],rate)
   for k in [k for k in volumes.keys() if k not in [x[0] for x in dfmat]]:
      vbox.remove(volumes[k].hbox)
      del volumes[k]
      win.resize(*win.size_request())
   return True

win,rate,volumes=gtk.Window(gtk.WINDOW_TOPLEVEL),1000,{}
win.set_border_width(10)
#.win.modify_bg(gtk.STATE_NORMAL, gtk.gdk.color_parse('#000000'))
win.set_title('Storage')
win.resize(100,50);
win.set_position(gtk.WIN_POS_MOUSE)
win.set_resizable(False)
vbox = gtk.VBox(False,spacing=0)
update()
win.add(vbox)
vbox.show()
win.show()      
timer = gobject.timeout_add(rate, update)
win.connect("destroy",gtk.main_quit)
gtk.main()
gobject.source_remove(timer)
