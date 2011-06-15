def get_words(length=0):
    return set([x.strip().lower() for x in open('/usr/share/dict/words','r').readlines() if (not length or len(x.strip()) == length) and x.strip().lower().isalpha()])

def gen_all():
    map = {'a':2, 'b':2,'c':2,'d':3,'e':3,'f':3,'g':4,'h':4,'i':4,'j':5,'k':5,'l':5,'m':6,'n':6,'o':6,'p':7,'q':7,'r':7,'s':7,'t':8,'u':8,'v':8,'w':9,'x':9,'y':9,'z':9}
    words = get_words
    for x in words:
        print x, ''.join([str(map[y]) for y in x])

def num_to_words(num):   
    map = {0: " o", 1: " l",2:"abc",3:"def",4:"ghi",5:"jkl",6:"mno",7:"pqrs",8:"tuv",9:"wxyz"}
    pin = [ list(map[int(x)]) for x in list(num) ]
    r=[[]]
    for x in pin:
        r = [ i + [y] for y in x for i in r ]
    words = get_words(len(num))
    results = [''.join(x) for x in r]
    #print [''.join(x) for x in r]
    print [y for y in results if y in words]
    words = get_words(6)
    print [y for y in words if y in "\n".join(results)]

num_to_words('272538789')

