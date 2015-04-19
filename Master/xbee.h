int action_length=-1;

enum
{
	action_sleep,

	action_get_pan,
	action_get_tilt,
	action_set_pan,
	action_set_tilt,

	action_lcd_clear,
	action_lcd_set_cursor,

	action_lcd_put_char_start,
	action_lcd_put_char,

	action_lcd_print_string_start,
	action_lcd_print_string,

	action_get_motor,
	action_set_motor,

	action_set_motors,
	action_set_motors_start,
	action_set_motors_M1_DIRECTION,
	action_set_motors_M1_SPEED,
	action_set_motors_M2_DIRECTION,
	action_set_motors_M2_SPEED,
	action_set_motors_M3_DIRECTION,
	action_set_motors_M3_SPEED,
	action_set_motors_M4_DIRECTION,
	action_set_motors_M4_SPEED,
	action_set_motors_stop,

	action_ultrasound_ping,

	action_get_network,
	action_send_network_message
}current_state,next_state;


void take_action(char c)
{

	switch(current_state)
	{
		case action_sleep:
				switch(c)
				{
					//send_network_message()
					case 0:
						next_state=action_send_network_message;
					break;

					//lcd_put_char()
					case 33:
						next_state=action_lcd_put_char_start;
					break;


					//lcd_put_string()
					case 34:
						next_state=action_lcd_print_string_start;
					break;

					//set_motors()
					case 10:
						next_state=action_set_motors;
					break;
				}
		break;
	
		case action_lcd_put_char_start:
			next_state=action_lcd_put_char;
		break;

		case action_lcd_print_string_start:
			next_state=action_lcd_print_string;
		break;

		//******************************************
		//set_motors(byte[8])
		case action_set_motors:
			next_state=action_set_motors_start;
		break;

		case action_set_motors_start:
			next_state=action_set_motors_M1_DIRECTION;
		break;

		case action_set_motors_M1_DIRECTION:
			next_state=action_set_motors_M1_SPEED;
		break;

		case action_set_motors_M1_SPEED:
			next_state=action_set_motors_M2_DIRECTION;
		break;

		case action_set_motors_M2_DIRECTION:
			next_state=action_set_motors_M2_SPEED;
		break;

		case action_set_motors_M2_SPEED:
			next_state=action_set_motors_M3_DIRECTION;
		break;

		case action_set_motors_M3_DIRECTION:
			next_state=action_set_motors_M3_SPEED;
		break;

		case action_set_motors_M3_SPEED:
			next_state=action_set_motors_M4_DIRECTION;
		break;
	
		case action_set_motors_M4_DIRECTION:
			next_state=action_set_motors_M4_SPEED;
		break;

		case action_set_motors_M4_SPEED:
			next_state=action_set_motors_stop;
		break;

		default:
			next_state=action_sleep;
		break;
	}
	
	current_state=next_state;

	switch(current_state)
	{
		case action_lcd_print_string:
			lcd_data(c);
		break;

		case action_lcd_put_char:
			lcd_data(c);
			current_state=action_sleep;
		break;
		
		case (action_set_motors_start):
lcd_column=15;
lcd_row=1;
		break;
		case (action_set_motors_M1_DIRECTION):
				if(c=='+')
				{
					M3A_mem=0;
					M3B_mem=1;
				}
				else
				{
					M3A_mem=1;
					M3B_mem=0;
				}
lcd_data(c);
		break;
		case (action_set_motors_M1_SPEED):
				speed_motor3=c;
lcd_data_hex(c);
		break;
		case (action_set_motors_M2_DIRECTION):
				if(c=='+')
				{
					M4A_mem=0;
					M4B_mem=1;
				}
				else
				{
					M4A_mem=1;
					M4B_mem=0;
				}
lcd_data(c);
		break;
		case (action_set_motors_M2_SPEED):
				speed_motor4=c;
lcd_data_hex(c);
		break;
		case (action_set_motors_M3_DIRECTION):
				if(c=='+')
				{
					M1B_mem=1;
					M1A_mem=0;
				}
				else
				{
					M1A_mem=1;
					M1B_mem=0;
				}
lcd_data(c);
		break;
		case (action_set_motors_M3_SPEED):
				speed_motor1=c;
lcd_data_hex(c);
		break;
		case (action_set_motors_M4_DIRECTION):
				if(c=='+')
				{							
					M2B_mem=1;
					M2A_mem=0;
				}
				else
				{
					M2A_mem=1;
					M2B_mem=0;
				}
lcd_data(c);
		break;
		case (action_set_motors_M4_SPEED):
				speed_motor2=c;
lcd_data_hex(c);
		break;

		case (action_set_motors_stop):
			rpi_buffer_start=0;
			rpi_buffer_end=0;
			current_state=action_sleep;
		break;
	}
/*
	switch(c)
	{
		
		case 'W':
			wheel_state=wheel_test;
			update_wheel();
		break;
		case 'S':
			wheel_state=backward;
			update_wheel();
		break;
		
		case 'A':
			wheel_state=left_rotate;
			update_wheel();
		break;
		
		case 'D':
			wheel_state=right_rotate;
			update_wheel();
		break;
		
		case 'X':
			wheel_state=off;
			update_wheel();
		break;


		case 'w':
			if(wheel_state!=forward)
				speed_motor=steps ;
			else if(speed_motor<250)
				speed_motor+=steps ;

			wheel_state=forward;
			update_wheel();
			break;
		case 's':

			if(wheel_state!=backward)
				speed_motor=steps ;
			else if(speed_motor<250)
				speed_motor+=steps ;

			wheel_state=backward;
			update_wheel();
			break;
		case 'a':

			if(wheel_state!=left_rotate)
				speed_motor=steps ;
			else if(speed_motor<250)
				speed_motor+=steps ;

			wheel_state=left_rotate;
			update_wheel();
			break;
		case 'd':
			if(wheel_state!=right_rotate)
				speed_motor=steps ;
			else if(speed_motor<250)
				speed_motor+=steps ;

			wheel_state=right_rotate;
			update_wheel();
			break;
		case 'i':up();
			break;
		case 'k':down();
			break;
		case 'j':left();
			break;
		case 'l':right();
			break;
		case 'x':
			wheel_state=off;
			update_wheel();
			break;

	}
*/
}