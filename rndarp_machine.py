#! /bin/python
import sys


def intervalNoteBPM(bpm):
	"""da bpm a intervallo ottavo"""
	bps = bpm/60
	durataBattutaSec = bps * 4	
	durataNota = durataBattutaSec/8
	durataNotaMillisec = durataNota * 1000
	return durataNotaMillisec

if sys.argv[1] is not None:
	bpm = float(sys.argv[1])
	print intervalNoteBPM(bpm)
