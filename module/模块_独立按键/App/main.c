#include "./Head/stc12c5a60s2.h"
#include "./Head/key.h"
sbit LED0=P2^0;
void main()
{
	while(1)
	{
	   unsigned char keycode=keyscan();
	   if(keycode==EN_KEY0)
	   {
	   	LED0=!LED0;
	   }
	}
}