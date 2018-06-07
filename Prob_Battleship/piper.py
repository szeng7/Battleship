#!/usr/bin/env python

# We start two processes and hook together their respective stdin/stdout for
# basic interprocess communication.
#
# The two programs we launch *must* understand the option "-f" or "--first"
# to decide which one goes first. (Earlier versions of piper.py also needed
# a "-d" or "--duel" option to force them to adhere to the protocol for duels,
# but we don't do that anymore.)
#
# Raluca wrote the first version of this, Peter mainly did some simple cleanup
# and refactoring. Actually, in 2008 Peter added random startup delays and
# improved stability quite a bit (at least he hopes that's true :-). In 2015
# Peter revised the code a little for summer session, removing the -d stuff.

import os, sys, select, signal, time, random

MAXFD = 100
MAXMSG = 255

def launch_client(cmd, fdin, fdout):
	cmd = cmd.split()
	pid = os.fork()
	if pid == 0:
		# child
		os.close(0)
		os.close(1)

		if os.dup(fdin) != 0:
			print "cannot duplicate fdin"

		if os.dup(fdout) != 1:
			print "cannot duplicate fdout"

		for i in range(3, MAXFD):
			try:
				os.close(i)
			except:
				pass
		try:
			os.execv(cmd[0], cmd)
		finally:
			print >>sys.stdrr, "failed to execv %s!" % cmd
			os._exit(1)
	else:
		# parent
		print "started %s %d" % (cmd, pid)
		return pid

def main():
	if len(sys.argv) != 3:
		print "Usage: piper <first> <second>"
		sys.exit(1)

	first = sys.argv[1] + " -f"
	second = sys.argv[2] + ""

	toC1read, toC1write = os.pipe()
	fromC1read, fromC1write = os.pipe()

	toC2read, toC2write = os.pipe()
	fromC2read, fromC2write = os.pipe()

	C1 = launch_client(first, toC1read, fromC1write)
	time.sleep(1+random.random()/2) # sleep 1 to 1.5 seconds
	C2 = launch_client(second, toC2read, fromC2write)

	os.close(toC1read)
	os.close(fromC1write)

	os.close(toC2read)
	os.close(fromC2write)

	while True:
		(r, w, e) = select.select([fromC1read, fromC2read], [], [fromC1read, fromC2read], 4)
		try:

			if len(r) == 0 and len(w) == 0 and len(e) == 0:
				print "select timed out, exit"
				break

			if len(e) > 0:
				print "select returned error, exit: %s" % e
				break

			if fromC1read in r:
				str = os.read(fromC1read, MAXMSG)
				if len(str) == 0:
					print "client 1 (%d) stopped" % C1
					break
				print "first->second"
				print str,
				os.write(toC2write, str)

			if fromC2read in r:
				str = os.read(fromC2read, MAXMSG)
				if len(str) == 0:
					print "client 2 (%d) stopped" % C2
					break
				print "second->first"
				print str,
				os.write(toC1write, str)

		except:
			# read or write error, exit
			print "cannot read or write, argh!"
			break

	# while loop over, cleanup
	os.close(fromC1read)
	os.close(fromC2read)
	os.kill(C1, signal.SIGKILL)
	os.kill(C2, signal.SIGKILL)

if __name__ == "__main__":
	main()
