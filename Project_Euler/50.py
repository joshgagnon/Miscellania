import pickle
from xml.dom import minidom

def primes(n):
    if n<2:
        return []
    nums = range(2,int(n))
    p=[]
    while nums:
        prime=nums[0]
        p.append(prime)
        if prime>n**0.5:
            return p
        print prime
        for i in nums[1:]:
            if i % prime==0:
                nums.remove(i)
        nums.remove(nums[0])
    return p

def main():
    #p=primes(1000000)
    #myfile = open("./primes1.txt",'r')
    #primes = pickle.load(myfile[1:10000])
    primes=[]
    primes = pickle.load( open("./primes",'r'))
    print "Got primes"
    for i in range(len(p)):
        print i
        for j in range(i+1,i+20):
            if sum(p[i:j])==1000000:
                print sum(p[i:j])
                return
    return

main()

