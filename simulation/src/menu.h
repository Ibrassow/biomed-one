#pragma once


#include <string>
#include <iostream>



typedef std::string str;



//running manager


class IMenu {
public:

	virtual int detect_input() = 0;
	virtual ~IMenu() {};
};


class Menu : public IMenu {

public:

	str menu_name;
	int options;

	Menu(str name, int nb_options);
	~Menu();

	//Display the menu as it is written in the corresponding text file (../src/database)
	void get_menu_display();



	int detect_input();

	//Get to the next window depending on the user choice
	virtual void switch_window(int choice); //specific to each menu





private:
	//clear the entire window
	void clear(); 


};



























