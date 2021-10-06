#include<stdbool.h>
#include<stddef.h>
#include<stdint.h>

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg){
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color){
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str){
	size_t len = 0;
	while(str[len])
		len++;
	return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void){
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for(size_t y = 0; y < VGA_HEIGHT ; y++){
		for(size_t x = 0; x < VGA_WIDTH; x++){
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ',terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color){
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y){
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll(void){
	for(size_t x = 0; x < VGA_WIDTH; x++){
		terminal_buffer[x] = vga_entry(' ', terminal_color);
	}
	for(size_t y = 1; y < VGA_HEIGHT; y++){
		for(size_t x = 0; x < VGA_WIDTH; x++){
			size_t dest = (y-1) * VGA_WIDTH + x;
			size_t src = y * VGA_WIDTH + x;
			
			terminal_buffer[dest] = vga_entry(terminal_buffer[src], terminal_color);
		}
	}
	for(size_t x = 0; x < VGA_WIDTH; x++){
		const size_t index = (VGA_HEIGHT-1) * VGA_WIDTH + x;
		terminal_buffer[index] = vga_entry(' ',terminal_color);
	}
	terminal_column = 0;
	terminal_row = VGA_HEIGHT -2;
}

void terminal_putchar(char c){
	if(c == '\n'){
		if(++terminal_row == VGA_HEIGHT - 1){
			terminal_scroll();
		}
		else{
			terminal_column = 0;
		}
	}
	
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if(++terminal_column == VGA_WIDTH){
		terminal_column = 0;
		if(++terminal_row == VGA_HEIGHT - 1){
			terminal_scroll();
		}
	}
}

void terminal_write(const char * data, size_t size){
	for(size_t i = 0; i < size; i++){
		terminal_putchar(data[i]);
	}
}

void terminal_writestring(const char* data){
	terminal_write(data, strlen(data));
}

void kernel_main(void){
	terminal_initialize();
//	terminal_writestring("a");
	terminal_writestring("Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.Contrary to popular belief, Lorem Ipsum is not simply random text. It has roots in a piece of classical Latin literature from 45 BC, making it over 2000 years old. Richard McClintock, a Latin professor at Hampden-Sydney College in Virginia, looked up one of the more obscure Latin words, consectetur, from a Lorem Ipsum passage, and going through the cites of the word in classical literature, discovered the undoubtable source. Lorem Ipsum comes from sections 1.10.32 and 1.10.33 of \"de Finibus Bonorum et Malorum\" (The Extremes of Good and Evil) by Cicero, written in 45 BC. This book is a treatise on the theory of ethics, very popular during the Renaissance. The first line of Lorem Ipsum, \"Lorem ipsum dolor sit amet..\", comes from a line in section 1.10.32.t is a long established fact that a reader will be distracted by the readable content of a page when looking at its layout. The point of using Lorem Ipsum is that it has a more-or-less normal distribution of letters, as opposed to using 'Content here, content here', making it look like readable English. Many desktop publishing packages and web page editors now use Lorem Ipsum as their default model text, and a search for 'lorem ipsum' will uncover many web sites still in their infancy. Various versions have evolved over the years, sometimes by accident, sometimes on purpose (injected humour and the like).There are many variations of passages of Lorem Ipsum available, but the majority have suffered alteration in some form, by injected humour, or randomised words which don't look even slightly believable. If you are going to use a passage of Lorem Ipsum, you need to be sure there isn't anything embarrassing hidden in the middle of text. All the Lorem Ipsum generators on the Internet tend to repeat predefined chunks as necessary, making this the first true generator on the Internet. It uses a dictionary of over 200 Latin words, combined with a handful of model sentence structures, to generate Lorem Ipsum which looks reasonable. The generated Lorem Ipsum is therefore always free from repetition, injected humour, or non-characteristic words etc.");

}
