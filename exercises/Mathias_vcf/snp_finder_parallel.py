#!/usr/bin/env python2

from __future__ import with_statement, print_function
import multiprocessing as mp


def createSNPDict(snplist):
	'''
	Creates a set with all SNPs in the form tuple(CHR, POS, ALT),
	e.g. tuple(chr18, 3000528, T), given a list with SNPs formatted as in "the file".
	'''

	snpdict = {}
	for elem in snplist:
		l = elem.split('\t')
		#               CHR   POS   ALT
		snpdict[tuple((l[0], l[1], l[4]))] = elem

	return snpdict


def getSNPs(workpackage):
	'''
	Returns a set with SNPs that are in sbjct but not in ctrl. sbjct and ctrl
	must be file names.
	'''

	ctrllist, sbjctlist = workpackage

	ctrldict = createSNPDict(ctrllist)
	sbjctdict = createSNPDict(sbjctlist)

	# This returns dict keys that are in sbjctdict but not in ctrldict
	snps = set(sbjctdict).difference(set(ctrldict))

	return [sbjctdict[snp] for snp in snps]


def readSNPfile(fname):
	'''
	Reads a SNP file, discards the commented lines and returns all other lines
	as a list (suitable for createSNPSet()).
	'''

	snplists = []
	current = None
	with open(fname, 'r') as f:
		for line in f:
			if line.startswith('#'):
				continue
			if line[:5] != current:
				snplists.append([])
				current = line[:5]
			snplists[-1].append(line)

	return snplists


def prepareParallel(ctrlfile, sbjctfile):
	ctrllists = readSNPfile(ctrlfile)
	sbjctlists = readSNPfile(sbjctfile)

	if len(ctrllists) != len(sbjctlists):
		raise ValueError('Length of the two lists are not the same ({} vs {})'.format(len(ctrllists), len(sbjctlists)))

	return zip(ctrllists, sbjctlists)


if __name__ == '__main__':
	ctrlfile = 'MMR_664_control.raw.snps_filtered.vcf'
	sbjctfile = 'MMR_1370_subject.raw.snps_filtered.vcf'

	workpackages = prepareParallel(ctrlfile, sbjctfile)

	pool = mp.Pool(4)

	result = pool.map(getSNPs, workpackages)

	with open('result.txt', 'w') as out:
		totalLength = 0
		for chromosome in result:
			totalLength += len(chromosome)
			for snp in chromosome:
				out.write(snp)

	print('number of SNPs found:', totalLength)


