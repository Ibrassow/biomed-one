#include "menu.h"
#include <fstream> 
#include <Windows.h>
#include <conio.h>



Menu::Menu(str name, int nb_options){

	menu_name = name;
	options = (int)nb_options;
};


Menu::~Menu(){
    clear();
}




void Menu::get_menu_display() {

	str menu_txt;
	std::ifstream FileReader;

    /*while (!FileReader)
    {
        FileReader.open("../src/database/" + menu_name + ".txt");
    }*/
	
    FileReader.open("../src/database/" + menu_name + ".txt");

	//Check if the file is open


	if (FileReader) {
		// Use a while loop together with the getline() function to read the file line by line
		while (getline(FileReader, menu_txt)) {
			std::cout << menu_txt << '\n';
		}
		FileReader.close();
	}
	else {
		std::cout << "Unable to open the specific menu file";
        exit(1);
	}
    FileReader.close();
	
}





int Menu::detect_input() {

    int choice = -1;

    do
    {
        if (std::cin >> choice) {
            std::cout << "\033[A\033[2K"; //clear line if the choice isn't correct
        }
        Sleep(500);

    } while (!(0 <= choice && choice <= options));
    return choice;

};


//specific to each menu
void Menu::switch_window(int choice) {

    switch(choice){
		case 0:
            std::cout << "test for case 0\n";
            break;
        case 1:
            std::cout << "test for case 1\n";
            break;
        case 2:
            std::cout << "test for case 2\n";
            break;
    }
        
}



void Menu::clear() {
    std::cout << "\x1B[2J\x1B[H";
}




