import sys, math, numpy

def lseq(start,stop,n):
	start = math.log(start)
	stop = math.log(stop)
	step = (stop-start)/n
	times = numpy.arange(start, stop, step)
	for i in range(len(times)):
		times[i] = math.exp(times[i])
	return times

if __name__ == "__main__":
	times = lseq(float(sys.argv[1]), float(sys.argv[2]), float(sys.argv[3]))
	for t in times:
		print t
