
#include "command-app2app.h"

void CommandApp2app::executeHelp()
{
        printf("\n[APP2APP] - 2\n" \
               "\t2C - create\n" \
               "\t2D - delete\n" \
               "\t2c - connect\n" \
               "\t2d - disconnect\n" \
               "\t2w - write\n" \
               "\t2m - write max bytes\n" \
               "\t2M - write max + 1 bytes\n" \
               "\t2g - datagram\n" \
               "\t2x - max datagram\n" \
               "\t2X - max + 1 datagram\n" \
               "\t2r - read\n" \
               "\t2u - get connectable users\n" \
               "\t2L - get stream list\n" \
               "\n" \
              );
        fflush(stdout);
}

const char* CommandApp2app::stream_types_as_text[] = { "ALL_STREAMS", "SENDING_STREAMS", "RECEIVED_STREAMS", "" };

void CommandApp2app::execute_C()
{
    bool result;
    String appname = ask("enter app name:");
    skype->App2AppCreate(appname, result);
    if (!result) 
        printf("failed to create app2app\n");
}

void CommandApp2app::execute_D()
{
    bool result;
    String appname = ask("enter app name:");
    skype->App2AppDelete(appname, result);
    if (!result) 
        printf("failed to delete app2app\n");
   
}

void CommandApp2app::execute_c()
{
    bool result;
    String appname = ask("enter app name:");
    String skypename = ask("enter skype name:");
    skype->App2AppConnect(appname, skypename, result);
    if (!result) 
        printf("failed to connect\n");
}

void CommandApp2app::execute_d()
{
    bool result;
    String appname = ask("enter app name:");
    String stream = ask("enter stream:");
    skype->App2AppDisconnect(appname, stream, result);
    if (!result) 
        printf("failed to disconnect\n");
}

void CommandApp2app::execute_w()
{
    bool result;
    String appname = ask("enter app name:");
    String stream = ask("enter stream:");
    int cnt = ask_int("count of messages:");
    unsigned char message[] = {'t', 'e', 's', 't', '\0'};
    Binary data(message, sizeof(message));
    for (int i = 0; i < cnt; i++)
    {
        skype->App2AppWrite(appname, stream, data, result);
        if (!result) 
        {
            printf("failed to write\n");
            break;
        }
    }
}

void CommandApp2app::writeTrashData(unsigned int size)
{
    bool result;
    String appname = ask("enter app name:");
    String stream = ask("enter stream:");
    Binary data((unsigned char*)malloc(size), size);
    skype->App2AppWrite(appname, stream, data, result);
    if (!result) 
    {
        printf("failed to write\n");
    }
}

void CommandApp2app::execute_m()
{
    writeTrashData(MAX_WRITE_SIZE);
}

void CommandApp2app::execute_M()
{
    writeTrashData(MAX_WRITE_SIZE + 1);
}

void CommandApp2app::execute_g()
{
    bool result;
    String appname = ask("enter app name:");
    String stream = ask("enter stream name:");
    int cnt = ask_int("count of messages:");
    unsigned char message[] = {'t', 'e', 's', 't', '\0'};
    Binary data(message, sizeof(message));
    for (int i = 0; i < cnt; i++)
    {
        skype->App2AppDatagram(appname, stream, data, result);
        if (!result) 
        {
            printf("failed to send datagram\n");
            break;
        }
    }
}

void CommandApp2app::writeTrashDatagram(unsigned int size)
{
    bool result;
    String appname = ask("enter app name:");
    String stream = ask("enter stream name:");
    Binary data((unsigned char*)malloc(size), size);
    skype->App2AppDatagram(appname, stream, data, result);
    if (!result) 
    {
        printf("failed to send datagram\n");
    }
}

void CommandApp2app::execute_x()
{
    writeTrashDatagram(MAX_DATAGRAM_SIZE);
}

void CommandApp2app::execute_X()
{
    writeTrashDatagram(MAX_DATAGRAM_SIZE + 1);
}

void CommandApp2app::execute_r()
{
    bool result = true;
    String appname = ask("enter app name:");
    String stream = ask("enter stream:");
    Binary data;
    while (result) 
    {
        skype->App2AppRead(appname, stream, result, data);
        if (result) 
        {
            if (data.size() == 0)
                break;
            const char * p = (const char*)data;
            for (unsigned int i = 0; i < data.size(); i++) 
                printf("%d %c\n", i, p[i]);
        }
        else if (!result)
        {
            printf("failed to read\n");
            break;
        }
    }
}

void CommandApp2app::execute_u()
{
    bool result;
    String appname = ask("enter app name:");
    List_String users;
    skype->App2AppGetConnectableUsers(appname, result, users);
    if (!result) 
        printf("failed to get users\n");
    else {

        for (unsigned int i = 0; i < users.size(); i++)
            printf("%d %s\n", i, (const char*)users[i]);
    }

}

void CommandApp2app::execute_L()
{
    bool result;
    List_String streams;
    List_uint receivedSizes;
    
    String appname = ask("enter app name:");
    int listType = ask_list("type stream type", stream_types_as_text);

    skype->App2AppGetStreamsList(appname, (Skype::APP2APP_STREAMS)listType, result, streams, receivedSizes);

    if (!result) 
        printf("failed to get stream list\n");
    else {
        for (unsigned int i = 0; i < streams.size(); i++)
            printf("%d %s\n", i, (const char*)streams[i]);
    }
}

