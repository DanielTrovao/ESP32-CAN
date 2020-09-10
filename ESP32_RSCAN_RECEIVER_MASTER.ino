#include <ESP32CAN.h>
#include <CAN_config.h>

CAN_device_t CAN_cfg;

int time_stamp = 5;
void setup() {
    Serial.begin(9600);
    CAN_cfg.speed=CAN_SPEED_500KBPS;
    CAN_cfg.tx_pin_id = GPIO_NUM_5;
    CAN_cfg.rx_pin_id = GPIO_NUM_4;
    CAN_cfg.rx_queue = xQueueCreate(10,sizeof(CAN_frame_t));
    //start CAN Module
    ESP32Can.CANInit();
}

void loop() {
    CAN_frame_t rx_frame;
    long time_begin = micros();
    //receive next CAN frame from queue
    if(xQueueReceive(CAN_cfg.rx_queue,&rx_frame, 3*portTICK_PERIOD_MS)==pdTRUE){
      if(rx_frame.FIR.B.RTR==CAN_RTR)
        int i =1;
        //printf(" RTR from 0x%08x, DLC %d\r\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
      else{
        
        printf("AA %08x %08x %02x ",(time_stamp),(rx_frame.MsgID),(rx_frame.FIR.B.DLC));
        for(int i = 0; i < 8; i++){
          printf(" %02x ",(rx_frame.data.u8[i]));
          //printf("%c\t", (char)rx_frame.data.u8[i]);
        }
        printf(" BB ");
        long time_end = micros(); 
        time_stamp = time_end - time_begin;
        printf("%d",time_stamp);
        printf("\n");
      }
    }
    else
    {
      rx_frame.FIR.B.FF = CAN_frame_std;
      rx_frame.MsgID = 1;
      rx_frame.FIR.B.DLC = 8;
      rx_frame.data.u8[0] = 'h';
      rx_frame.data.u8[1] = 'e';
      rx_frame.data.u8[2] = 'l';
      rx_frame.data.u8[3] = 'l';
      rx_frame.data.u8[4] = 'o';
      rx_frame.data.u8[5] = 'c';
      rx_frame.data.u8[6] = 'a';
      rx_frame.data.u8[7] = 'n';

      
      ESP32Can.CANWriteFrame(&rx_frame);
    }
}
