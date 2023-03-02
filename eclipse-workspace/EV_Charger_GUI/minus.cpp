/*minus_button->on_click([&blinking_green_wire_timer,&battery_progress_bar, &percentage_charged_text,&remaining_time_value, &remaining_time_text,&elapsed_time_text,&delivered_energy_text,&battery_fully_charged_text,&screen_saver_timer](egt::Event &)
	{
		screen_saver_timer.stop();
		screen_saver_timer.start();

		battery_progress_bar->value(battery_progress_bar->value()-10);
		percentage_charged_text->text(std::to_string(battery_progress_bar->value())+" %");
		if(remaining_time_value<120)
		{
			remaining_time_value+=12;
		}
		remaining_time_text->text(std::to_string(remaining_time_value));
		elapsed_time_text->text(std::to_string(120-remaining_time_value));
		delivered_energy_text->text(std::to_string(25*(120-remaining_time_value)/60));

	});*/
