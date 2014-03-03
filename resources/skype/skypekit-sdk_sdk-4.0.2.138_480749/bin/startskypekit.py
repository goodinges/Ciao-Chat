#!/usr/bin/python
import ConfigParser
import sys
import os
import subprocess
import getopt
import time
import signal

def end_process(process):
	ret = None
	poll_time = 2
	poll_period = 0.5
	while poll_time > 0:
		ret = process.poll() 
		if ret == None:
			time.sleep(poll_period)
			poll_time -= poll_period
		else:
			print "process ", process.pid , " finished without killing"
			break
	if ret == None:
			print "Killing process ", process.pid
			os.kill(process.pid, signal.SIGTERM)
			process.wait()

def help(options):
	print '\nThis script is used to start skypekit process together with voicehost\n' \
	'& videohost processes. Script autodetects needed voicehost & videohost binary,\n' \
	'starts the processes in right order, manages the output and monitors\n' \
	'the death of processes. All this can be done manually of course, the script is\n' \
	'only here to speed things up littlebit\n\n' \
	'Usage example: ./startskypekit.py --skypekit=./linux-i686/linux-x86-skypekit-voicertp-videortp\n' \
	'Other supported options: (look at the script source for further info)'
	for opt_name in options:
		print "--" + opt_name 
	raise SystemExit

def main():

	skypekit_binary = None
	videohost_binary = None
	voicehost_binary = None
	options = ["skypekit=", "videohost=", "voicehost=", "acspath=", "debuglogname=", "kitlog=", "videolog=", "voicelog=", "help", "valgrind", "instance="]
	acspath = None
	debuglogname = None
	kitlog = None
	videolog = None
	voicelog = None
	print_help = None
	valgrind = None
	instance_name = None

	try:
		opts, args = getopt.getopt(sys.argv[1:], "", options)
		for o, a in opts:
			if o == "--skypekit":
				skypekit_binary = a
			if o == "--videohost":
				videohost_binary = a
			if o == "--voicehost":
				videohost_binary = a
			if o == "--acspath":
				acspath = a
			if o == "--debuglogname":
				debuglogname = a
			if o == "--kitlog":
				kitlog = open(a, "w")			
			if o == "--videolog":
				videolog = open(a, "w")
			if o == "--voicelog":
				voicelog = open(a, "w")
			if o == "--help":
				print_help = True
			if o == "--valgrind":
				valgrind = True
			if o == "--instance":
				instance_name = a

	except getopt.GetoptError, err:
		# print help information and exit:
		print str(err) # will print something like "option -a not recognized"
		print_help = True

	
	if print_help == True:
		help(options)
	
	# if binary name given from command line...
	if skypekit_binary != None:

		# if videhost not given, use default 
		if videohost_binary == None:
			if skypekit_binary.count("videortp"):
				videohost_binary =  os.path.dirname(skypekit_binary) + "/videortphost-test"
	
		# if voicehost not given, use default 
		if voicehost_binary == None:
			if skypekit_binary.count("voicertp"):
				voicehost_binary = os.path.dirname(skypekit_binary) + "/voicertphost-loopback"
			elif skypekit_binary.count("voicepcm"):
				voicehost_binary = os.path.dirname(skypekit_binary) + "/voicepcmhost-loopback"
	
	# if no binary name given then try to read binary from kit.cfg
	else:
		if acspath != None:
			os.chdir(acspath)
		
		config = ConfigParser.RawConfigParser()
		config.read('kit.cfg')

		if config.has_option('Running', 'skypekit_binary'):
			skypekit_binary = config.get('Running', 'skypekit_binary')
	
		if config.has_option('Running', 'videohost_binary'):
			videohost_binary = config.get('Running', 'videohost_binary')
			
		if config.has_option('Running', 'voicehost_binary'):	
			voicehost_binary = config.get('Running', 'voicehost_binary')

	if skypekit_binary == None:
		print 'no skypekit binary specified'
		help(options)

	if videohost_binary != None:
		print 'Starting videohost: ' + videohost_binary
		videohost_cmdline = [videohost_binary]
		if instance_name != None:
			videohost_cmdline.extend(['-s', instance_name])
		videohost_proc = subprocess.Popen(videohost_cmdline, stdout=videolog, stderr=subprocess.STDOUT)
		print 'videohost started: ', videohost_proc.pid
	
	if voicehost_binary != None:
		print 'Starting voicehost: ' + voicehost_binary
		voicehost_cmdline = [voicehost_binary]
		if instance_name != None:
			voicehost_cmdline.extend(['-s', instance_name])
		voicehost_proc = subprocess.Popen(voicehost_cmdline, stdout=voicelog, stderr=subprocess.STDOUT)
		print 'voicehost started: ', voicehost_proc.pid

	
	skypekit_cmdline = []
	if valgrind:
		skypekit_cmdline.extend(['valgrind', '--leak-check=full'])
	skype_data_loc = ".Skype"
	if instance_name != None:
		os.system("mkdir " + instance_name)
		skype_data_loc = instance_name + "/.Skype"
	skypekit_cmdline.extend(['./' + skypekit_binary, '-f', skype_data_loc, '-x']);
	if instance_name != None:
		skypekit_cmdline.extend(['-s', instance_name, '-l', instance_name])
	if debuglogname != None:
		skypekit_cmdline.extend(['-d', debuglogname]);

	skypekit_proc = subprocess.Popen(skypekit_cmdline, stdout=kitlog, stderr=subprocess.STDOUT)
	print 'skypekit started: ', skypekit_proc.pid
	
	try: 
		while True:
			time.sleep(1)
			ret = skypekit_proc.poll()

			if ret is not None:
				print('skypekit dead!, exit')
				if videohost_binary != None:
					end_process(videohost_proc)
				if voicehost_binary != None:
					end_process(voicehost_proc)
				break;

			if videohost_binary != None:
				ret = videohost_proc.poll()
				if ret is not None:
					print('Videohost dead!, exit')
					end_process(skypekit_proc)
					if voicehost_binary != None:
						end_process(voicehost_proc)
					break;

			if voicehost_binary != None:
				ret = voicehost_proc.poll()	
				if ret is not None:
					print('Voicehost dead!, exit')
					end_process(skypekit_proc)
					if videohost_binary != None:
						end_process(videohost_proc)
					break;

	except KeyboardInterrupt:
		print 'Ctrl-C'

	print 'waiting skypekit to exit'
	skypekit_proc.wait()
	if kitlog != None:
		kitlog.close()		

	if videohost_binary != None:	
		print 'waiting videohost to exit'
		videohost_proc.wait()
		if videolog != None:
			videolog.close()			

	if voicehost_binary != None:	
		print 'waiting voicehost to exit'
		voicehost_proc.wait()
		if voicelog != None:
			voicelog.close()			
	
if __name__ == "__main__":
	main() 
