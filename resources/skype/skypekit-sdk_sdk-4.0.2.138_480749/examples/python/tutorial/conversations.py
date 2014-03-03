print('****************************************************************************');
print('SkypeKit Python Wrapper Tutorial: Retrieving Inbox Conversation List');
print('****************************************************************************');

# This example demonstrates, how to:
# 1. Log in and retrieve a list of conversations.

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
	print('Usage: python conversations.py <skypename> <password>');
	sys.exit();

accountName = sys.argv[1];
accountPsw  = sys.argv[2];
loggedIn	= False;

#----------------------------------------------------------------------------------
# Creating our main Skype object

try:
	MySkype = Skype.GetSkype(keypair.keyFileName);
except Exception:
	raise SystemExit('Unable to create Skype instance');

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

#----------------------------------------------------------------------------------
# Retrieving account and logging in with it.

account = MySkype.GetAccount(accountName);

print('Logging in with ' + accountName);
account.LoginWithPassword(accountPsw, False, False);

while loggedIn == False:
	sleep(1);

	
# In this case will retrieve a list of recently active conversations (inbox)
convList = MySkype.GetConversationList('INBOX_CONVERSATIONS');
# Other possible filter values are:
# 'ALL_CONVERSATIONS'
# 'BOOKMARKED_CONVERSATIONS'
# 'LIVE_CONVERSATIONS'

print('Found ' + str(len(convList)) + ' conversations.');
N = 1;
for c in convList:
	unread = c.unconsumed_suppressed_messages + c.unconsumed_normal_messages + c.unconsumed_elevated_messages;
	print(str(N) + '. ' + c.displayname + '  (type = ' + c.type + ') unread messages: ' + str(unread));
	N = N + 1;

raw_input('Press ENTER to continue');
print('Exiting..');
MySkype.stop();
