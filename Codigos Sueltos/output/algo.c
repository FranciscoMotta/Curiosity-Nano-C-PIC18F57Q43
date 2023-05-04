#include <stdio.h>
#include <stdint.h>

char hex_code[] = "100400002f8220000e7f24000e01880000000000d3";
char bin_nibble[sizeof(hex_code)];
uint8_t bin_byte[sizeof(hex_code)/2];

void main(void)
{
    uint16_t index_string;
    while(hex_code[index_string] != '\0')
    {
        int16_t index_convert;
        char bin_converted_nibble;
        for(index_convert = 7 ; index_convert >= 0 ; index_convert --)
        {
            if(hex_code[index_string] & (1 << 6))
            {
            	// En caso sean letras: a - b - c - d - e - f
                hex_code[index_string] += 9;
                hex_code[index_string] &= ~0b01100000;
            }
            else
            {
            	// Para números: 0 - 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9
                hex_code[index_string] &= ~0b00110000;
            }
            bin_nibble[index_string] |= ((hex_code[index_string]) & (1 << index_convert));
        }
        printf("0x%02x\n\r", bin_nibble[index_string]);
		index_string++;
    }
    uint16_t pairs_counter;
    for(pairs_counter = 0 ; pairs_counter < sizeof(hex_code) ; pairs_counter += 2)
    {
    	bin_byte[pairs_counter / 2] = (uint8_t)((bin_nibble[pairs_counter] << 4) | bin_nibble[pairs_counter + 1]);
	}
	uint16_t pairs_show;
	printf("Bytes generados: \n\r");
	for(pairs_show = 0 ; pairs_show < sizeof(bin_byte) ; pairs_show++)
	{
		printf("0x%02x ", bin_byte[pairs_show]);
	}
}