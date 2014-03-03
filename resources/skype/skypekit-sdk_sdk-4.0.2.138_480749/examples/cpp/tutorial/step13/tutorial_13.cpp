/****************************************************************************

Getting Started With SkypeKit. Tutorial Application, Step 13.

In this example, we will write a simple SkypeKit-based SMS message sender.

To make things slightly more interesting, let's make the sender capable of
taking multiple phone numbers as targets.

**/

#include <math.h>
#include "skype-embedded_2.h"
#include "keypair.h"
#include "tutorial_common.h"

using namespace Sid;

SEString      myAccountName;
SEString      myAccountPsw;
SEStringList  targetList;

//----------------------------------------------------------------------------
// Interface section

class MySms : public Sms
{
public:
  typedef DRef<MySms, Sms> Ref;
  typedef DRefs<MySms, Sms> Refs;

  bool canSend;       // Set once all targets are either in TARGET_ACCEPTABLE or TARGET_NOT_ROUTABLE state
  bool isSent;        // Set once Sms::P_STATUS goes to SENT_TO_SERVER
  bool hasFailed;     // Set once Sms::P_STATUS goes to Sms::FAILED
  bool isDelivered;   // Set once Sms::P_STATUS goes to Sms::DELIVERED
  
  MySms(unsigned int oid, SERootObject* root) : Sms(oid, root)
  {
    canSend     = false;
    isSent      = false;
    hasFailed   = false;
    isDelivered = false;
  };

  void OnChange(int prop);
  void PrintFailureReason();
};

class MySkype : public Skype
{
public:
  MySkype() : Skype() {};
  Account*    newAccount(int oid) {return new MyAccount(oid, this);}
  Sms*        newSms(int oid) {return new MySms(oid, this);}
};

//----------------------------------------------------------------------------
// Implementation section

void MySms::PrintFailureReason()
{
  Sms::FAILUREREASON problem;
  this->GetPropFailurereason(problem);
  printf("%s\n", (const char*)tostring(problem));
};

void MySms::OnChange(int prop)
{
  // As target statuses for all targets are kept in one Sms propery (P_TARGET_STATUSES),
  // we need to loop through all targets in the list separately.

  if (prop == Sms::P_TARGET_STATUSES)
  {
    bool allTargetsAnalyzed = true;

    for (uint i=0; i < targetList.size(); i++)
    {
      SEString targetNumber;
      targetNumber = targetList[i];

      Sms::TARGETSTATUS targetStatus;
      this->GetTargetStatus(targetNumber, targetStatus);
      SEString statusAsText = tostring(targetStatus);

      printf("%s - %s\n", (const char*)targetNumber, (const char*)statusAsText);

      allTargetsAnalyzed = 
        allTargetsAnalyzed 
        && ((targetStatus == Sms::TARGET_NOT_ROUTABLE) || (targetStatus == Sms::TARGET_ACCEPTABLE));

      if (targetStatus == Sms::TARGET_NOT_ROUTABLE)
      {
        /** At this point, we already know that the SMS delivery will fail - because the target
        status was TARGET_NOT_ROUTABLE, so it looks like there is no reason to even attempt
        sending it. However, at this point we still don't know what was the reason behind
        "no routabilty" - The Sms FAILURE_REASON property will only get set during attempt
        to send it.

        One common cause of failure would be an attempt to send SMS messages from account with
        zero balance. That too would trigger TARGET_NOT_ROUTABLE. But your UI would not know
        this - as while in composing state, the SMS failure reason will remain empty.

        Thus, it actually makes sense -not- to stop the sending attempt here, post the SMS,
        knowing that it will fail, and then retrieve the actual error cause. **/

        printf("WARNING: unroutable target %s!\n", (const char*)targetNumber);
      };
    };
    this->canSend = allTargetsAnalyzed;
    return;
  };

  /** The price property as well, is common to all targets (targets may have different prices).
  The total price we can look at via Sms::GetPropPrice but to get to target specific prices
  we will need to loop through targets and use Sms::GetTargetPrice **/
  if   (prop == Sms::P_PRICE)
  {
    for (uint i=0; i < targetList.size(); i++)
    {

      /** The price information is kept in three properties:
      1. price - integer, i.e. 123
      2. precision - integer, i.e. 3
      3. currency - string - i.e. "EUR"

      To calculate actual price value: price / 10^precision
      I.e. 123 / 10^3 = 0.123 EUR

      Note that the price value for any given target (and the total
      price of the SMS) will only become available once the target
      has reached TARGET_ACCEPTABLE status. Until then the price
      will remain at its default value: -1 **/

      SEString targetNumber;
      targetNumber = targetList[i];

      // Getting target price
      uint price;
      this->GetTargetPrice(targetNumber, price);

      if (price == MAX_UINT)
      {
        printf("SMS price for %s is still unknown!\n", (const char*)targetNumber);
        return;
      };

      uint precision;
      this->GetPropPricePrecision(precision);
      SEString currency;
      this->GetPropPriceCurrency(currency);

      float f = precision;
      float actualPrice = price / pow(10, f);

      printf("SMS Price for %s is set to %2.3f %s\n", 
        (const char*)targetNumber, 
        actualPrice, 
        (const char*)currency);

      // Getting SMS total price (we can reuse currency and precision)
      this->GetPropPrice(price);
      actualPrice = price / pow(10, f);
      printf("Total SMS price is now %2.3f %s\n", 
        actualPrice, 
        (const char*)currency);
    };
    return;
  };

  if (prop == Sms::P_STATUS)
  {
    Sms::STATUS status;
    this->GetPropStatus(status);
    this->isSent        = (status == Sms::SENT_TO_SERVER);
    this->hasFailed     = (status == Sms::FAILED);
    this->isDelivered   = (status == Sms::DELIVERED);
  };

  // Logging class / property / new value
  SEStringList dbg;
  SEString someProp;
  someProp = this->GetProp(prop);
  dbg = this->getPropDebug(prop, someProp);
  printf("%s %s value: %s\n", (const char*)dbg[0], (const char*)dbg[1], (const char*)dbg[2]);
};


//----------------------------------------------------------------------------
// Main

MySkype* skype = 0;

int main(int argc, char * argv[])
{
  printf("*****************************************************************\n");
  printf(" SkypeKit Tutorial, Step 13. - Sending SMS messages.\n");
  printf("*****************************************************************\n");

  if (argc < 4)
  {
    printf("usage: tutorial_13 <skypename> <password> <target> [target2]\n");
    return 0;
  };

  myAccountName = argv[1];
  myAccountPsw = argv[2];

  targetList.append(argv[3]);

  // Second SMS target is optional.
  if (argv[4])
  {
    targetList.append(argv[4]);
  };

  printf("Creating skype ..\n");
  skype = new MySkype();
  printf("Submitting application token..\n");
  getKeyPair ();
  skype->init(keyBuf, inetAddr, portNum, "streamlog.txt");
  skype->start();

  printf("Retrieving account ..\n");
  MyAccount::Ref account;

  if (skype->GetAccount(myAccountName, account))
  {
    printf("Logging in..\n");
    account->LoginWithPassword(myAccountPsw, false, true);
    account->BlockWhileLoggingIn();
    printf("Loggin complete.\n");

    printf("Press ENTER to compose SMS\n");
    getchar();

    // SMS objects are created with Skype::CreateOutgoingSms method.
    MySms::Ref sms;
    skype->CreateOutgoingSms(sms);

    // Sms::SetBody will attempt to set the message body text.
    Sms::SETBODYRESULT setBodyResult;
    uint freeUntilNextChunk;
    SEStringList chunks;
    sms->SetBody("Test message", setBodyResult, chunks, freeUntilNextChunk);

    if (setBodyResult != Sms::BODY_OK)
    {
      printf("Failed to set SMS body property.\n");
    }
    else
    {
      printf("Number of SMS chunks used: %d, free characters until next chunk: %d\n", chunks.size(), freeUntilNextChunk);

      bool targetsSuccess;
      sms->SetTargets(targetList, targetsSuccess);
      if (!targetsSuccess)
      {
        printf("Failed to set SMS targets.\n");
      }
      else
      {
        // canSend gets set from MySms::OnChange
        while (!sms->canSend) { Delay(1); };

        printf("Press ENTER to send SMS\n");
        getchar();

        // While created in global scope, posting Sms objects goes through
        // conversations.

        ConversationRef conv;
        skype->GetConversationByParticipants(targetList, conv, true, false);

        // The body argument in PostSMS is currently unused.
        conv->PostSMS(sms, "unused");

        /** For detecting success, we have two variables we can check:
        sms->isSent - message has been sent to server
        sms->isDelivered - we have confirmation of delivery. **/

        while ( (!sms->isDelivered) && (!sms->hasFailed) ) { Delay(1); };
        if (sms->hasFailed) { sms->PrintFailureReason(); };

        // Printing out corresponding Message object body text
        MessageRef msg;
        sms->GetPropChatmsgId(msg);

        SEString msgBody;
        msg->GetPropBodyXml(msgBody);

        printf("\nCorresponding Message object body text:\n%s\n", (const char*)msgBody);
      };
    };

    printf("Press ENTER to quit.\n");
    getchar();

    /** Printing out remaining account balance. The logic here is the same
    as in case of Sms::P_PRICE, in MySms::OnChange **/
    uint balance, precision;
    SEString currency;
    account->GetPropSkypeoutBalance(balance);
    account->GetPropSkypeoutBalanceCurrency(currency);
    account->GetPropSkypeoutPrecision(precision);
    float f = precision;
    float actualBalance = balance / pow(10, f);
    printf("Remaining balance on this account: %2.3f %s\n", actualBalance, (const char*)currency);

    printf("Logging out..\n");
    account->Logout(false);
    account->BlockWhileLoggingOut();
    printf("Logout complete.\n");
  }
  else
  {
      printf("Account does not exist\n");
  };

  printf("Cleaning up.\n");
  skype->stop();
  delete skype;
  printf("Done.\n");
  return 0;
};
