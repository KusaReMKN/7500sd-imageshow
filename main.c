#include <conio.h>	/* for inp*(), outp*() */
#include <i86.h>	/* for int86() */

#include <stdio.h>
#include <stdlib.h>

#define CLI()	do __asm { cli }; while (0)
#define STI()	do __asm { sti }; while (0)

/* I/O Addresses */
#define IOP_03C0	0x03C0
#define IOP_03C4	0x03C4
#define IOP_03C6	0x03C6
#define IOP_03C8	0x03C8
#define IOP_03C9	0x03C9
#define IOP_03CE	0x03CE
#define IOP_03D4	0x03D4
#define IOP_03DA	0x03DA
#define IOP_FF0A	0xFF0A

static char far *const VRAM_WIN = (void far *)0xC0000000UL;

void attr_write(unsigned char index, unsigned char value) {
	(void)inp(IOP_03DA);	/* XXX */
	(void)outp(IOP_03C0, index);
	(void)outp(IOP_03C0, value);
}

void attrindex_write(unsigned char value) {
	(void)inp(IOP_03DA);	/* XXX */
	(void)outp(IOP_03C0, value);
}

void vga_init() {  // mode 13h sim
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
	unsigned int i, j,voffset,vseg;
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

	vga_init();
	(void)outp(IOP_03C6, 0xFF);
	(void)outp(IOP_03C8, 0x00);
	for (i = 0; i < 768; i+=3) {
		r = pallete[i] >> 4;
		g = pallete[i+1] >> 4;
		b = pallete[i+2] >> 4;
		(void)outp(IOP_03C9, r);
		(void)outp(IOP_03C9, g);
		(void)outp(IOP_03C9, b);
    }

	STI();

	// more vram window
	(void)outpw(IOP_03C4, 0x0106);
	(void)outpw(IOP_03C4, 0x0108);
	(void)outpw(IOP_03C4, 0x0006);
	(void)outpw(IOP_FF0A, 0x0000);
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		VRAM_WIN[0x0000+i] = pic[i];
	}
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		VRAM_WIN[0x8000+i] = pic[i];
	}

	(void)outpw(0xFF0A, 0x0040);
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		VRAM_WIN[0x0000+i] = pic[i];
	}
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		VRAM_WIN[0x8000+i] = pic[i];
	}

	// more vram window
	(void)outpw(IOP_03C4, 0x0106);
	(void)outpw(IOP_03C4, 0x1108);
	(void)outpw(IOP_03C4, 0x0006);
	(void)outpw(IOP_FF0A, 0x0000);
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		VRAM_WIN[0x0000+i] = pic[i];
	}
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		VRAM_WIN[0x8000+i] = pic[i];
	}
	(void)outpw(0xFF0A, 0x0040);
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		VRAM_WIN[0x0000+i] = pic[i];
	}
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		VRAM_WIN[0x8000+i] = pic[i];
	}

	// more vram window
	(void)outpw(IOP_03C4, 0x0106);
	(void)outpw(IOP_03C4, 0x2108);
	(void)outpw(IOP_03C4, 0x0006);
	(void)outpw(IOP_FF0A, 0x0000);
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		VRAM_WIN[0x0000+i] = pic[i];
	}
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		VRAM_WIN[0x8000+i] = pic[i];
	}

	fclose(img);

	while(1)
          ;
        return 0;
}
