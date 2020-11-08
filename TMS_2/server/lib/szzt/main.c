#include "zttap_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int iRet;
    int i=0,j=0;
    //unsigned char cReg;
    
    iRet = adxl345_open();
    if(iRet != 0)
    {
        printf("adxl345_open error.\n");
        exit (-1);   
    }    
    
    //iRet = adxl345_read_id(&cReg);
		//printf("adxl345_read_id cReg =%d \n",cReg);
    adxl345_tap_init();
    //adxl345_tap_clear();
      	 	
    while(1)
    {
    	printf("thresh value = %d\n",i);   
    	adxl345_tap_thresh(i);
    	
    	for(j=0;j<10;j++)
    	{
	    	iRet=adxl345_tap_detect();
	    	printf("adxl345_tap_detect iRet =%d \n",iRet);	
				if(iRet)
				{
	    		adxl345_tap_clear();
	    	}
	    	 sleep(1);
    	}
    	i+=10;
    }

    adxl345_close();
    
    return 0;
}