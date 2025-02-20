#include <stdio.h>
#include <stdlib.h>

void attr_write(unsigned char index, unsigned char value) {
  __asm {
		mov dx,0x3da
		in al,dx
		mov dx,0x3c0
		mov al,index
		out dx,al
		mov dx,0x3c0
		mov al,value
    	out dx,al
  }
}

void attrindex_write(unsigned char value) {
  __asm {
		mov dx,0x3da
		in al,dx
		mov dx,0x3c0
		mov al,value
		out dx,al
  }
}

void vga_init() {  // mode 13h sim
	// attribute registers
	__asm{
		cli
	}
	attr_write(16, 0x41);
	attrindex_write(0x20);
	// sequencer register
	__asm {
		mov dx,0x3c4
		mov ax,0x0c04
		out dx,ax
		mov ax,0x0f02
		out dx,ax
	}
	// graphics control
	__asm {
		mov dx,0x3ce
		mov ax, 0x0001
		out dx, ax
		mov ax, 0x4005
		out dx, ax
		mov ax, 0x0106
		out dx, ax
	}
	// crt control	
	__asm {
		mov dx, 0x3d4 
		mov ax, 0x0011 
		out dx,ax
	}
}

int main() {
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
	__asm {
		mov ax, 0x1B8A 
		int 0x91
	}

	vga_init();
	__asm {
		mov dx,0x3c6
		mov al,0xff
		out dx,al
		mov dx,0x3c8
		mov al,0
		out dx,al
	}
	for (i = 0; i < 768; i+=3) {
		r = pallete[i] >> 4;
		g = pallete[i+1] >> 4;
		b = pallete[i+2] >> 4;
		__asm {
  			mov dx,0x3c9
  			mov al,r
  			out dx,al
  			mov al,g
  			out dx,al
  			mov al,b
  			out dx,al
		}
    }

	__asm{
		sti
	}

	// more vram window
	__asm {
		mov dx,0x3c4
		mov ax,0x0106
		out dx,ax
		mov ax,0x0108
		out dx,ax
		mov ax,0x0006
		out dx,ax
		mov dx,0xff0a
		mov ax,0
		out dx,ax
	}
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		pixel = pic[i];
		__asm {
			mov ax,0xc000
			mov es,ax
			mov ax,i
			mov di,ax
			mov al,pixel
			mov es:[di],al
		}
	}
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		pixel = pic[i];
		__asm {
			mov ax,0xc000
			mov es,ax
			mov ax,i
			add ax,0x8000
			mov di,ax
			mov al,pixel
			mov es:[di],al
		}
	}

	__asm {
		mov dx,0xff0a
		mov ax,0x40
		out dx,ax
	}
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		pixel = pic[i];
		__asm {
			mov ax,0xc000
			mov es,ax
			mov ax,i
			mov di,ax
			mov al,pixel
			mov es:[di],al
		}
	}
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		pixel = pic[i];
		__asm {
			mov ax,0xc000
			mov es,ax
			mov ax,i
			add ax,0x8000
			mov di,ax
			mov al,pixel
			mov es:[di],al
		}
	}

	// more vram window
	__asm {
		mov dx,0x3c4
		mov ax,0x0106
		out dx,ax
		mov ax,0x1108
		out dx,ax
		mov ax,0x0006
		out dx,ax
		mov dx,0xff0a
		mov ax,0
		out dx,ax
	}
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		pixel = pic[i];
		__asm {
			mov ax,0xc000
			mov es,ax
			mov ax,i
			mov di,ax
			mov al,pixel
			mov es:[di],al
		}
	}
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		pixel = pic[i];
		__asm {
			mov ax,0xc000
			mov es,ax
			mov ax,i
			add ax,0x8000
			mov di,ax
			mov al,pixel
			mov es:[di],al
		}
	}
	__asm {
		mov dx,0xff0a
		mov ax,0x40
		out dx,ax
	}
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		pixel = pic[i];
		__asm {
			mov ax,0xc000
			mov es,ax
			mov ax,i
			mov di,ax
			mov al,pixel
			mov es:[di],al
		}
	}
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		pixel = pic[i];
		__asm {
			mov ax,0xc000
			mov es,ax
			mov ax,i
			add ax,0x8000
			mov di,ax
			mov al,pixel
			mov es:[di],al
		}
	}

	// more vram window
	__asm {
		mov dx,0x3c4
		mov ax,0x0106
		out dx,ax
		mov ax,0x2108
		out dx,ax
		mov ax,0x0006
		out dx,ax
		mov dx,0xff0a
		mov ax,0
		out dx,ax
	}
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		pixel = pic[i];
		__asm {
			mov ax,0xc000
			mov es,ax
			mov ax,i
			mov di,ax
			mov al,pixel
			mov es:[di],al
		}
	}
	fread(pic, 1,0x8000, img);
	for (i = 0; i < 0x8000;i++) {
		pixel = pic[i];
		__asm {
			mov ax,0xc000
			mov es,ax
			mov ax,i
			add ax,0x8000
			mov di,ax
			mov al,pixel
			mov es:[di],al
		}
	}

	fclose(img);

	while(1)
          ;
        return 0;
}
