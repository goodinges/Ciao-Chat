print('****************************************************************************');
print('SkypeKit Python Wrapper Tutorial: Making outgoing (conference) calls');
print('****************************************************************************');

# This example demonstrates, how to:
# 1. Make outgoing (multi-target) calls.
# 2. Catch voice activity events during calls.

# NB! You will need to launch the SkypeKit runtime before running this example.

#----------------------------------------------------------------------------------
# Importing necessary libraries. Note that you will need to set the keyFileName value
# in the keypair.py file.

import sys;
import keypair;
from time import sleep;

sys.path.append(keypair.distroRoot + '/ipc/python');
sys.path.append(keypair.distroRoot + '/interfaces/skype/python');

try:
	import Skype;
except ImportError:
    raise SystemExit('Program requires Skype and skypekit modules');

#----------------------------------------------------------------------------------
# Taking skypename and password arguments from command-line.

if len(sys.argv) < 4:
	print('Usage: python autoanswer.py <skypename> <password> <call target>');
	sys.exit();

accountName = sys.argv[1];
accountPsw  = sys.argv[2];

# Collecting call target list from command-line arguments.
# 3rd and subsequent arguments are our call targets.
callTargets = [];
for i in range(3, len(sys.argv)):
	callTargets.append(sys.argv[i]);
	
loggedIn		= False;
isCallFinished	= False;

#----------------------------------------------------------------------------------
# Defining our own Account property change callback and assigning it to the
# SkyLib.Account class.

def AccountOnChange (self, property_name):
	global loggedIn;
	if property_name == 'status':
		if self.status == 'LOGGED_IN':
			loggedIn = True;
			print('Login complete.');

Skype.Account.OnPropertyChange = AccountOnChange;

Skype.isLiveSessionUp = False;
Skype.liveSession = 0;

#----------------------------------------------------------------------------------
# Defining our own Participant property change callback
# Unlike with incoming calls - we can assume that we already have the Conversation
# object - as we retrieved it ourselves when initiating the call. This makes our life
# much easier. So much so that we can make do without any Conversation callback 
# at all and put all our logic into Participant class.
#
# We can use the self.voice_status == 'SPEAKING' property change to detect when 
# any given participant reached live status and 'VOICE_STOPPED' for detecting when 
# they left the call.
# 
# When the self.voice_status == 'VOICE_STOPPED' fires for ourselves - the call has 
# ended. Note that in case of conference call, the rest of the participants will 
# drop too - when the conference host (us) went non-live.

def ParticipantOnChange (self, property_name):
	if property_name == 'voice_status':

		# It makes sense to only display 'joined call' and 'left call'
		# feedback messages for people other than ourselves.
		if self.identity != accountName:
			if self.voice_status == 'SPEAKING':
				print(self.identity + ' has joined the call.');

			if self.voice_status == 'VOICE_STOPPED':
				print(self.identity + ' has left the call.');

		if (self.identity == accountName) and (self.voice_status == 'VOICE_STOPPED'):
			global isCallFinished;
			isCallFinished = True;

	# This property enables you to have neat voice activity indicators in your UI.
	if property_name == 'sound_level':
		print(self.identity + ' sound level: ' + str(self.sound_level));

Skype.Participant.OnPropertyChange = ParticipantOnChange;
	

#----------------------------------------------------------------------------------
# Creating our main Skype object

try:
	MySkype = Skype.GetSkype(keypair.keyFileName);	
except Exception:
	raise SystemExit('Unable to create Skype instance');

#----------------------------------------------------------------------------------
# Retrieving account and logging in with it. Then waiting in loop.

account = MySkype.GetAccount(accountName);

print('Logging in with ' + accountName);
account.LoginWithPassword(accountPsw, False, False);

while loggedIn == False:
	sleep(1);

liveConversation = MySkype.GetConversationByParticipants(callTargets, True, False);

# We need the Participant list to get Participant property change events firing.
# If we don't have the participant objects - no events for us.
callParticipants = liveConversation.GetParticipants('ALL');

# ..but as we already need to have this list, we might as well use it for feedback.
if len(callParticipants) > 0:
	for P in callParticipants:
		if P.identity != accountName:
			print('Calling ' + P.identity + '..');

isCallFinished = False;
liveConversation.RingOthers(callTargets, False, '');

print('Press ENTER to finish the call and exit.');
raw_input('');

# Checking if the call was already finished (because of other participants dropping
# or whether we will need to drop the call ourselves. Going offline without properly
# shutting down the live session can cause confusion to remote clients.
if not isCallFinished:
	liveConversation.LeaveLiveSession(False);
	while not isCallFinished:
		sleep(1);

print('The call has ended.');
print('Exiting..');
MySkype.stop();
