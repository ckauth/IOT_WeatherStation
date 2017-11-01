#!/bin/bash
DEVICE_CONNECTION_STRING="HostName=xxx;DeviceId=xxx;SharedAccessKey=xxx"
PROJECT="iothub_client_sample_weatherStation"

if [ $# -eq 0 ]
  then
        echo Deploying to Pi
        ADDRESS="xxx.xxx.xxx.xxx"
        USERNAME="xxx"
        PASSWORD="xxx"
        SOURCETREE="/home/pi/azure-iot-sdk-c/iothub_client/samples/"
        BINARYTREE="/home/pi/azure-iot-sdk-c/cmake/iothub_client/samples/"
        
        sshpass -p $PASSWORD ssh $USERNAME@$ADDRESS "rm -r ${SOURCETREE}${PROJECT}; mkdir ${SOURCETREE}${PROJECT}"
        sshpass -p $PASSWORD ssh $USERNAME@$ADDRESS "cd ${SOURCETREE}${PROJECT}; mkdir cloud"
        sshpass -p $PASSWORD ssh $USERNAME@$ADDRESS "cd ${SOURCETREE}${PROJECT}; mkdir device"
        sshpass -p $PASSWORD ssh $USERNAME@$ADDRESS "cd ${SOURCETREE}${PROJECT}; mkdir payload"
        sshpass -p $PASSWORD scp ./main.c ./CMakeLists.txt "$USERNAME@$ADDRESS:$SOURCETREE$PROJECT"
        sshpass -p $PASSWORD scp ./cloud/iothub.c ./cloud/iothub.h "$USERNAME@$ADDRESS:$SOURCETREE$PROJECT/cloud"
        sshpass -p $PASSWORD scp ./device/weatherstation.c ./device/weatherstation.h "$USERNAME@$ADDRESS:$SOURCETREE$PROJECT/device"
        sshpass -p $PASSWORD scp ./payload/bme280.c ./payload/bme280.h ./payload/led.c ./payload/led.h ./payload/payload.c ./payload/payload.h ./payload/photocell.c ./payload/photocell.h ./payload/wiring.c ./payload/wiring.h "$USERNAME@$ADDRESS:$SOURCETREE$PROJECT/payload"
        sshpass -p $PASSWORD ssh $USERNAME@$ADDRESS "cd ./azure-iot-sdk-c/cmake; cmake .. && make"
        sshpass -p $PASSWORD ssh $USERNAME@$ADDRESS "cd ${BINARYTREE}${PROJECT}; sudo ./${PROJECT} '$DEVICE_CONNECTION_STRING'"
    else
        echo Deploying Locally
        rm -r ./cmake
        mkdir ./cmake
        cd ./cmake; cmake .. && make;
        sudo ./${PROJECT} $DEVICE_CONNECTION_STRING
fi
