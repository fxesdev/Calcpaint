#include "base.h"
#include "thefont.h"
#include "light.h"
#include "dark.h"
//hi
void invert_line(const word line)
{
	word i = 0;
	word j = 0;
	word offset = (line<<9);
	for(i = 0; i < 512; i++)
	{
		deref(0xF037) = 0;
		deref(0xF800+i+offset) = (~deref(0xF800+i+offset));
		deref(0xF037) = 4;
		deref(0xF800+i+offset) = (~deref(0xF800+i+offset));
	}
}

void wait(word seconds)
{
	word i = 0;
	word j = 0;
	for(j = 0;j < seconds;j++)
	{
		for(i = 0;i < 150;)
		{
			i++;
			invert_line(-1);
		}
	}
}

void drawbitmap(const byte* ptr, word offset, byte width, byte height,byte color)
{
	word x;
	word y;
	word i = 0;
	word j = 0;
	deref(0xF037) = 0;
	if(color == 1)
	{
		deref(0xF037) = 4;
	}
	for(y = 0; y < offset; y++)
	{
		++j;
		if((j&0x1f) == 0x18)
		{
			j += 0x8;
		}
	}

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			if(color == 2)
			{
				deref(0xF037) = 0;
				deref(0xf800+j) = ptr[i];
				deref(0xF037) = 4;
			}
			deref(0xf800+j) = ptr[i];
			++j;
			if((j&0x1f) == 0x18)
			{
				j += 0x8;
			}
			++i;
		}
		j += 32-width;
		if((j&0x1f) == 0x18)
		{
			j += 0x8;
		}
	}
}

void render(word addr)
{
    word i = 0;
    word j = 0;
    for(i = 0; i < 0x0800; i++)
    {
        j++;
        if((j & 0x001F) == 0x18)
        {
            j+=8;
        }
        *((word *)(0xf800 + j)) = *((word *)(addr + i));
    }
}


word print(const byte* str, byte x, byte y,byte color)
{
	const byte* what = str;
	word curoffset = (word)x+((word)y<<8)+((word)y<<7);
	//derefw(0xB510) = (word)(what);a
	word i = 0;
	while(*what)
	{
		//deref(0xB500+i) = *what;

		drawbitmap(image_raw+((word)(*what)<<4),curoffset,1,16,color);
		++curoffset;
		++what;
		++i;
		//++i;
	}
	return i;
}

enum BUTTON
{
	B_0 = 0xb,
	B_1 = 0x3f,
	B_2 = 0x37,
	B_3 = 0x2f,
	B_4 = 0x3e,
	B_5 = 0x36,
	B_6 = 0x2e,
	B_7 = 0x3d,
	B_8 = 0x35,
	B_9 = 0x2d,

	B_A = 0x3c,
	B_B = 0x34,
	B_C = 0x2c,
	B_D = 0x24,
	B_E = 0x1c,
	B_F = 0x14,

	B_G = 0x3d,
	B_H = 0x35,
	B_I = 0x2d,
	B_J = 0x25,
	B_K = 0x1d,

	B_L = 0x3e,
	B_M = 0x36,
	B_N = 0x2e,
	B_O = 0x26,
	B_P = 0x1e,

	B_Q = 0x3f,
	B_R = 0x37,
	B_S = 0x2f,
	B_T = 0x27,
	B_U = 0x1f,

	B_V = 0xb,
	B_W = 0xc,
	B_X = 0xd,
	B_Y = 0xe,
	B_Z = 0xf,



	BUTTON_COUNT = 0x40
};

enum SPECIAL_CHARS
{
	SP_EXE = '\n',
	SP_TAB = '\t',
	SP_SPACE = ' ',
	SP_BACKSPACE = '\b',

	SP_HOME = 0x2A,
	SP_END = 0x1A,
	SP_YES = 0x3A,
	SP_NO = 0x12,
	SP_OK = 0x21,
	SP_UP = 0x20,
	SP_DOWN = 0x22,
	SP_LEFT = 0x29,
	SP_RIGHT = 0x19,
	SP_PLUS = 0x10,
	SP_MINUS = 0x11,
	SP_ALT = 0x31,
	SP_ABC = 0x30,
	SP_ESC = 0x39,
	SP_SELECTLEFT,
	SP_SELECTRIGHT,
	SP_PASTE,
	SP_COPY,

	SPECIAL_MAX
};

const byte button_to_char[64] = {
	  0,        0,       0,    0,    0,    0,   0,    0,
	  0,        0,       0,  '0',  ' ',  ',', '.', '\n',
SP_PLUS, SP_MINUS,   SP_NO,  ']',  ')',    0,   0,    0,
	  0, SP_RIGHT,  SP_END,  '[',  '(', '\b', '#',  '%',
  SP_UP,    SP_OK, SP_DOWN, '\'',  '/',  '!', '@',  '$',
	  0,  SP_LEFT, SP_HOME,  ';', '\\',  '9', '6',  '3',
 SP_ABC,   SP_ALT,     '*',  '-',  '=',  '8', '5',  '2',
	  0,   SP_ESC,  SP_YES, '\t',  '`',  '7', '4',  '1',
};

const byte button_to_char_abc[64] = {
	0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0, 'V', 'W', 'X', 'Y', 'Z',
	0,   0,   0,   0, 'F',   0,   0,   0,
	0,   0,   0,   0, 'E', 'K', 'P', 'U',
	0,   0,   0,   0, 'D', 'J', 'O', 'T',
	0,   0,   0,   0, 'C', 'I', 'N', 'S',
	0,   0,   0,   0, 'B', 'H', 'M', 'R',
	0,   0,   0,   0, 'A', 'G', 'L', 'Q',
};

const byte button_to_char_alt[64] = {
	0,              0,        0,   0,   0,   0,   0,   0,
	0,              0,        0,   0,   0, '<', '>',   0,
	0,              0, SP_PASTE, '}', '^',   0,   0,   0,
	0, SP_SELECTRIGHT,        0, '{', '*',   0,   0,   0,
	0,              0,        0, '"', '?',   0,   0,   0,
	0,  SP_SELECTLEFT,        0, ':', '|',   0,   0,   0,
	0,              0,      '&', '_', '+',   0,   0,   0,
	0,              0,  SP_COPY,   0, '~',   0,   0,   0,
};

/*word byteToTwoBytes(unsigned char input) {
    byte highNibble = (input >> 4) & 0x0F;
    byte lowNibble = input & 0x0F;

    byte result = 0;

    // Convert the high nibble to its ASCII representation
    byte highChar = highNibble + '0';
    if (highChar > '9') {
        highChar += 7; // Adjust for hexadecimal A-F
    }

    // Convert the low nibble to its ASCII representation
    lowChar = lowNibble + '0';
    if (lowChar > '9') {
        lowChar += 7; // Adjust for hexadecimal A-F
    }

    // Combine the two ASCII characters into a word (16-bit)
    result = (highChar << 8) | lowChar;

    return result;
}
*/
void print_day()
{
	char Monday[] =		 " Monday   ";
	char Tuesday[] = 	 " Tuesday  ";
	char Wednesday[] =	 " Wednesday";
	char Thursday[] = 	 " Thursday ";
	char Friday[] = 	 " Friday   ";
	char Saturday[] =	 " Saturday ";
	char Sunday[] = 	 " Sunday   ";
	byte rtc_day = deref(0xf0c3);
	if(rtc_day == 1)
	{
		print(Monday,7,2,2);
	} else if(rtc_day == 2)
	{
		print(Tuesday,7,2,2);
	} else if(rtc_day == 3)
	{
		print(Wednesday,7,2,2);
	} else if(rtc_day == 4)
	{
		print(Thursday,7,2,2);
	} else if(rtc_day == 5)
	{
		print(Friday,7,2,2);
	} else if(rtc_day == 6)
	{
		print(Saturday,7,2,2);
	} else if(rtc_day == 7)
	{
		print(Sunday,7,2,2);
	}
}

byte menu(const byte* option_a, const byte* option_b, const byte* option_c, const byte* option_d)
{
    word i = 0;
    word j = 0;
    word x = 0;
	for(x = 0;x <2048;x++) {
		deref(0xF037) = 0;
		deref(0xF800+x) = 0;
		deref(0xF037) = 4;
		deref(0xF800+x) = 0;
	}
	print(option_a, 0, 0, 1);
    print(option_b, 0, 1, 1);
    print(option_c, 0, 2, 1);
    print(option_d, 0, 3, 1);
	invert_line(i);
	while(1)
	{
		byte pressedbutton = CheckButtons();
		if(pressedbutton == 0xff)
		{
			continue;
		} else if(pressedbutton == SP_DOWN) {
			i++;
			if(i == 4) {
				i = 0;
			}
			invert_line(j);
		} else if(pressedbutton == SP_UP) {
			i--;
			if(i == -1) {
				i = 3;
			}
			invert_line(j);
		} else if(pressedbutton == SP_EXE || pressedbutton == SP_OK || pressedbutton == SP_YES) {
			/*for(x = 0;x <2048;x++) {
				deref(0xF037) = 0;
				deref(0xF800+x) = 0;
				deref(0xF037) = 4;
				deref(0xF800+x) = 0;
			}*/
			if(i == 0) {
				return 1;
			} else if(i == 1) {
				return 2;
			} else if(i == 2) {
				return 3;
			} else if(i == 3) {
				return 4;
			}
		}
		invert_line(i);
		j = i;

	}


}

byte menu_2(const byte* option_a, const byte* option_b)
{
    word i = 0;
    word j = 0;
    word x = 0;
	for(x = 0;x <2048;x++) {
		deref(0xF037) = 0;
		deref(0xF800+x) = 0;
		deref(0xF037) = 4;
		deref(0xF800+x) = 0;
	}
	print(option_a, 0, 0, 1);
    print(option_b, 0, 1, 1);
	invert_line(i);
	while(1)
	{
		byte pressedbutton = CheckButtons();
		if(pressedbutton == 0xff)
		{
			continue;
		} else if(pressedbutton == SP_DOWN) {
			i++;
			if(i == 2) {
				i = 0;
			}
			invert_line(j);
		} else if(pressedbutton == SP_UP) {
			i--;
			if(i == -1) {
				i = 1;
			}
			invert_line(j);
		} else if(pressedbutton == SP_EXE || pressedbutton == SP_OK || pressedbutton == SP_YES) {
			/*for(x = 0;x <2048;x++) {
				deref(0xF037) = 0;
				deref(0xF800+x) = 0;
				deref(0xF037) = 4;
				deref(0xF800+x) = 0;
			}*/
			if(i == 0) {
				return 1;
			} else if(i == 1) {
				return 2;
			}
		}
		invert_line(i);
		j = i;

	}


}

byte lastbutton = 0xff;
byte CheckButtons()
{
	byte x;
	byte y;
	byte i = 0;
	for(x = 0x80; x != 0; x = x >> 1)
	{
		deref(0xf046) = x;
		for(y = 0x80; y != 0; y = y >> 1)
		{
			if((deref(0xf040) & y) == 0)
			{
				if(i != lastbutton)
				{
					lastbutton = i;
					return i;
				}
				return 0xff;
			}
			++i;
		}
	}
	lastbutton = 0x50;
	return 0xff;
}

void byte_to_nibble(byte cv2)
{
	byte bufa = 0;
	byte bufb = 0;
	bufa = (cv2&0xF0)>>4;
	bufb = cv2&0x0F;
	deref(0xe0e0) = (bufa + 0x30);
	deref(0xe0e1) = (bufb + 0x30);
}

void main()
{
	char option_a[] = "1x1";
	char option_b[] = "2x2";
	char option_c[] = "3x3";
	char option_d[] = "4x4";
	char option_aa[] = "Brush Size";
	char option_ab[] = "Save";
	char option_ac[] = "Load";
	char option_ad[] = "More";
	char option_ba[] = "Slot 1";
	char option_bb[] = "Slot 2";
	char option_bc[] = "Slot 3";
	char option_bd[] = "Slot 4";
	char option_ca[] = "Slide-Show";
	char option_cb[] = "Clock";
	word x = 1;
	word y = 1;
	word i = 0;
	byte brush = 1;
	byte menu_option = 0;
	byte save = 0;
	deref(0xf0c7) = 0x01;
/*
	deref(0xf00a) = 0x81;
	deref(0xf030) = 0x04;
	deref(0xf031) = 0x15;
	deref(0xf032) = 0x12;
	deref(0xf033) = 0x03;
	deref(0xf034) = 0x03;
	deref(0xf035) = 0x17;
	deref(0xf036) = 0x08;
	deref(0xf037) = 0x04;
	deref(0xf039) = 0;

	deref(0xf03d) = 0x07;

	deref(0xF4C6) = 0x50;
	deref(0xF4C7) = 0x88;
	deref(0xF4C8) = 0x01;
	deref(0xF4C9) = 0x49;
	deref(0xF4CA) = 0x88;
	deref(0xF4CB) = 0x12;
	deref(0xF400) = 0xFF;
	deref(0xF48A) = 0x69;
	deref(0xF48B) = 0x01;
	deref(0xF414) = 0x14;
	deref(0xF415) = 0x01;
	deref(0xF221) = 0x7F;
	deref(0xF223) = 0x7F;
	deref(0xF0D0) = 0x03;
	deref(0xF0D1) = 0x06;
	deref(0xF000) = 0x04;
	deref(0xF010) = 0x22;
	deref(0xF018) = 0x03;
	deref(0xF03D) = 0x07;
	deref(0xF04A) = 0x07;
	deref(0xF04C) = 0x07;
	*/
	//very sloppy text editor
	//My emu has alot of junk on screen, clearing both screen buffers helps.
	deref(0xf037) = 0;
	for(i = 0; i < 2048; i++)
	{
		deref(0xf800+i) = 0x00;
	}
	deref(0xf037) = 4;
	for(i = 0; i < 2048; i++)
	{
		deref(0xf800+i) = 0x00;
	}
	render(dark);
	deref(0xF037) = 0;
	render(light);
	wait(2);
	for(i = 0;i <2048;i++)
	{
	    deref(0xF037) = 0;
	    deref(0xF800+i) = deref(0x9800+i);
	    deref(0xF037) = 4;
	    deref(0xF800+i) = deref(0x9000+i);
	}
	while(1)
	{
		byte pressedbutton = CheckButtons();
		if(pressedbutton == 0xff)
		{
			continue;
		}
		if(pressedbutton == SP_DOWN)
		{
			y++;
		} else if(pressedbutton == SP_UP)
		{
			y--;
		} else if(pressedbutton == SP_LEFT)
		{
			x--;
		} else if(pressedbutton == SP_RIGHT)
		{
			x++;
		} else if(pressedbutton == B_1)
		{
			set_pixel(x,y,1,brush);
		} else if(pressedbutton == B_2)
		{
			set_pixel(x,y,2,brush);
		} else if(pressedbutton == B_3)
		{
			set_pixel(x,y,3,brush);
		} else if(pressedbutton == B_0)
		{
			set_pixel(x,y,0,brush);
		} else if(pressedbutton == SP_HOME)
		{
            menu_option = menu(option_aa,option_ab,option_ac,option_ad);
            if(menu_option == 1)
            {
            	brush = menu(option_a,option_b,option_c,option_d);
            } else if(menu_option == 2)
            {
                save = menu(option_ba,option_bb,option_bc,option_bd);
                for(i = 0;i <2048;i++)
                {
                	deref(0x9800 + i + (0x1000 * save)) = deref(0x9000+i);
                	deref(0xA000 + i + (0x1000 * save)) = deref(0x9800+i);
                }
            } else if(menu_option == 3)
            {
                save = menu(option_ba,option_bb,option_bc,option_bd);
                for(i = 0;i <2048;i++)
                {
                	deref(0x9000 + i) = deref(0x9800 + i + (0x1000 * save));
                	deref(0x9800 + i) = deref(0xA000 + i + (0x1000 * save));
                }
            } else if(menu_option == 4)
            {
                save = menu_2(option_ca,option_cb);
                if(save == 2)
                {

                	byte sec = deref(0xf0c0);
                	byte min = deref(0xf0c1);
                	byte hour = deref(0xf0c2);
                	word ad_dr = 0xF0C0;
                	byte numbuf = 0;
                	char printbuf[] = "00";
                	char seperator[] = ":  :";
                	for(i = 0;i <2048;i++)
            		{
                		deref(0xF037) = 0;
                		deref(0xF800+i) = 0;
                		deref(0xF037) = 4;
                		deref(0xF800+i) = 0;
            		}
                	invert_line(2);
                	print(seperator,10,1,1);
                	while(1)
                	{
                		byte pressedbutton = CheckButtons();
                		print_day();
                		byte_to_nibble(deref(0xf0c0));
                		printbuf[0] = deref(0xe0e0);
                		printbuf[1] = deref(0xe0e1);
                		print(printbuf,14,1,2);
                		byte_to_nibble(deref(0xf0c1));
                		printbuf[0] = deref(0xe0e0);
                		printbuf[1] = deref(0xe0e1);
                		print(printbuf,11,1,2);
                		byte_to_nibble(deref(0xf0c2));
                		printbuf[0] = deref(0xe0e0);
                		printbuf[1] = deref(0xe0e1);
                		print(printbuf,8,1,2);


                		if(pressedbutton == 0xFF)
                		{
                			continue;
                		}
                		deref(0xe0e8) = pressedbutton;
                		if(pressedbutton == 0x1D) { //delete
                			break;
                		} else if(pressedbutton == 0x0F) {
                			deref(0xf0c7) = 0;
                			while(1)
                			{
                				deref(0xe8e8) = pressedbutton;
                				pressedbutton = CheckButtons();
                				if(pressedbutton == 0xFF)
                				{
                					continue;
                				} else if(pressedbutton == SP_UP) {
                					if((deref(ad_dr)&0xF)==9)
                					{
                						deref(ad_dr)+=6;
                					}
                					deref(ad_dr)++;
                				} else if(pressedbutton == SP_DOWN) {
                					if((deref(ad_dr)&0xF)==0)
                					{
                						deref(ad_dr)-=6;
                					}

                					deref(ad_dr)--;
                				} else if(pressedbutton == 0x0F) {
                					break;
                				} else if(pressedbutton == SP_RIGHT) {
                        			ad_dr--;
                        			if(ad_dr == 0xf0bf)
                        			{
                        				ad_dr = 0xf0c0;
                        			}
                        		} else if(pressedbutton == SP_LEFT) {
                        			ad_dr++;
                        			if(ad_dr == 0xf0c4)
                        			{
                        				ad_dr = 0xf0c3;
                        			}
                        		}
                				print_day();
                				byte_to_nibble(deref(0xf0c0));
                        		printbuf[0] = deref(0xe0e0);
                        		printbuf[1] = deref(0xe0e1);
                        		print(printbuf,14,1,2);
                        		byte_to_nibble(deref(0xf0c1));
                        		printbuf[0] = deref(0xe0e0);
                        		printbuf[1] = deref(0xe0e1);
                        		print(printbuf,11,1,2);
                        		byte_to_nibble(deref(0xf0c2));
                        		printbuf[0] = deref(0xe0e0);
                        		printbuf[1] = deref(0xe0e1);
                        		print(printbuf,8,1,2);


                			}
                			deref(0xf0c7) = 0x01;
                		}

                	}
                }

            }
            for(i = 0;i <2048;i++)
            {
                deref(0xF037) = 0;
                deref(0xF800+i) = deref(0x9800+i);
                deref(0xF037) = 4;
                deref(0xF800+i) = deref(0x9000+i);
            }
		}
		//PrintWord(tempbuffer,0,1,2);

	}
	/*
	deref(0xf037) = 0;
	for(i = 0; i < 2048; i++)
	{
		deref(0xf800+i) = 0x3c;
	}
	*/
}

void set_pixel(word x, word y, byte color, byte brush) {
    set_pixel_r(x,y,color);
    if(brush == 2)
    {
    	set_pixel_r(x+1,y+1,color);
    	set_pixel_r(x+1,y,color);
    	set_pixel_r(x,y+1,color);

    } else if(brush == 3)
    {
    	set_pixel_r(x+1,y+1,color);
    	set_pixel_r(x+1,y,color);
    	set_pixel_r(x,y+1,color);

    	set_pixel_r(x-1,y-1,color);
    	set_pixel_r(x-1,y,color);
    	set_pixel_r(x,y-1,color);

    	set_pixel_r(x-1,y+1,color);
    	set_pixel_r(x+1,y-1,color);

    } else if(brush == 4)
    {
    	set_pixel_r(x+1,y+1,color);
    	set_pixel_r(x+1,y,color);
    	set_pixel_r(x,y+1,color);

    	set_pixel_r(x-1,y-1,color);
    	set_pixel_r(x-1,y,color);
    	set_pixel_r(x,y-1,color);

    	set_pixel_r(x-1,y+1,color);
    	set_pixel_r(x+1,y-1,color);

    	set_pixel_r(x+2,y-1,color);
    	set_pixel_r(x+2,y,color);
    	set_pixel_r(x+2,y+1,color);
    	set_pixel_r(x+2,y+2,color);

    	set_pixel_r(x-1,y+2,color);
    	set_pixel_r(x,y+2,color);
    	set_pixel_r(x+1,y+2,color);


    }
}

void set_pixel_r(word x, word y, byte color) {
	word byteIndex = (y<<5) + (x >> 3);
	byte bitIndex = x % 8;
	deref(0xF037) = 0;
	deref(byteIndex + 0xF800) &= ~(0x80>>bitIndex);
	deref(byteIndex + 0x9800) &= ~(0x80>>bitIndex);
	deref(0xF037) = 4;
	deref(byteIndex + 0xF800) &= ~(0x80>>bitIndex);
	deref(byteIndex + 0x9000) &= ~(0x80>>bitIndex);
    if(color == 1)
    {
    	deref(0xF037) = 0;
    	deref(byteIndex + 0xF800) |= (0x80>>bitIndex);
    	deref(byteIndex + 0x9800) |= (0x80>>bitIndex);
    }
    if(color > 1)
    {
    	deref(0xF037) = 4;
    	deref(byteIndex + 0xF800) |= (0x80>>bitIndex);
    	deref(byteIndex + 0x9000) |= (0x80>>bitIndex);
    }
    if(color == 3)
    {
        deref(0xF037) = 0;
        deref(byteIndex + 0xF800) |= (0x80>>bitIndex);
        deref(byteIndex + 0x9800) |= (0x80>>bitIndex);
    }
}

byte cabs(byte x) {
    byte mask = x >> (sizeof(byte) * 8 - 1);
    return (x + mask) ^ mask;
}

void custom_break()
{
	word i = 0;
	byte error[] = "BRK TRIGGERED";
	byte message[] = "drawing is saved";
	/*for(i=0;i<2048;i++) {
			deref(0xF037) = 0;
			deref(0xF800+i) = 0;
			deref(0xF037) = 4;
			deref(0xF800+i) = 0;
	}
	for(i=0;i<2048;i++) {
		deref(0xF037) = 0;
		deref(0xF800+i) = (deref(0xF800+i) | deref(0x2000+i));
		deref(0xF037) = 4;
		deref(0xF800+i) = (deref(0xF800+i) | deref(0x2800+i));
	}
	*/
	while(1)
	{
		//draw_line(1,1,192,20);
		print(error,1,1,2);
		print(message,1,2,1);
	}

}
