/*
 * Name : main.cpp
 * Author : Microchip MPU32 Technical marketing
 *
 * Copyright (C) Microchip Technology Inc.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <string>
#include <ctime>
#include <memory>
#include <egt/ui>
#include <egt/uiloader.h>
#include "time_management.h"

//Optionnal : Red LED Blinkig when reaching 100% of batterry charge
#include <iostream>
#include <fstream>

void blink_red_led(bool state)
{

	std::ofstream blue_led_on_off;
	std::ofstream blue_led_blinker;
	blue_led_on_off.open("/sys/class/leds/blue/brightness");
	blue_led_blinker.open("/sys/class/leds/blue/trigger");

	std::ofstream red_led_on_off;
	std::ofstream red_led_blinker;
	red_led_on_off.open("/sys/class/leds/red/brightness");
	red_led_blinker.open("/sys/class/leds/red/trigger");

	if (state)
	{
		blue_led_on_off<<"0";
		red_led_blinker<<"heartbeat";

	}
	else if (!state)
	{
		blue_led_blinker<<"heartbeat";
		red_led_on_off<<"0";


	}
	blue_led_on_off.close();
	blue_led_blinker.close();

	red_led_on_off.close();
	red_led_blinker.close();
}

int main(int argc, char** argv)
{

	egt::Application app(argc, argv);
	egt::experimental::UiLoader loader; //Load the xml file loader

	//----------Window widgets----------//
	auto window = std::static_pointer_cast<egt::Window>(loader.load("file:ui.xml")); // Load the ui.xml file
	auto screen_saver=window->find_child<egt::Window>("Screen_saver_window"); //Smart pointers pointing to the three screens
	auto startup_screen=window->find_child<egt::Window>("Startup_screen_window");
	auto main_screen = window->find_child<egt::Window>("Main_screen_window");


	//---------------------------------//

	//-----------Initializations------//
	auto plus_button=window->find_child<egt::ImageButton>("ImageButtonPlus");
	auto minus_button=window->find_child<egt::ImageButton>("ImageButtonMinus");

	plus_button->fill_flags().clear();
	minus_button->fill_flags().clear();

    auto pattern_pb=egt::Pattern(egt::Pattern::Type::linear_vertical,{{0, egt::Color::css("#03a5f5")},{100, egt::Color::css("#023e7a8")}});

	auto battery_progress_bar=window->find_child<egt::ProgressBar>("Battery_ProgressBar");

	battery_progress_bar->border(0);
	battery_progress_bar->fill_flags().clear();
	battery_progress_bar->color(egt::Palette::ColorId::button_bg,pattern_pb);


	auto percentage_charged_text=window->find_child<egt::Label>("BatteryCharged_value_0");
	percentage_charged_text->text("50 %");

	auto elapsed_time_text=window->find_child<egt::Label>("Elapsed_Time_value");
	elapsed_time_text->text("60");

	auto delivered_energy_text=window->find_child<egt::Label>("Delivered_energy_value");
	delivered_energy_text->text("25");

	auto battery_fully_charged_text=window->find_child<egt::Label>("Label_Battery_Full_Charged");
	//---------------End------------------//

	//Periodic Timers initializations
    egt::PeriodicTimer screen_saver_timer(std::chrono::seconds(10));
    egt::PeriodicTimer blinking_green_wire_timer(std::chrono::milliseconds(500));


	// passing from startup screen to main screen when pressing the red button
	auto power_off_button =window->find_child<egt::ImageButton>("ImageButton_power_off");
	power_off_button->fill_flags().clear(); //A laisser
	power_off_button->on_click([&screen_saver_timer,&startup_screen, &main_screen] (egt::Event &)
	{
		startup_screen->hide();
		main_screen->show();
		screen_saver_timer.start();

	});

	//passing from the main screen to the startup screen when pressing the green button

	auto button_power_on =window->find_child<egt::ImageButton>("ImageButton_power_on");
	button_power_on->fill_flags().clear();
	button_power_on->on_click([&screen_saver_timer,&main_screen, &startup_screen] (egt::Event &)
	{
		startup_screen->show();
		main_screen->hide();
		screen_saver_timer.stop();

	});

	//Initialization of the Blinking text
    egt::PeriodicTimer blinking_text_timer(std::chrono::milliseconds(500));
	blinking_text_timer.on_timeout([&battery_fully_charged_text]()
	{
		battery_fully_charged_text->visible_toggle();

	});

    //Plus button management

    auto green_wire_image=window->find_child<egt::ImageLabel>("Green_wire");
	int remaining_time_value=60;
	auto remaining_time_text=window->find_child<egt::Label>("Remaining_Time_value_0");
	plus_button->on_click([&green_wire_image,&blinking_text_timer,&remaining_time_text,&blinking_green_wire_timer,&battery_progress_bar, &percentage_charged_text, &remaining_time_value,&elapsed_time_text,&delivered_energy_text,&battery_fully_charged_text,&screen_saver_timer](egt::Event &)
	{
		screen_saver_timer.stop();
		screen_saver_timer.start();

		battery_progress_bar->value(battery_progress_bar->value()+10);
		percentage_charged_text->text(std::to_string(battery_progress_bar->value())+" %");

		if(remaining_time_value>0)
		{
			remaining_time_value-=12;

		}

		remaining_time_text->text(std::to_string(remaining_time_value));
		elapsed_time_text->text(std::to_string(120-remaining_time_value));
		delivered_energy_text->text(std::to_string(25*(120-remaining_time_value)/60));
		if(remaining_time_value==0)
		{
			blinking_text_timer.start();
			blinking_green_wire_timer.stop();
			green_wire_image->hide();
			blink_red_led(true);
		}

	});

	//Minus button management
	minus_button->on_click([&blinking_text_timer,&blinking_green_wire_timer,&battery_progress_bar, &percentage_charged_text,&remaining_time_value, &remaining_time_text,&elapsed_time_text,&delivered_energy_text,&battery_fully_charged_text,&screen_saver_timer](egt::Event &)
		{
			screen_saver_timer.stop();
			screen_saver_timer.start();

			battery_progress_bar->value(battery_progress_bar->value()-10);
			percentage_charged_text->text(std::to_string(battery_progress_bar->value())+" %");
			if(remaining_time_value<120)//Pareil que précédemment.
			{
				remaining_time_value+=12;
			}
			remaining_time_text->text(std::to_string(remaining_time_value));
			elapsed_time_text->text(std::to_string(120-remaining_time_value));
			delivered_energy_text->text(std::to_string(25*(120-remaining_time_value)/60));

			if(remaining_time_value!=0)
			{
				battery_fully_charged_text->hide();
				blinking_green_wire_timer.start();//
				blinking_text_timer.stop();
				blink_red_led(false);
			}
		});

    //Screen saver management
    auto screen_saver_remaining_time_text=window->find_child<egt::Label>("Remaining_time_value");
    //Battery_charged_value_screen_saver
    auto screen_saver_battery_charged_value=window->find_child<egt::Label>("Battery_charged_value_screen_saver");

    //Screen saver progressbar initialization

    egt::PeriodicTimer progress_bar_timer(std::chrono::milliseconds(20));

    auto screen_saver_progress_bar=window->find_child<egt::ProgressBar>("Screen_saver_ProgressBar");

	screen_saver_progress_bar->border(0);
	screen_saver_progress_bar->fill_flags().clear();
	screen_saver_progress_bar->move({308,361});
	screen_saver_progress_bar->color(egt::Palette::ColorId::button_bg,pattern_pb,egt::Palette::GroupId::normal);

	// What does the script does when the screen_saver_timer reaches 10 seconds
    screen_saver_timer.on_timeout([&percentage_charged_text,&screen_saver_battery_charged_value,&screen_saver, &main_screen,&screen_saver_remaining_time_text, &remaining_time_text, &progress_bar_timer ]()
    {
       main_screen->hide();
       screen_saver->show();
       screen_saver_remaining_time_text->text(remaining_time_text->text()+" min");
       screen_saver_battery_charged_value->text(percentage_charged_text->text());
       progress_bar_timer.start();

    });

    //Animation of the progress bar of the screen_saver
    progress_bar_timer.on_timeout([&battery_progress_bar,&screen_saver_progress_bar]()
	{
    	if (screen_saver_progress_bar->value()==100)
		{
    		screen_saver_progress_bar->value(0);
		}
    	screen_saver_progress_bar->value(screen_saver_progress_bar->value()+1);//
	});

    //K1 press management
    window->on_event([&main_screen,&screen_saver,&screen_saver_timer](egt::Event & event)
	{
    	 switch (event.key().keycode)
		{
			case egt::EKEY_0:
			{
				main_screen->show();
			    screen_saver->hide();
				screen_saver_timer.stop();
				screen_saver_timer.start();
				break;
			}

			default:
				break;
		}
		}, {egt::EventId::keyboard_up});


    blinking_green_wire_timer.on_timeout([&green_wire_image]()
	{
    	green_wire_image->visible_toggle();
	});
    blinking_green_wire_timer.start();

    //Time and date management
    egt::PeriodicTimer time_timer(std::chrono::seconds(1));
	auto date_text=window->find_child<egt::Label>("Date_value");
	auto time_text=window->find_child<egt::Label>("Time_value");
	date_text->text(current_date());
    time_timer.on_timeout([&time_text]()
    {
       time_text->text(current_time());
    });
    time_timer.start();

    //Optional : Animation of the MCHP Logo

	auto logo=window->find_child<egt::ImageLabel>("ImageLabel57");

	egt::PropertyAnimator animation;
	animation.starting(logo->x());
	animation.ending(logo->x() + 615);
	animation.duration(std::chrono::seconds(4));
	animation.easing_func(egt::easing_linear);
	animation.on_change([&logo](egt::PropertyAnimator::Value value) { logo->x(value); });

	auto delay= new egt::AnimationDelay(std::chrono::milliseconds(500));
	auto sequence =new egt::AnimationSequence(true);
	sequence->add(animation);
	sequence->add(*delay);
	sequence->start();

return app.run();
}


