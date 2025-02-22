#include <conio.h>	/* for inp*(), outp*() */
#include <i86.h>	/* for int86() */

#include <stdio.h>
#include <stdlib.h>

#define CLI()	do __asm { cli }; while (0)
#define STI()	do __asm { sti }; while (0)

/* I/O Addresses */
#define VGA_ATTR_REG	0x03C0
#define VGA_SEQ_REG	0x03C4
#define VGA_PEL_REG	0x03C6
#define VGA_PALLETE_ADDR_REG	0x03C8
#define VGA_PALLETE_DATA_REG	0x03C9
#define VGA_GR_REG	0x03CE
#define VGA_CRTC_REG	0x03D4
#define VGA_STATUS_REG	0x03DA
#define IOP_FF0A	0xFF0A

static char far *const VRAM_WIN = (void far *)0xC0000000UL;

void attr_write(unsigned char index, unsigned char value) {
	(void)inp(VGA_STATUS_REG);	/* Attribute Reg FF reset */
	(void)outp(VGA_ATTR_REG, index);
	(void)outp(VGA_ATTR_REG, value);
}

void attrindex_write(unsigned char value) {
	(void)inp(VGA_STATUS_REG);	/* Attribute Reg FF reset */
	(void)outp(VGA_ATTR_REG, value);
}

void vga_init() {  // 640x480x256 color
	// attribute registers
	attr_write(0x10, 0x41);
	attrindex_write(0x20);
	// sequencer register
	(void)outpw(VGA_SEQ_REG, 0x0C04);
	// graphics control
	(void)outpw(VGA_GR_REG, 0x4005);
	(void)outpw(VGA_GR_REG, 0x0106);
}

	// attribute registers
	CLI();
	attr_write(16, 0x41);
	attrindex_write(0x20);
	// sequencer register
	(void)outpw(IOP_03C4, 0x0C04);
	(void)outpw(IOP_03C4, 0x0F02);
	// graphics control
	(void)outpw(IOP_03CE, 0x0001);
	(void)outpw(IOP_03CE, 0x4005);
	(void)outpw(IOP_03CE, 0x0106);
	// crt control	
	(void)outpw(IOP_03D4, 0x0011);
}

int main() {
	union REGS regs;
	FILE *pal, *img;
	unsigned int i,j,k,voffset,vseg;
	unsigned char r,g,b;
	unsigned char pallete[768]={0};
	unsigned char __far *pic;
	unsigned char pixel;

	pic = malloc(0x8000);
	if(pic == 0){
		printf("malloc failed");
		return -1;
	}

    pal = fopen("pallete.bin", "rb");
	if(pal==NULL){
		printf("pallete open failed");
		return -1;
	}
	fread(pallete, 1, 768, pal);
    fclose(pal);

	img = fopen("pic.bin", "rb");
	if(img==NULL){
		printf("image open failed");
		return -1;
	}


	// system row erase
	regs.w.ax = 0x1B8A;
	(void)int86(0x91, &regs, &regs);

	CLI();
	vga_init();
	(void)outp(VGA_PEL_REG, 0xFF);
	(void)outp(VGA_PALLETE_ADDR_REG, 0x00);
	for (i = 0; i < 768; i+=3) {
		r = pallete[i] >> 4;
		g = pallete[i+1] >> 4;
		b = pallete[i+2] >> 4;
		(void)outp(VGA_PALLETE_DATA_REG, r);
		(void)outp(VGA_PALLETE_DATA_REG, g);
		(void)outp(VGA_PALLETE_DATA_REG, b);
    }

	STI();

	for (j = 0; j < 3;j++){
	// more vram window
		(void)outpw(VGA_SEQ_REG, 0x0106);
		(void)outpw(VGA_SEQ_REG, 0x0108 | j << 12);
		(void)outpw(VGA_SEQ_REG, 0x0006);
		for (k = 0; k < 4;k++){
			(void)outpw(IOP_FF0A, 0x40 * (k>>1));
			fread(pic, 1, 0x8000, img);
	for (i = 0; i < 0x8000;i++) {
				VRAM_WIN[k*0x8000+i] = pic[i];
	}
		}
	}
	fclose(img);

	while(1)
          ;
        return 0;
}
