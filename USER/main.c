#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "touch.h" 
//#include "exti.h"

int x_sit_ship=120,y_sit_ship=300;  //坐标记录量
int x_sit_bullets,x_sit_bullets2,x_sit_bullets3,x_sit_bullets4,x_sit_bullets5;
int y_sit_bullets=300,y_sit_bullets2=300,y_sit_bullets3=300,y_sit_bullets4=300,y_sit_bullets5=300;
int x_sit_enemy=120,x_sit_enemy2=90,x_sit_enemy3=150,x_sit_enemy_random,x_sit_enemy_random2,x_sit_enemy_random3;
int y_sit_enemy=10,y_sit_enemy2=10,y_sit_enemy3=10;
int state,state2,state3,state4,state5,state_enemy=1,state_enemy2=1,state_enemy3=1,state_ship=1;
int limits,random_num_position,random_num_speed;
int speed_enemy=1,speed_enemy2=1,speed_enemy3=1,speed_enemy_random,speed_enemy_random2,speed_enemy_random3;
int mark_win,life=10;


////////////////////////////////////////////////////////////////////////////////
//电容触摸屏专有部分
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}
//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  
// Function of picking a random number of enemies' speed
void CC_pick_random_speed()
{
	if(random_num_speed==1)
	{
		speed_enemy_random=1;
		speed_enemy_random2=3;
		speed_enemy_random3=2;
	}
	else
		if(random_num_speed==2)
		{
			speed_enemy_random=2;
			speed_enemy_random2=4;
			speed_enemy_random3=5;
		}
	else
		if(random_num_speed==3)
		{
			speed_enemy_random=3;
			speed_enemy_random2=2;
			speed_enemy_random3=1;
		}
	else
		if(random_num_speed==4)
		{
			speed_enemy_random=4;
			speed_enemy_random2=3;
			speed_enemy_random3=5;
		}
	else
		if(random_num_speed==5)
		{
			speed_enemy_random=5;
			speed_enemy_random2=1;
			speed_enemy_random3=2;
		}
	else
		if(random_num_speed==6)
		{
			speed_enemy_random=2;
			speed_enemy_random2=2;
			speed_enemy_random3=3;
		}
	else
		if(random_num_speed==7)
		{
			speed_enemy_random=4;
			speed_enemy_random2=5;
			speed_enemy_random3=4;
		}
	else
		if(random_num_speed==8)
		{
			speed_enemy_random=3;
			speed_enemy_random2=1;
			speed_enemy_random3=2;
		}
	else
		if(random_num_speed==9)
		{
			speed_enemy_random=3;
			speed_enemy_random2=3;
			speed_enemy_random3=1;
		}
	else
		if(random_num_speed==10)
		{
			speed_enemy_random=1;
			speed_enemy_random2=2;
			speed_enemy_random3=4;
		}
}

// Function of picking a random number of enemies' position
void CC_pick_random_position()
{
	if(random_num_position==1)
	{
		x_sit_enemy_random=20;
		x_sit_enemy_random2=80;
		x_sit_enemy_random3=40;
	}
	else
		if(random_num_position==2)
		{
			x_sit_enemy_random=40;
			x_sit_enemy_random2=20;
			x_sit_enemy_random3=200;
		}
	else
		if(random_num_position==3)
		{
			x_sit_enemy_random=60;
			x_sit_enemy_random2=160;
			x_sit_enemy_random3=120;
		}
	else
		if(random_num_position==4)
		{
			x_sit_enemy_random=80;
			x_sit_enemy_random2=200;
			x_sit_enemy_random3=20;
		}
	else
		if(random_num_position==5)
		{
			x_sit_enemy_random=100;
			x_sit_enemy_random2=40;
			x_sit_enemy_random3=60;
		}
	else
		if(random_num_position==6)
		{
			x_sit_enemy_random=120;
			x_sit_enemy_random2=140;
			x_sit_enemy_random3=180;
		}
	else
		if(random_num_position==7)
		{
			x_sit_enemy_random=140;
			x_sit_enemy_random2=60;
			x_sit_enemy_random3=160;
		}
	else
		if(random_num_position==8)
		{
			x_sit_enemy_random=160;
			x_sit_enemy_random2=100;
			x_sit_enemy_random3=80;
		}
	else
		if(random_num_position==9)
		{
			x_sit_enemy_random=180;
			x_sit_enemy_random2=120;
			x_sit_enemy_random3=100;
		}
	else
		if(random_num_position==10)
		{
			x_sit_enemy_random=200;
      x_sit_enemy_random2=180;			
			x_sit_enemy_random3=140;
		}			
}

//Function for game table
void CC_GameTable()
{
	  LCD_Clear(BLACK);  
	  // tip of ammo,mark and life
	  POINT_COLOR=BLUE;
	  LCD_ShowString(180,22,200,16,16,"MARK:");
	  LCD_ShowNum(220,22,mark_win,2,16);
	  LCD_ShowString(180,36,200,16,16,"LIFE:");
	  LCD_ShowNum(220,36,life,2,16);
	  if(limits==0)
		{
			POINT_COLOR=BLUE;
			LCD_ShowString(180,8,200,16,16,"AMMO:5");
		}
		else
			if(limits==1)
			{
				POINT_COLOR=BLUE;
			  LCD_ShowString(180,8,200,16,16,"AMMO:4");
			}
			else
				if(limits==2)
				{
					POINT_COLOR=BLUE;
					LCD_ShowString(180,8,200,16,16,"AMMO:3");
				}
			else
				if(limits==3)
				{
					POINT_COLOR=BLUE;
					LCD_ShowString(180,8,200,16,16,"AMMO:2");
				}
			else
				if(limits==4)
				{
					POINT_COLOR=BLUE;
					LCD_ShowString(180,8,200,16,16,"AMMO:1");
				}
			else
				if(limits==5)
				{
					POINT_COLOR=BLUE;
					LCD_ShowString(168,8,200,16,16,"Reloading");
				}
				else
				{
					POINT_COLOR=BLUE;
					LCD_ShowString(168,8,200,16,16,"Reloading");
				}
	
	  // draw the ship
	  if(state_ship)
		{
			gui_fill_circle(x_sit_ship,y_sit_ship,8,GREEN); 
			gui_fill_circle(x_sit_ship,y_sit_ship+6,10,GREEN);	
		}
	  // draw enemies
	  if(state_enemy)
		{
			LCD_Fill(x_sit_enemy,y_sit_enemy,x_sit_enemy+16,y_sit_enemy+8,RED);
			gui_fill_circle(x_sit_enemy+8,y_sit_enemy+8,5,RED);
			y_sit_enemy = y_sit_enemy+speed_enemy;
			if(y_sit_enemy>300)
			{
				state_enemy = 0;
				CC_pick_random_position();
				CC_pick_random_speed();
				x_sit_enemy = x_sit_enemy_random;
				y_sit_enemy = 10;
				speed_enemy = speed_enemy_random;
				life--;
				if(life<=0)
					life = 0;
			}
		}
		if(state_enemy2)
		{
			LCD_Fill(x_sit_enemy2,y_sit_enemy2,x_sit_enemy2+16,y_sit_enemy2+8,RED);
			gui_fill_circle(x_sit_enemy2+8,y_sit_enemy2+8,5,RED);
			y_sit_enemy2 = y_sit_enemy2+speed_enemy2;
			if(y_sit_enemy2>300)
			{
				state_enemy2 = 0;
				CC_pick_random_position();
				CC_pick_random_speed();
				x_sit_enemy2 = x_sit_enemy_random2;
				y_sit_enemy2 = 10;
				speed_enemy2 = speed_enemy_random2;
				life--;
				if(life<=0)
					life = 0;
			}
		}
		if(state_enemy3)
		{
			LCD_Fill(x_sit_enemy3,y_sit_enemy3,x_sit_enemy3+16,y_sit_enemy3+8,RED);
			gui_fill_circle(x_sit_enemy3+8,y_sit_enemy3+8,5,RED);
			y_sit_enemy3 = y_sit_enemy3+speed_enemy3;
			if(y_sit_enemy3>300)
			{
				state_enemy3 = 0;
				CC_pick_random_position();
				CC_pick_random_speed();
				x_sit_enemy3 = x_sit_enemy_random3;
				y_sit_enemy3 = 10;
				speed_enemy3 = speed_enemy_random3;
				life--;
				if(life<=0)
					life = 0;
			}
		}
	  //draw bullets
	  if(state)
		{
			POINT_COLOR=YELLOW;
			LCD_DrawLine(x_sit_bullets,y_sit_bullets,x_sit_bullets,y_sit_bullets-2);
			y_sit_bullets = y_sit_bullets-4;
			if(y_sit_bullets<10)
			{
				state = 0;
				y_sit_bullets = 300;
			}	
		}	
		if(state2)
		{
			POINT_COLOR=YELLOW;
			LCD_DrawLine(x_sit_bullets2,y_sit_bullets2,x_sit_bullets2,y_sit_bullets2-2);
			y_sit_bullets2 = y_sit_bullets2-4;
			if(y_sit_bullets2<10)
			{
				state2 = 0;
				y_sit_bullets2 = 300;
			}	
		}
		if(state3)
		{
			POINT_COLOR=YELLOW;
			LCD_DrawLine(x_sit_bullets3,y_sit_bullets3,x_sit_bullets3,y_sit_bullets3-2);
			y_sit_bullets3 = y_sit_bullets3-4;
			if(y_sit_bullets3<10)
			{
				state3 = 0;
				y_sit_bullets3 = 300;
			}	
		}
		if(state4)
		{
			POINT_COLOR=YELLOW;
			LCD_DrawLine(x_sit_bullets4,y_sit_bullets4,x_sit_bullets4,y_sit_bullets4-2);
			y_sit_bullets4 = y_sit_bullets4-4;
			if(y_sit_bullets4<10)
			{
				state4 = 0;
				y_sit_bullets4 = 300;
			}	
		}		
		if(state5)
		{
			POINT_COLOR=YELLOW;
			LCD_DrawLine(x_sit_bullets5,y_sit_bullets5,x_sit_bullets5,y_sit_bullets5-2);
			y_sit_bullets5 = y_sit_bullets5-4;
			if(y_sit_bullets5<10)
			{
				state5 = 0;
				limits = 0;
				y_sit_bullets5 = 300;
			}	
		}
		//draw explosion
		if(!state_enemy)
		{
			gui_fill_circle(x_sit_enemy+8,y_sit_enemy+4,1,RED);
			POINT_COLOR=YELLOW;
			LCD_DrawLine(x_sit_enemy+7,y_sit_enemy+3,x_sit_enemy+5,y_sit_enemy+1);
			LCD_DrawLine(x_sit_enemy+7,y_sit_enemy+5,x_sit_enemy+5,y_sit_enemy+7);
			LCD_DrawLine(x_sit_enemy+9,y_sit_enemy+3,x_sit_enemy+11,y_sit_enemy+1);
			LCD_DrawLine(x_sit_enemy+9,y_sit_enemy+5,x_sit_enemy+11,y_sit_enemy+7);
			
			//explosion sound(beep)
			
			GPIO_SetBits(GPIOF,GPIO_Pin_8);
		  delay_ms(5);
		  GPIO_ResetBits(GPIOF,GPIO_Pin_8);
			
			
			delay_ms(100);
			state_enemy = 1;
			x_sit_enemy = x_sit_enemy_random;
			y_sit_enemy = 10;
			speed_enemy = speed_enemy_random;
		}
		if(!state_enemy2)
		{
			gui_fill_circle(x_sit_enemy2+8,y_sit_enemy2+4,1,RED);
			POINT_COLOR=YELLOW;
			LCD_DrawLine(x_sit_enemy2+7,y_sit_enemy2+3,x_sit_enemy2+5,y_sit_enemy2+1);
			LCD_DrawLine(x_sit_enemy2+7,y_sit_enemy2+5,x_sit_enemy2+5,y_sit_enemy2+7);
			LCD_DrawLine(x_sit_enemy2+9,y_sit_enemy2+3,x_sit_enemy2+11,y_sit_enemy2+1);
			LCD_DrawLine(x_sit_enemy2+9,y_sit_enemy2+5,x_sit_enemy2+11,y_sit_enemy2+7);
			
			//explosion sound(beep)
			
			GPIO_SetBits(GPIOF,GPIO_Pin_8);
		  delay_ms(5);
		  GPIO_ResetBits(GPIOF,GPIO_Pin_8);
			
			
			delay_ms(100);
			state_enemy2 = 1;
			x_sit_enemy2 = x_sit_enemy_random2;
			y_sit_enemy2 = 10;
			speed_enemy2 = speed_enemy_random2;
		}
		if(!state_enemy3)
		{
			gui_fill_circle(x_sit_enemy3+8,y_sit_enemy3+4,1,RED);
			POINT_COLOR=YELLOW;
			LCD_DrawLine(x_sit_enemy3+7,y_sit_enemy3+3,x_sit_enemy3+5,y_sit_enemy3+1);
			LCD_DrawLine(x_sit_enemy3+7,y_sit_enemy3+5,x_sit_enemy3+5,y_sit_enemy3+7);
			LCD_DrawLine(x_sit_enemy3+9,y_sit_enemy3+3,x_sit_enemy3+11,y_sit_enemy3+1);
			LCD_DrawLine(x_sit_enemy3+9,y_sit_enemy3+5,x_sit_enemy3+11,y_sit_enemy3+7);
			
			//explosion sound(beep)
			
			GPIO_SetBits(GPIOF,GPIO_Pin_8);
		  delay_ms(5);
		  GPIO_ResetBits(GPIOF,GPIO_Pin_8);
			
			
			delay_ms(100);
			state_enemy3 = 1;
			x_sit_enemy3 = x_sit_enemy_random3;
			y_sit_enemy3 = 10;
			speed_enemy3 = speed_enemy_random3;
		}
}

// function for judgement if bullets hit enemies
void CC_Judgement()
{
	// judgement for bullets and enemy1
	if(x_sit_bullets>=x_sit_enemy&&x_sit_bullets<=x_sit_enemy+16&&y_sit_bullets<=y_sit_enemy)
	{
		state_enemy = 0;
		state = 0;
		x_sit_bullets = 0;
		y_sit_bullets=300;
		mark_win++;
	}
	if(x_sit_bullets2>=x_sit_enemy&&x_sit_bullets2<=x_sit_enemy+16&&y_sit_bullets2<=y_sit_enemy)
	{
		state_enemy = 0;
		state2 = 0;
		x_sit_bullets2 = 0;
		y_sit_bullets2=300;
		mark_win++;
	}
	if(x_sit_bullets3>=x_sit_enemy&&x_sit_bullets3<=x_sit_enemy+16&&y_sit_bullets3<=y_sit_enemy)
	{
		state_enemy = 0;
		state3 = 0;
		x_sit_bullets3 = 0;
		y_sit_bullets3=300;
		mark_win++;
	}
	if(x_sit_bullets4>=x_sit_enemy&&x_sit_bullets4<=x_sit_enemy+16&&y_sit_bullets4<=y_sit_enemy)
	{
		state_enemy = 0;
		state4 = 0;
		x_sit_bullets4 = 0;
		y_sit_bullets4=300;
		mark_win++;
	}
	if(x_sit_bullets5>=x_sit_enemy&&x_sit_bullets5<=x_sit_enemy+16&&y_sit_bullets5<=y_sit_enemy)
	{
		state_enemy = 0;
		state5 = 0;
		x_sit_bullets5 = 0;
		y_sit_bullets5=300;
		mark_win++;
	}
	
	// judgement for bullest and enemy2
	if(x_sit_bullets>=x_sit_enemy2&&x_sit_bullets<=x_sit_enemy2+16&&y_sit_bullets<=y_sit_enemy2)
	{
		state_enemy2 = 0;
		state = 0;
		x_sit_bullets = 0;
		y_sit_bullets=300;
		mark_win++;
	}
	if(x_sit_bullets2>=x_sit_enemy2&&x_sit_bullets2<=x_sit_enemy2+16&&y_sit_bullets2<=y_sit_enemy2)
	{
		state_enemy2 = 0;
		state2 = 0;
		x_sit_bullets2 = 0;
		y_sit_bullets2=300;
		mark_win++;
	}
	if(x_sit_bullets3>=x_sit_enemy2&&x_sit_bullets3<=x_sit_enemy2+16&&y_sit_bullets3<=y_sit_enemy2)
	{
		state_enemy2 = 0;
		state3 = 0;
		x_sit_bullets3 = 0;
		y_sit_bullets3=300;
		mark_win++;
	}
	if(x_sit_bullets4>=x_sit_enemy2&&x_sit_bullets4<=x_sit_enemy2+16&&y_sit_bullets4<=y_sit_enemy2)
	{
		state_enemy2 = 0;
		state4 = 0;
		x_sit_bullets4 = 0;
		y_sit_bullets4=300;
		mark_win++;
	}
	if(x_sit_bullets5>=x_sit_enemy2&&x_sit_bullets5<=x_sit_enemy2+16&&y_sit_bullets5<=y_sit_enemy2)
	{
		state_enemy2 = 0;
		state5 = 0;
		x_sit_bullets5 = 0;
		y_sit_bullets5=300;
		mark_win++;
	}
	
	// judgement for bullets and enemy3
	if(x_sit_bullets>=x_sit_enemy3&&x_sit_bullets<=x_sit_enemy3+16&&y_sit_bullets<=y_sit_enemy3)
	{
		state_enemy3 = 0;
		state = 0;
		x_sit_bullets = 0;
		y_sit_bullets=300;
		mark_win++;
	}
	if(x_sit_bullets2>=x_sit_enemy3&&x_sit_bullets2<=x_sit_enemy3+16&&y_sit_bullets2<=y_sit_enemy3)
	{
		state_enemy3 = 0;
		state2 = 0;
		x_sit_bullets2 = 0;
		y_sit_bullets2=300;
		mark_win++;
	}
	if(x_sit_bullets3>=x_sit_enemy3&&x_sit_bullets3<=x_sit_enemy3+16&&y_sit_bullets3<=y_sit_enemy3)
	{
		state_enemy3 = 0;
		state3 = 0;
		x_sit_bullets3 = 0;
		y_sit_bullets3=300;
		mark_win++;
	}
	if(x_sit_bullets4>=x_sit_enemy3&&x_sit_bullets4<=x_sit_enemy3+16&&y_sit_bullets4<=y_sit_enemy3)
	{
		state_enemy3 = 0;
		state4 = 0;
		x_sit_bullets4 = 0;
		y_sit_bullets4=300;
		mark_win++;
	}
	if(x_sit_bullets5>=x_sit_enemy3&&x_sit_bullets5<=x_sit_enemy3+16&&y_sit_bullets5<=y_sit_enemy3)
	{
		state_enemy3 = 0;
		state5 = 0;
		x_sit_bullets5 = 0;
		y_sit_bullets5=300;
		mark_win++;
	}
	
}

// Function of recording the state of buttons
void CC_Move()  
{
	u8 key;
	key=KEY_Scan(1);
	if(key==3)
	{
		/*
		GPIO_SetBits(GPIOF,GPIO_Pin_8);
		delay_ms(5);
		GPIO_ResetBits(GPIOF,GPIO_Pin_8);
		*/
		x_sit_ship=x_sit_ship-10;
		CC_pick_random_position();     // Pick a random number of position whenever push the button
		CC_pick_random_speed();  // Pick a random number of speed whenever push the button
	}
	else
		if(key==1)
		{
			/*
			GPIO_SetBits(GPIOF,GPIO_Pin_8);
		  delay_ms(5);
		  GPIO_ResetBits(GPIOF,GPIO_Pin_8);
			*/
			x_sit_ship=x_sit_ship+10;
      CC_pick_random_position();     // Pick a random number whenever push the button		
			CC_pick_random_speed();  // Pick a random number of speed whenever push the button
		}
		else
			  //key = KEY_Scan(0);
				if(key==4)
				{
					/*
					GPIO_SetBits(GPIOF,GPIO_Pin_8);
		      delay_ms(10);
		      GPIO_ResetBits(GPIOF,GPIO_Pin_8);
					*/
					limits++;
					if(limits==1)
					{
						state = 1;
					  x_sit_bullets=x_sit_ship;
					}
					else
						if(limits==2)
						{
							state2 = 1;
							x_sit_bullets2=x_sit_ship;
						}
						else
							if(limits==3)
							{
								state3 = 1;
								x_sit_bullets3=x_sit_ship;
							}
							else
								if(limits==4)
								{
									state4 = 1;
									x_sit_bullets4=x_sit_ship;
								}
								else
									if(limits==5)
									{
										state5 = 1;
										x_sit_bullets5=x_sit_ship;
									}
									CC_pick_random_position();     // Pick a random number whenever push the button
									CC_pick_random_speed();  // Pick a random number of speed whenever push the button
				}
				if(!state&&!state2&&!state3&&!state4&&!state5)
					limits = 0;
}



//主函数
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化 
	KEY_Init(); 				//按键初始化  
	tp_dev.init();			//触摸屏初始化
	LCD_Clear(BLACK);
	
	//start menu
	while(1)
	{
		POINT_COLOR=BLUE;
		LCD_ShowString(60,150,200,16,24,"*STAR  WAR*");
		POINT_COLOR=RED;
		LCD_ShowString(60,180,200,16,16,"win for 15 marks");
		LCD_ShowString(20,200,200,16,16,"Push KEY1 join the battle");
		if(KEY_Scan(0)==2)
			break;
	}
	
	//main loop
	while(1)
	{
		
		CC_GameTable();
		CC_Move();
		CC_Judgement();
		
		//chranging random numbers
		if(random_num_position>10)
			random_num_position = 0;
		random_num_position++;
		if(random_num_speed>10)
			random_num_speed = 0;
		random_num_speed++;
		
		//judgement of win
		if(mark_win==15)
		{
			while(1)
			{
				POINT_COLOR=RED;
				LCD_ShowString(60,150,200,16,24,"*YOU WIN*");
				POINT_COLOR=BLUE;
				LCD_ShowString(20,180,200,16,16,"Press on KEY1 to restart");
				delay_ms(500);                   
				LCD_Clear(BLACK);
				delay_ms(500);
				if(KEY_Scan(1)==2)
				{
					//initialize everything
					state = 0;
					state2 = 0;
					state3 = 0;
					state4 = 0;
					state5 = 0;
					state_enemy = 1;
					state_enemy2 = 1;
					state_enemy3 = 1;
					x_sit_ship=120;
					y_sit_ship=300;
					x_sit_enemy=120;
					x_sit_enemy2=90;
					x_sit_enemy3=150;
					y_sit_enemy=10;
					y_sit_enemy2=10;
					y_sit_enemy3=10;
					limits = 0;
					mark_win = 0;
					life = 10;
					break;
				}
			}
		}
		//judgement of lose
		if(life==0)
		{
			while(1)
			{
				POINT_COLOR=RED;
				LCD_ShowString(60,150,200,16,24,"*YOU LOSE*");
				POINT_COLOR=BLUE;
				LCD_ShowString(20,180,200,16,16,"press on KEY1 to restart");
				delay_ms(500);
				LCD_Clear(BLACK);
				delay_ms(500);
				if(KEY_Scan(1)==2)
				{
					//initialize everything
					state = 0;
					state2 = 0;
					state3 = 0;
					state4 = 0;
					state5 = 0;
					state_enemy = 1;
					state_enemy2 = 1;
					state_enemy3 = 1;
					x_sit_ship=120;
					y_sit_ship=300;
					x_sit_enemy=120;
					x_sit_enemy2=90;
					x_sit_enemy3=150;
					y_sit_enemy=10;
					y_sit_enemy2=10;
					y_sit_enemy3=10;
					limits = 0;
					mark_win = 0;
					life = 10;
					break;
				}
			}
		}
		delay_ms(100);
	}
}
