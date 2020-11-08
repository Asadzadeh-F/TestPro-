#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "vpcard_interface.h"

int main(int argc, char *argv[])
{
	char mag1[256]={0}, mag2[256]={0}, mag3[256]={0};
	char ATR1[128]={0}, ATR2[128]={0}, ATR3[128]={0};
	char card_type[4]={0}, uid[12]={0}, random[32];
	char cup_ver[32]={0}, bank_ver[32]={0}, fact_ver[32]={0};
	int  i, apdu_ln, step, iRet, iRet1, iRet2, iRet3;

	if(argc != 2 )
	{
		printf("usage 606_test step\n"
		       "   step=1  to test magic\n"
		       "   step=2  to test CPU card\n"
		       "   step=3  to test psam1\n"
		       "   step=4  to test psam2\n"
		       "   step=5  to test rfid cpu card\n"
		       "   step=6  to test rfid mem card\n");
		exit(0);
	}
	step = atoi(argv[1]);

	iRet=vpcard_open(38400, "/dev/ttySAC1");
	if(iRet < 0)
	{
		printf("Failed to open the card reader\n");	
	}
	
	vpcard_firmware_version(cup_ver, bank_ver, fact_ver);
	printf("fact_ver=%s\n", fact_ver);
	
	//test card IC and RFID
	if( step == 0 )
	{
		//reset card reader
		iRet = vpcard_reset();
		printf("Please swipe your Magnetic stripe card\n");
		iRet = vpmag_card_check_open(0x03);
		while(1)
		{
			iRet = vpmag_card_get_status();
			if (iRet == 1)
			{
				iRet = vpmag_read3(mag1, mag2, mag3);
				if( iRet == 0 )
				{
					printf("    mag1=%s\n", mag1);
					printf("    mag2=%s\n", mag2);
					printf("    mag3=%s\n", mag3);
				}
			}
			else
			{
				printf("NO MAG CARD\n");	
			}
			
			if (iRet == 2)
			{
				iRet1 = vpcard_contact_poweron(VPCARD_USRCARD1, 0, ATR1);
				if (iRet1 == 0)
				{
					printf("power on success\n");
					printf("User IC card Power Up: ret=%d\n\n", iRet1);
					printf("    ATR1[%d]:%s\n", strlen(ATR1), ATR1);
				}
			}
			else
			{
				printf("NO IC CARD\n");	
			}
			
			usleep(500*1000);
		}
		iRet = vpmag_card_check_close();
	}
	if( step == 1 )
	{
		printf("Please swipe your Magnetic stripe card\n");
		iRet = vpcard_reset();
		while(1)
		{
			//iRet = vpmag_read2(mag2, mag3);
			iRet = vpmag_read3(mag1, mag2, mag3);
			if( iRet == 0 )
			{
				printf("    mag1=%s\n", mag1);
				printf("    mag2=%s\n", mag2);
				printf("    mag3=%s\n", mag3);
			}
			else
				printf("No Card\n");
		}
	}

	//IC Card Test
	if( step == 2 )
	{
		//Power On
		printf("IC Card test\n");
		iRet1 = vpcard_contact_poweron(VPCARD_USRCARD1, 0, ATR1);
		printf("User IC card Power Up: ret=%d\n\n", iRet1);
		printf("    ATR1[%d]:%s\n", strlen(ATR1), ATR1);

		//Get random data
		fprintf(stderr, "Get Random data Test\n");
		iRet = vpcard_contact_apdu(VPCARD_USRCARD1, "\xa0\xa4\x00\x00\x02\x3f\x00",
					7, random, &apdu_ln);
		iRet = vpcard_contact_apdu(VPCARD_USRCARD1, "\xa0\xa4\x00\x00\x02\x2f\xe2",
					7, random, &apdu_ln);
		iRet = vpcard_contact_apdu(VPCARD_USRCARD1, "\xa0\xb0\x00\x00\x0a",
					5, random, &apdu_ln);
		fprintf(stderr, "Random Data:[%d]:", iRet);
		if( iRet == 0 )
		{
			for(i=0; i<(apdu_ln-2); i++)
				printf("%02X ", random[i]&0xff);
			printf("    %02X%02X\n", random[8]&0xff, random[9]&0xff);
		}
		else
		{
			printf("Error:%s\n", random);
		}
		
		//Power off
		printf("IC Card Power Off\n");
		vpcard_contact_poweroff(VPCARD_USRCARD1);
		
	}

	//psam1
	if( step == 3 )
	{	
		//power on
		printf("SAM Card Power Up\n");
		iRet2 = vpcard_psam_poweron(VPCARD_PSAM1, 0, ATR2);
		printf("SAM ATR[%d]:%s\n", strlen(ATR2), ATR2);

		//Get random data
		printf("Get random data from PSAM\n");
		memset(random, 0, sizeof(random));
		apdu_ln = 0;
		iRet = vpcard_psam_apdu(VPCARD_PSAM1, "\xFF\x00\x00\x00\x14",
					5, random, &apdu_ln);
		printf("Random Data[%d]:", iRet);
		if( iRet == 0 )
		{
			for(i=0; i<(apdu_ln-2); i++)
				printf("%02X ", random[i]&0xff);
			printf("    %02X%02X\n", random[8]&0xff, random[9]&0xff);
		}
		else
		{
			printf("Error:%s\n", random);
		}
		
		//power off
		printf("Power Off\n");
		vpcard_psam_poweroff(VPCARD_PSAM1);
	}

	//psam2
	if( step == 4 )
	{		
		//power on
		printf("SAM2 Power Up\n");
		iRet3 = vpcard_psam_poweron(VPCARD_PSAM2, 0, ATR3);
		printf("SAM2 ATR[%d]:%s\n", strlen(ATR3), ATR3);

		//Get random data
		printf("Get Random form PSAM Card\n");
		memset(random, 0, sizeof(random));
		apdu_ln = 0;
		iRet = vpcard_psam_apdu(VPCARD_PSAM2, "\x00\x84\x00\x00\x08",
					5, random, &apdu_ln);
		printf("Random Data:[%d]:", iRet);
		if( iRet == 0 )
		{
			for(i=0; i<(apdu_ln-2); i++)
				printf("%02X ", random[i]&0xff);
			printf("    %02X%02X\n", random[8]&0xff, random[9]&0xff);
		}
		else
		{
			printf("Error:%s\n", random);
		}
		
		//power off
		printf("SAM2 Power Off\n");
		vpcard_psam_poweroff(VPCARD_PSAM2);
	}

	if( step == 5 )
	{
		printf("Please close your RF card to the Anter :\n");
		while(1)
		{
			iRet = vpcard_rfid_active(0, card_type, uid, ATR1);
			if( iRet == 0 )
			{
				printf("RF Card Type:%s\n", card_type);
				printf("uid:%s\n", uid);
				printf("ATR:%s\n", ATR1);
				break;
			}
			if( iRet > 0 ) printf("card sts=%d\n", iRet);
			sleep(1);
		}

		memset(random, 0, sizeof(random));
		fprintf(stderr, "Get Random form RF Card:\n");
		iRet = vpcard_rfid_apdu(VPCARD_RFID1, "\x00\x84\x00\x00\x08",
					5, random, &apdu_ln);
		fprintf(stderr, " Random Data:[%d]:", iRet);
		if( iRet == 0 )
		{
			for(i=0; i<(apdu_ln-2); i++)
				printf("%02X ", random[i]&0xff);
			printf("    %02X%02X\n", random[8]&0xff, random[9]&0xff);
		}
		else
		{
			printf("Error:%s", random);
		}

		printf("RF Card Power Off:\n");
		iRet = vpcard_rfid_halt(0);
		printf("VPCARD_RFID1=%d\n\n", iRet);
	}
	
	if( step == 6 )
	{
		printf("Please put your RFID Tag close to the Anter :\n");
		
		int ret=0;
		char atr[16];
		char uid[16];
		char ct;
		char random[32];
		int value = 3;
	  		
		printf("SLE4442 POWER UP!!\n");
		ret = vpcard_rfmem_active(2000,&ct,atr,uid);
		if(ret<0)
			printf("vpcard_rfmem_active ret=%d\n\n", ret);
		else
		{
			printf("vpcard_rfmem_active ct=%0x\n\n", ct);
	
			printf("PIN AUTH \n");
			memset(random,0xff,6);
			ret = vpcard_rfmem_pinauth(2,0x60,random);
			if(ret<0)
			{
				printf("vpcard_rfmem_pinauth ret=%d\n\n", ret);
			}
			else
				printf("pin auth successful\n");

			//printf("READ DATA FORM ADDR 0 LEN 20\n");
			memset(random,0x88,sizeof(random));
			ret = vpcard_rfmem_write_block(8,random);
			if(ret<0)
				printf("vpcard_ic_write ret=%d\n\n", ret);
			else
			{
				printf("vpcard_ic_write to IC card success!\n");
				for(i=0;i<0x10;i++)
					printf("%02X ",random[i]&0xff);
				printf("\n");
		
			}	
				
			//printf("READ DATA FORM ADDR 0 LEN 20\n");
			memset(random,0x0,sizeof(random));
			ret = vpcard_rfmem_read_block(8,random);
			if(ret<0)
				printf("vpcard_rfmem_read_block ret=%d\n\n", ret);
			else
			{
				printf("vpcard_rfmem_read_block success!\n");
				for(i=0;i<0x10;i++)
					printf("%02X ",random[i]&0xff);
				printf("\n");
			} 


			/* for block 9 the function vpcard_rfmem_write_valueblock just need one time*/
			/* set block 9 to value block */
			ret = vpcard_rfmem_write_valueblock(9, (unsigned char *)&value);
			if(ret < 0)
				printf("vpcard_rfmem_write_valueblock ret = %d\n", ret);
			else
				printf("vpcard_rfmem_write_valueblcok success ! write value is %d\n", value);
			
			ret = vpcard_rfmem_read_valueblock(9, (unsigned char *)&value);
			if(ret < 0)
				printf("vpcard_rfmem_read_valueblock ret = %d\n", ret);
			else
				printf("vpcard_rfmem_read_valueblock success ! read value is %d \n", value);			

			value = 10;
			ret = vpcard_rfmem_increment(9, (unsigned char *)&value);
			if(ret < 0)
				printf("vpcard_rfmem_increment ret = %d\n", ret);
			else
				printf("vpcard_rfmem_increment success !\n add value is %d \n", value); 
				

			ret = vpcard_rfmem_read_valueblock(9, (unsigned char *)&value);
			if(ret < 0)
				printf("vpcard_rfmem_read_valueblock ret = %d\n", ret);
			else
				printf("vpcard_rfmem_read_valueblock success ! read value is %d \n", value);

			value = 8;
			ret = vpcard_rfmem_decrement(9, (unsigned char *)&value);
			if(ret < 0)
				printf("vpcard_rfmem_decrement ret = %d\n", ret);
			else
				printf("vpcard_rfmem_decrement success !\n decrement value is %d \n", value); 

			ret = vpcard_rfmem_read_valueblock(9, (unsigned char *)&value);
			if(ret < 0)
				printf("vpcard_rfmem_read_valueblock ret = %d\n", ret);
			else
				printf("vpcard_rfmem_read_valueblock success ! read value is %d \n", value);

			ret=vpcard_rfmem_halt(0);
			if(ret<0)
			{
				printf("vpcard_rfmem_halt ret=%d\n\n", ret);
			}
			else
				printf("vpcard_rfmem_halt successful\n");
		}
	}

	vpcard_close();

	return 0;
}
