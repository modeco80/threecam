#!/bin/env python3

import sys
import os

sys.path.append(os.path.abspath(os.path.dirname(__file__)))

# modules we depend on
from utils.mips import Mips
from utils.pnach import PnachWriter
from utils.elf import ElfWrapper
from utils.ldparse import LdScript

# Yields word chunks from a word-aligned byte array
def yieldWordChunksFromBytes(byteArray: bytes):
	length = len(byteArray)
	if length % 4 != 0:
		raise RuntimeError(f'Core is not word-aligned by {length % 4} bytes. Fix please.')
	counter = 0
	while True:
		if counter >= length:
			break
		yield byteArray[counter:counter+4]
		counter += 4

# The fun begins...
def main():
	# Open the ELF file so we can get at symbols in it.
	# Open the Region-specific LD file so we can grab other symbols we need too
	elf = ElfWrapper(f'bin/ssx_3_cam_{sys.argv[2]}.elf')
	ldScript = LdScript.new(f'../bxapi/ld/{sys.argv[2]}.ld')

	# Write the pnach out.
	pnachAuthor = 'modeco80'
	pnachComment = 'Free camera mod for SSX 3'

	with PnachWriter.file(sys.argv[1]) as pnachWriter:
			with pnachWriter.cheat('SSX 3cam', pnachAuthor, pnachComment) as cheat:
				def jalHook(hookAddress: int, targetAddress: int):
					cheat.setAddress(hookAddress)
					cheat.word(Mips.jal(targetAddress)) # Perform trampoline
					cheat.word(Mips.nop())				# delay slot

				# Poke in static trampoline
				cheat.comment(' Static Trampoline')
				jalHook(ldScript.symbol('ModHookPoint'), elf.symbol('_start'))

				cheat.comment(' Code Blob Begin')
				cheat.setAddress(elf.symbol('_begin'))
				for word in yieldWordChunksFromBytes(elf.makeCore()):
						cheat.word(word)
				cheat.comment(' Code Blob End')

if __name__ == '__main__':
	main()
