uint32_t lcd_reg_read(uint32_t reg)
{
	uint8_t read_loop = 0;
	uint32_t data = 0;

	switch (reg){
		case 0x0A: //Read Display Power Mode
		case 0x0B: //Read Display MADCTL
		case 0x0C: //Read Display Pixel Format
		case 0x0D: //Read DIsplay Image Format
		case 0x0E: //Read Display Signal Mode
		case 0x0F: //Read Display Self-Diagnostic Result
		case 0x52: //Read Display Brightness
		case 0x54: //Read CTRL Display
		case 0x56: //Read Content Adaptive Brightness Control
		case 0x5F: //Read CABC Minimum Brightness
		case 0xDA: //Read LCD Module's Manufacture
		case 0xDB: //Read LCD Module's Driver Version
		case 0xDC: //Read LCD Module's Driver ID
			read_loop = 1; break;
		case 0x45: //Get Scanline
		case 0xD2: //NV Memory Status Read
			read_loop = 2; break;
		case 0x04: //Read Display Identification Information
		case 0x09: //Read Display Status
		case 0xD3: //Read ID4 (Static Return: 0x00009341)
			read_loop = 3; break;
		default:   //Undefined Command
			return 0;
	}

	//CAN NOT shift-left more than 32
	if(read_loop >5){
		return 0;
	}

	//Chip Selected
	LCD_CS_CLR;

	//Write the exact command
	lcd_cmd_write(reg);

	//Disable Write
	LCD_WR_SET;
	
	//Switch to DATA
	LCD_DC_SET;
	
	//Prepare a falling edge
	//to let LCD ILI9314 prepare data
	LCD_RD_SET;

	//Dummy read first
	for( _lcd_read_raw() ; read_loop--; )
	{
		data = (data<<8) | _lcd_read_raw();
	}

	//Chip Un-Selected
	LCD_CS_SET;
	
	return data;
}

void  lcd_draw_straight_line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color)
{
	int32_t len = 0;

	//straight line only
	if((x1 != x2) && (y1 == y2))	//Horizontal
	{
		len = x2 - x1;

		if(len > 0){
			for(len +=1; len >0; len--){
				lcd_draw_point(x1++, y1, color);
			}
		} else {
			for(len -=1; len <0; len++){
				lcd_draw_point(x1--, y1, color);
			}
		}
	} else if((x1 == x2) && (y1 != y2))	//Vertical
	{
		len = y2 - y1;

		if(len > 0){
			for(len +=1; len >0; len--){
				lcd_draw_point(x1, y1++, color);
			}
		} else {
			for(len -=1; len <0; len++){
				lcd_draw_point(x1, y1--, color);
			}
		}
	}
}
