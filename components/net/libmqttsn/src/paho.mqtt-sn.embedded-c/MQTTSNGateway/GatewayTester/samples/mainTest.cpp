/****************************************************************************
 * Copyright (c) 2016, Tomoaki Yamaguchi
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 *---------------------------------------------------------------------------
 *
 *   MQTT-SN GATEWAY TEST CLIENT
 *
 *   Supported functions.
 *
 *   void PUBLISH    ( const char* topicName, uint8_t* payload,
 *                     uint16_t len, uint8_t qos, bool retain = false );
 *
 *   void PUBLISH    ( uint16_t topicId, uint8_t* payload,
 *                     uint16_t len, uint8_t qos, bool retain = false );
 *
 *   void SUBSCRIBE  ( const char* topicName, TopicCallback onPublish,
 *                      uint8_t qos );
 *
 *   void UNSUBSCRIBE( const char* topicName );
 *
 *   void DISCONNECT ( uint16_t sleepInSecs );
 *
 *   void DISPLAY( format, .....);    <== instead of printf()
 *
 *
 * Contributors:
 *    Tomoaki Yamaguchi - initial API and implementation
 ***************************************************************************/

#include "LMqttsnClientApp.h"
#include "LMqttsnClient.h"
#include "LScreen.h"

using namespace std;
using namespace linuxAsyncClient;
extern LMqttsnClient* theClient;
extern LScreen* theScreen;

/*------------------------------------------------------
 *    UDP Configuration    (theNetcon)
 *------------------------------------------------------*/
UDPCONF  = {
    "GatewayTestClient", // ClientId
    {225,1,1,1},         // Multicast group IP
    1883,                // Multicast group Port
    20001,               // Local PortNo
};

/*------------------------------------------------------
 *    Client Configuration  (theMqcon)
 *------------------------------------------------------*/
MQTTSNCONF = {
    60,            //KeepAlive [seconds]
    true,          //Clean session
    300,           //Sleep duration [seconds]
    "",            //WillTopic
    "",            //WillMessage
    0,             //WillQos
    false          //WillRetain
};

/*------------------------------------------------------
 *     Define Topics
 *------------------------------------------------------*/
const char* topic1 = "ty4tw/topic1";
const char* topic2 = "ty4tw/topic2";
const char* topic3 = "ty4tw/topic3";


/*------------------------------------------------------
 *       Callback routines for Subscribed Topics
 *------------------------------------------------------*/
int on_Topic01(uint8_t* pload, uint16_t ploadlen)
{
    DISPLAY("\n\nTopic1 recv.\n");
    char c = pload[ploadlen-1];
    pload[ploadlen-1]= 0;   // set null terminator
    DISPLAY("Payload -->%s%c<--\n\n",pload, c);
    return 0;
}

int on_Topic02(uint8_t* pload, uint16_t ploadlen)
{
    DISPLAY("\n\nTopic2 recv.\n");
    pload[ploadlen-1]= 0;   // set null terminator
    DISPLAY("Payload -->%s　<--\n\n",pload);
    return 0;
}

int on_Topic03(uint8_t* pload, uint16_t ploadlen)
{
    DISPLAY("\n\nNew callback recv Topic2\n");
    pload[ploadlen-1]= 0;   // set null terminator
    DISPLAY("Payload -->%s　<--\n\n",pload);
    return 0;
}

/*------------------------------------------------------
 *      A Link list of Callback routines and Topics
 *------------------------------------------------------*/

SUBSCRIBE_LIST = {// e.g. SUB(topic, callback, QoS),
                  SUB(topic1, on_Topic01, 1),
                  END_OF_SUBSCRIBE_LIST
                 };


/*------------------------------------------------------
 *    Test functions
 *------------------------------------------------------*/

void publishTopic1(void)
{
    char payload[300];
    sprintf(payload, "publish \"ty4tw/Topic1\" \n");
    uint8_t qos = 0;
    PUBLISH(topic1,(uint8_t*)payload, strlen(payload), qos);
}

void subscribeTopic2(void)
{
    uint8_t qos = 1;
    SUBSCRIBE(topic2, on_Topic02, qos);
}

void publishTopic2(void)
{
    char payload[300];
    sprintf(payload, "publish \"ty4tw/topic2\" \n");
    uint8_t qos = 0;
    PUBLISH(topic2,(uint8_t*)payload, strlen(payload), qos);
}

void unsubscribe(void)
{
    UNSUBSCRIBE(topic2);
}

void subscribechangeCallback(void)
{
    uint8_t qos = 1;
    SUBSCRIBE(topic2, on_Topic03, qos);
}

void test3(void)
{
    char payload[300];
    sprintf(payload, "TEST3 ");
    uint8_t qos = 0;
    PUBLISH(topic2,(uint8_t*)payload, strlen(payload), qos);
}

void disconnect(void)
{
    DISCONNECT(0);
}

void asleep(void)
{
    DISCONNECT(theMqcon.sleepDuration);
}

/*------------------------------------------------------
 *    A List of Test functions is valid in case of
 *    line 23 of LMqttsnClientApp.h is commented out.
 *    //#define CLIENT_MODE
 *------------------------------------------------------*/

TEST_LIST = {// e.g. TEST( Label, Test),
             TEST("Step1:Publish topic1",     publishTopic1),
             TEST("Step2:Publish topic2",     publishTopic2),
             TEST("Step3:Subscribe topic2",   subscribeTopic2),
             TEST("Step4:Publish topic2",     publishTopic2),
             TEST("Step5:Unsubscribe topic2", unsubscribe),
             TEST("Step6:Publish topic2",     publishTopic2),
             TEST("Step7:subscribe again",    subscribechangeCallback),
             TEST("Step8:Publish topic2",     publishTopic2),
             TEST("Step9:Sleep     ",         asleep),
             TEST("Step10:Publish topic1",    publishTopic1),
             TEST("Step11:Disconnect",        disconnect),
             END_OF_TEST_LIST
            };


/*------------------------------------------------------
 *    List of tasks is valid in case of line23 of
 *    LMqttsnClientApp.h is uncommented.
 *    #define CLIENT_MODE
 *------------------------------------------------------*/
TASK_LIST = {// e.g. TASK( task, executing duration in second),
            TASK(publishTopic1, 4),  // publishTopic1() is executed every 4 seconds
            TASK(publishTopic2, 7),  // publishTopic2() is executed every 7 seconds
             END_OF_TASK_LIST
            };


/*------------------------------------------------------
 *    Initialize function
 *------------------------------------------------------*/
void setup(void)
{

}


/*****************  END OF  PROGRAM ********************/
