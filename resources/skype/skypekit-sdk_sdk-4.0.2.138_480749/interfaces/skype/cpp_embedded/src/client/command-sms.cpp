
#include "command-sms.h"

void CommandSms::executeHelp()
{
        printf("\n[SMS] - S\n"
               "\ts - send\n"
               "\tp - price\n"
               "\tc - char count\n"
               "\tm - message count\n"
              );
}

Sms::Ref CommandSms::create_sms()
{
        Sms::Ref sms;
        if (!skype->CreateOutgoingSms(sms)) {
                printf("can not create sms\n");
                return 0;
        }
        Sms::SETBODYRESULT result;
        List_String chunks;
        unsigned int charsUntilNextChunk;
        if (!sms->SetBody(ask("sms body: "), result, chunks, charsUntilNextChunk)) {
                printf("can not set body\n");
                return 0;
        }
        const char* bodyresults_as_text[] = { "invalid", "truncated", "ok", "last char ignored" };
        printf("body set with result %s and %d left bytes in last chunk\n", bodyresults_as_text[result], charsUntilNextChunk);
        for (int i = 0, e = chunks.size(); i != e; i++)
                printf("CHUNK[%d] %s\n", i, (const char*)chunks[i]);
                
        return sms;
}

void CommandSms::execute_s() // send
{        
        Conversation::Ref conversation;// = skype->get_current_conversation();
        String target = ask("add target number:");
        skype->GetConversationByIdentity(target, conversation);
        if (conversation) {
                Sms::Ref sms;
                bool result = false;
                
                if (!skype->CreateOutgoingSms(sms)) {
                        printf("can not create sms\n");
                        return;
                }
                                                
                Sms::SETBODYRESULT result2;
                List_String chunks;
                unsigned int charsUntilNextChunk;
                if (!sms->SetBody(ask("sms body:"), result2, chunks, charsUntilNextChunk)) {
                        printf("can not set body\n");
                        return;
                }                                
                
                List_String targets; 
                targets.append(target); 
                result = false;
                sms->SetTargets(targets, result);
                if (!result) {
                        printf("can not set target number\n");
                        return;
                }                                  
                               
                if (!conversation->PostSMS(sms, "")) {
                        printf("can not send sms to participants of %s\n", (const char*)conversation->GetStrProp(Conversation::P_META_TOPIC));
                } 
                               
                printf("\nsms sent\n");                
        } else {
                printf("unable to create conversation from identity %s\n", (const char*)target); 
        }                           
}

void CommandSms::execute_p() // price
{
        Sms::Ref sms = create_sms();
        if (!sms) return;

        List_String targets;
        String target = ask("add phone number: ");
        while (target != "") {
                targets.append(target);
                target = ask("add phone number: ");
        }
        bool ok;
        sms->SetTargets(targets, ok);
        if (!ok) {
                printf("some invalid targets\n");
                return;
        }

        printf("sms price = %u\n", sms->GetUintProp(Sms::P_PRICE));
        unsigned p;
        for (int i = 0, e = targets.size(); i != e; i++) {
                sms->GetTargetPrice(targets[i], p);
                printf("	%s = %d\n", (const char*)targets[i], p);
        }
}

void CommandSms::execute_c() // char count
{
        Sms::Ref sms = create_sms();
        if (!sms) return;
        int numchar = 0;
        List_String textChunks;
        unsigned int charsUntilNextChunk;
        sms->GetBodyChunks(textChunks, charsUntilNextChunk);
        for (int i = 0, e = textChunks.size(); i != e; i++) numchar += textChunks[i].size();
        printf("num char = %d (sum of chunks) %d (body string)\n", numchar, (int) sms->GetStrProp(Sms::P_BODY).size());
}

void CommandSms::execute_m() // message count
{
        Sms::Ref sms = create_sms();
        if (!sms) return;
        List_String textChunks;
        unsigned int charsUntilNextChunk;
        sms->GetBodyChunks(textChunks, charsUntilNextChunk);
        for (int i = 0, e = textChunks.size(); i != e; i++)
                printf("CHUNK[%d] %s\n", i, (const char*)textChunks[i]);
}


