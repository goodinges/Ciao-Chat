print('****************************************************************************');
print('SkypeKit Python Wrapper Tutorial: Contacts and Online Status Events');
print('****************************************************************************');

# This example demonstrates, how to:
# 1. Retrieve and display contact list
# 2. Catch contact online appearance events.

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
	print('Usage: python contacts.py <skypename> <password>');
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
# Skype.Account class.

def AccountOnChange (self, property_name):
	global loggedIn;
	if property_name == 'status':
		if self.status == 'LOGGED_IN':
			loggedIn = True;
			print('Login complete.');

Skype.Account.OnPropertyChange = AccountOnChange;

#----------------------------------------------------------------------------------
# Defining our own Contact property change callback and assigning it to the
# SkyLib.Contact class.

def ContactOnPropertyChange(self, property_name):
	if property_name == 'availability':
		print('Online status event: ' + self.displayname + ' is now ' + self.availability);
		
Skype.Contact.OnPropertyChange = ContactOnPropertyChange;

#----------------------------------------------------------------------------------
# Retrieving account and logging in with it.

account = MySkype.GetAccount(accountName);

print('Logging in with ' + accountName);
account.LoginWithPassword(accountPsw, False, False);

while loggedIn == False:
	sleep(1);

#----------------------------------------------------------------------------------
# Retrieving contact list in two steps:
# 1. we need to get a CountactGroup object with MySkype.GetHardwiredContactGroup
# 2. then we need to get a contact list with ContactGroup.GetContacts

skypeContactGroup = MySkype.GetHardwiredContactGroup('SKYPE_BUDDIES');
skypeContacts = skypeContactGroup.GetContacts();

# Note that we are using SKYPE_BUDDIES filter here - which only includes skypename 
# contacts - phone number contacts (PSTN) will not be included. The full list of
# 'Hardwired' filters is:
# 	ALL_KNOWN_CONTACTS
# 	ALL_BUDDIES
# 	SKYPE_BUDDIES
# 	SKYPEOUT_BUDDIES
# 	ONLINE_BUDDIES
# 	UNKNOWN_OR_PENDINGAUTH_BUDDIES
# 	RECENTLY_CONTACTED_CONTACTS
# 	CONTACTS_WAITING_MY_AUTHORIZATION

print('List of Skype contacts for this account: ');
for C in skypeContacts:
	print(C.displayname);

# Note that at this point - immediately after the login - there is not much point
# to display contact online status. Online statuses will update themselves over
# over next couple of seconds. These events we will catch in ContactOnPropertyChange
	
print('Press ENTER to exit');
raw_input('');
print('Exiting..');
MySkype.stop();
