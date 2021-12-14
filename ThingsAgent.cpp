#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <memory.h>
#include <sys/time.h>
#include <limits.h>
#include "aws_iot_log.h"
#include "aws_iot_version.h"
#include "aws_iot_shadow_interface.h"
#include "aws_iot_shadow_json_data.h"
#include "aws_iot_config.h"
#include "aws_iot_mqtt_interface.h"
#include <wiringPi.h>
#include <string.h>

void ShadowUpdateStatusCallback(const char *pThingName, ShadowActions_t action, Shadow_Ack_Status_t status,
                const char *pReceivedJsonDocument, void *pContextData) {

        if (status == SHADOW_ACK_TIMEOUT) {
                INFO("Update Timeout--");
        } else if (status == SHADOW_ACK_REJECTED) {
                INFO("Update RejectedXX");
        } else if (status == SHADOW_ACK_ACCEPTED) {
                INFO("Update Accepted !");
        }
}

char certDirectory[PATH_MAX + 1] = "../../certs";
char HostAddress[255] = AWS_IOT_MQTT_HOST;
uint32_t port = AWS_IOT_MQTT_PORT;
uint8_t numPubs = 5;
char SERIAL_DEV[13] = "/dev/ttyUSB0";

void parseInputArgsForConnectParams(int argc, char** argv) {
        int opt;

        while (-1 != (opt = getopt(argc, argv, "h:p:c:n:"))) {
                switch (opt) {
                case 'h':
                        strcpy(HostAddress, optarg);
                        DEBUG("Host %s", optarg);
                        break;
                case 'p':
                        port = atoi(optarg);
                        DEBUG("arg %s", optarg);
                        break;
                case 'c':
                        strcpy(certDirectory, optarg);
                        DEBUG("cert root directory %s", optarg);
                        break;
                case 'n':
                        numPubs = atoi(optarg);
                        DEBUG("num pubs %s", optarg);
                        break;
                case '?':
                        if (optopt == 'c') {
                                ERROR("Option -%c requires an argument.", optopt);
                        } else if (isprint(optopt)) {
                                WARN("Unknown option `-%c'.", optopt);
                        } else {
                                WARN("Unknown option character `\\x%x'.", optopt);
                        }
                        break;
                default:
                        ERROR("ERROR in command line argument parsing");
                        break;
                }
        }

}

#define MAX_LENGTH_OF_UPDATE_JSON_BUFFER 200

int main(int argc, char** argv) {
        int gpsp;               
        int serial_read;        
        
        IoT_Error_t rc = NONE_ERROR;
        int32_t i = 0;

        MQTTClient_t mqttClient;
        aws_iot_mqtt_init(&mqttClient);

        char JsonDocumentBuffer[MAX_LENGTH_OF_UPDATE_JSON_BUFFER];
        size_t sizeOfJsonDocumentBuffer = sizeof(JsonDocumentBuffer) / sizeof(JsonDocumentBuffer[0]);
        char *pJsonStringToUpdate;

        char car_number [] = "wc4443";
        jsonStruct_t car_numberHandler;
        car_numberHandler.cb = NULL;
        car_numberHandler.pKey = "car_number";
        car_numberHandler.pData = &car_number;
        car_numberHandler.type = SHADOW_JSON_STRING;

        jsonStruct_t latiHandler;
        latiHandler.cb = NULL;
        latiHandler.pKey = "latitude";
        latiHandler.type = SHADOW_JSON_STRING;

        jsonStruct_t longiHandler;
        longiHandler.cb = NULL;
        longiHandler.pKey = "longitude";
        longiHandler.type = SHADOW_JSON_STRING;

        char rootCA[PATH_MAX + 1];
        char clientCRT[PATH_MAX + 1];
        char clientKey[PATH_MAX + 1];
        char CurrentWD[PATH_MAX + 1];
        char cafileName[] = AWS_IOT_ROOT_CA_FILENAME;
        char clientCRTName[] = AWS_IOT_CERTIFICATE_FILENAME;
        char clientKeyName[] = AWS_IOT_PRIVATE_KEY_FILENAME;

        parseInputArgsForConnectParams(argc, argv);

        INFO("\nAWS IoT SDK Version(dev) %d.%d.%d-%s\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_TAG);

        int PIN=2;
        wiringPiSetup () ;
        pinMode (PIN, INPUT) ;

        INFO("GPIO Setup using PIN : %d\n", PIN);

        getcwd(CurrentWD, sizeof(CurrentWD));
        sprintf(rootCA, "%s/%s/%s", CurrentWD, certDirectory, cafileName);
        sprintf(clientCRT, "%s/%s/%s", CurrentWD, certDirectory, clientCRTName);
        sprintf(clientKey, "%s/%s/%s", CurrentWD, certDirectory, clientKeyName);
        
        DEBUG("Using rootCA %s", rootCA);
        DEBUG("Using clientCRT %s", clientCRT);
        DEBUG("Using clientKey %s", clientKey);

        ShadowParameters_t sp = ShadowParametersDefault;
        sp.pMyThingName = AWS_IOT_MY_THING_NAME;
        sp.pMqttClientId = AWS_IOT_MQTT_CLIENT_ID;
        sp.pHost = HostAddress;
        sp.port = port;
        sp.pClientCRT = clientCRT;
        sp.pClientKey = clientKey;
        sp.pRootCA = rootCA;

        INFO("Shadow Init");
        rc = aws_iot_shadow_init(&mqttClient);

        INFO("Shadow Connect");
        rc = aws_iot_shadow_connect(&mqttClient, &sp);

        if (NONE_ERROR != rc) {
                ERROR("Shadow Connection Error %d", rc);
        }

        if (NONE_ERROR != rc) {
                ERROR("Shadow Register Delta Error");
        }
        while (NONE_ERROR == rc) {
        if(digitalRead(PIN)){
                char serial_read_char = '\0';
                char gpsmsg[2000] ;
                char lati_read[10];
                char longi_read[11];
                //GPS
                gpsp = serialOpen (SERIAL_DEV,9600);
               while(strcmp(gpsmsg,"$GPRMC")!=0){
                        serial_read = serialGetchar(gpsp);
                        serial_read_char = (char)serial_read;
                        if(serial_read_char!='$'){
                                strncat(gpsmsg,&serial_read_char,1);
                        }else{
                                memset(gpsmsg,0,2000);
                                strncat(gpsmsg,&serial_read_char,1);
                        }
                }
                while((char)serial_read!='*'){
                        serial_read = serialGetchar(gpsp);
                        serial_read_char = (char)serial_read;
                        strncat(gpsmsg,&serial_read_char,1);
                }
                DEBUG("GPS Sentense : %s",gpsmsg);
                char * pch;
                int tokcount = 1;
                pch = strtok (gpsmsg,",");
                while (pch != NULL){
                        pch = strtok (NULL, ",");
                        if(tokcount==3){
                                strcpy(lati_read,pch);
                        }else if(tokcount==5){
                                strcpy(longi_read,pch);
                        }
                        tokcount++;
                }
                DEBUG(" LATITUDE : %s      LONGITUDE : %s",lati_read,longi_read);
                latiHandler.pData = lati_read;
                longiHandler.pData = longi_read;
                rc = aws_iot_shadow_yield(&mqttClient, 200);
                INFO("\n=======================================================================================\n");
                INFO("On Device: Car Number %s", car_number);
                rc = aws_iot_shadow_init_json_document(JsonDocumentBuffer, sizeOfJsonDocumentBuffer);
                if (rc == NONE_ERROR) {
                        rc = aws_iot_shadow_add_reported(JsonDocumentBuffer, sizeOfJsonDocumentBuffer, 3,&car_numberHandler ,&latiHandler, &longiHandler);           $
                        if (rc == NONE_ERROR) {
                                rc = aws_iot_finalize_json_document(JsonDocumentBuffer, sizeOfJsonDocumentBuffer);
                                if (rc == NONE_ERROR) {
                                        INFO("Update Shadow: %s", JsonDocumentBuffer);
                                        rc = aws_iot_shadow_update(&mqttClient, AWS_IOT_MY_THING_NAME, JsonDocumentBuffer, ShadowUpdateStatusCallback,
                                        NULL, 4, true);
                                }
                        }
                }
                INFO("*****************************************************************************************\n");
                sleep(60);
        }
        }

        if (NONE_ERROR != rc) {
                ERROR("An error occurred in the loop %d", rc);
        }

        INFO("Disconnecting");
        rc = aws_iot_shadow_disconnect(&mqttClient);

        if (NONE_ERROR != rc) {
                ERROR("Disconnect error %d", rc);
        }

        return rc;
}
