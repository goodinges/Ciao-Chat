print('****************************************************************************');
print('SkypeKit Python Wrapper Tutorial: Picking Up Incoming Calls');
print('****************************************************************************');

# This example demonstrates, how to:
# 1. Pick up incoming voice calls

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

if len(sys.argv) != 3:
	print('Usage: python autoanswer.py <skypename> <password>');
	sys.exit();

accountName = sys.argv[1];
accountPsw  = sys.argv[2];
loggedIn	= False;

#----------------------------------------------------------------------------------
# Defining our own Account property change callback and assigning it to the
# Skype.Account class.

def AccountOnChange (self, property_name):
	global loggedIn;
	if property_name == 'status':
		if self.status == 'LOGGED_IN':
			loggedIn = True;
			print('Login complete.');

Skype.Account.OnPropertyChange = AccountOnChange;

#----------------------------------------------------------------------------------
# The first thing to do here is actually related to our upcoming Skype class.
# We will need some way to determine whether we already have a live conversation
# up in the system. We also need to have a way to access that live conversation.
# The most reasonable place for those is the Skype module, so we can reference
# them later as MySkype.isLiveSessionUp and MySkype.liveSession (Conversation object).
#
# The main reason we need the liveSession variable is that we want to keep getting
# OnPropertyChange events for this object even when the conversation goes non-live.

Skype.Skype.isLiveSessionUp = False;
Skype.Skype.liveSession = 0;

#----------------------------------------------------------------------------------
# The next thing will be to add our own Conversation method for picking up calls.
# We will be picking up calls from two separate places:
# 1. Conversation.OnPropertyChange when local_livestatus goes 'RINGING_FOR_ME'
# 2. SkyLib.OnConversationListChange when local_livestatus goes 'RINGING_FOR_ME'
# So it makes sense to have the code surrounding the standard Conversation.JoinLiveSession
# all in one place.

def PickUpCall(self):
	participants = self.GetParticipants('OTHER_CONSUMERS');
	partListStr = '';
	for p in participants:
		partListStr = partListStr + ' ' + p.identity;
	
	print('Incoming call from: ' + partListStr);
	
	if MySkype.isLiveSessionUp:
		print('We already have a live session. Cannot have two at the same time.');
	else:
		self.JoinLiveSession('');
		MySkype.liveSession = self;
		
Skype.Conversation.PickUpCall = PickUpCall;

#----------------------------------------------------------------------------------
# Conversation.OnPropertyChange event will only fire when the corresponding
# Conversation object is already created. This means that we will not get
# this event when the object is created as a -result- of a property change.
# The -first- Conversation.local_livestatus == 'RINGING_FOR_ME' will -not-
# fire for us, because we only get the conversation object as a result of this
# property change, in Skype.OnConversationListChange callback.

def ConversationOnChange (self, property_name):

	if property_name == 'local_livestatus':

		if self.local_livestatus == 'RINGING_FOR_ME':
			print(self.displayname + ' RING! RING! (from Conversation.OnPropertyChange)');
			self.PickUpCall();
	
		if self.local_livestatus == 'IM_LIVE':
			MySkype.isLiveSessionUp = True;
			MySkype.liveSession = self;
			print(self.displayname + ' is now live.');

		if self.local_livestatus == 'RECENTLY_LIVE':
			MySkype.isLiveSessionUp = False;
			print(self.displayname + ' is no longer live.');

Skype.Conversation.OnPropertyChange = ConversationOnChange;

#----------------------------------------------------------------------------------
# Skype.Skype (basically our Skype class)
# Now, Skype.OnConversationListChange -will- fire on the first incoming call, 
# as we already have a Skype object and OnConversationListChange is a method of that
# class. So the first incoming call in any given conversation we can pick up here,
# when the local_livestatus goes 'RINGING_FOR_ME'.
#
# The problem here is that if the call drops and immediately after that, another call 
# comes in from the same conversation - the OnConversationListChange event will not 
# fire again. The reason is that conversations remain in the LIVE_CONVERSATIONS list
# for several seconds after the call goes down. Solution is to keep the conversation
# object somewhere (in our case the SkyLib.liveSession variable) so that OnPropertyChange
# callback will fire, when the second call comes. Then we can pick the call up from there.

def SkypeOnConversationListChange (self, conversation, type, added):

	if type == 'LIVE_CONVERSATIONS':

		if conversation.local_livestatus == 'RINGING_FOR_ME':
			print(conversation.displayname + ' RING! RING! (from Skype.OnConversationListChange).');
			conversation.PickUpCall();
			
		if added == True:
			MySkype.liveSession = conversation;
			print(conversation.displayname + ' added to live list.');

		if added == False:
			MySkype.liveSession = 0;
			print(conversation.displayname + ' is no longer in live list.');

Skype.Skype.OnConversationListChange = SkypeOnConversationListChange;

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

print('Now accepting incoming calls..');	
print('Press ENTER to exit');
raw_input('');

if MySkype.liveSession != 0:
	# Leaving the live session before calling Skype.Stop() is mandatory.
	# Otherwise the runtime will exit with an error.
	print('There is a live conversation up, dropping it before exit.');	
	MySkype.liveSession.LeaveLiveSession();

print('Exiting..');
MySkype.stop();
