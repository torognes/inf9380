#!/usr/bin/env python2

from __future__ import with_statement, print_function

def createSNPDict(snplist):
	'''
	Creates a set with all SNPs in the form tuple(CHR, POS, ALT),
	e.g. tuple(chr18, 3000528, T), given a list with SNPs formatted as in "the file".
	'''

	snpdict = {}
	for elem in snplist:
		l = elem.split('\t')
		#              CHR   POS   ALT
		snpdict[tuple((l[0], l[1], l[4]))] = elem

	return snpdict


def getSNPs(ctrllist, sbjctlist):
	'''
	Returns a set with SNPs that are in sbjct but not in ctrl. sbjct and ctrl
	must be file names.
	'''

	ctrllist = readSNPfile(ctrlfile)
	sbjctlist = readSNPfile(sbjctfile)

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

	snplist = []
	with open(fname, 'r') as f:
		for line in f:
			if line.startswith('#'):
				continue
			snplist.append(line)

	return snplist


if __name__ == '__main__':
	ctrlfile = 'MMR_664_control.raw.snps_filtered.vcf'
	sbjctfile = 'MMR_1370_subject.raw.snps_filtered.vcf'

	result = getSNPs(ctrlfile, sbjctfile)

	with open('result.txt', 'w') as out:
		for snp in result:
			out.write(snp)

	print('number of SNPs found:', len(result))


