#include <iostream>
#include <fstream>
#include <string>
#include "SFML/Graphics.hpp"

#define DIR_NAME "Files/"
#define PRECISION 8 //8 pixels will be converted into single sign

using namespace std;

string get_input_path();
string get_output_path();
char ** convert_image_into_text(string, int&, int&);
char get_sign(sf::Color);
void save_text(string, char **, int, int);

int main()
{
	system("Color 0");

	string input_file_name = get_input_path();
	string output_file_name = get_output_path();

	int number_of_horizontal_sings;
	int number_of_vertical_sings;
	char **text_array = convert_image_into_text(input_file_name, number_of_horizontal_sings, number_of_vertical_sings);
	save_text(output_file_name, text_array, number_of_horizontal_sings, number_of_vertical_sings);

	return 0;
}


string get_input_path()
{
	string file_name;
	fstream file;
	bool flag = 1;

	do
	{
		cout << "Put the image into Files/" << endl;
		cout << "Type the input file name with extension" << endl;
		cin >> file_name;
		system("cls");

		file.open(DIR_NAME + file_name, ios::in);
		if (!file.good())
		{
			cout << "Cannot find " + file_name + " file!" << endl;
		}
		else
		{
			flag = 0;
			file.close();
		}

	} while (flag);

	return file_name;
}


string get_output_path()
{
	string file_name;
	fstream file;
	bool flag = 1;

	do
	{
		
		cout << "Type the output file name without extension" << endl;
		cin >> file_name;
		file_name += ".txt";
		system("cls");

		file.open(DIR_NAME + file_name, ios::in);
		if (file.good())
		{
			cout << file_name + " exists!" << endl;
			file.close();
		}
		else
		{
			flag = 0;
		}

	} while (flag);

	return file_name;
}


char ** convert_image_into_text(string input_file_name, int& number_of_horizontal_sings, int& number_of_vertical_sings)
{
	system("cls");

	sf::Image map;
	if (!map.loadFromFile(DIR_NAME+input_file_name))
		cout << "Error during loading map!" << endl;

	int map_horizontal_size = map.getSize().x;
	int map_vertical_size = map.getSize().y;
	cout << "Image size: " << map_horizontal_size << "x" << map_vertical_size << endl;

	number_of_horizontal_sings = map_horizontal_size/PRECISION;
	number_of_vertical_sings = map.getSize().y/PRECISION;
	cout << "Number of horizontal signs: " << number_of_horizontal_sings << endl;
	cout << "Number of vertical signs: " << number_of_vertical_sings << endl;

	char ** array = new char*[number_of_vertical_sings];
	for (int i = 0; i < number_of_vertical_sings; i++)
	{
		array[i] = new char[number_of_horizontal_sings];
	}

	sf::Color color;
	for (int i = 0; i < number_of_vertical_sings; i++)
	{
		for (int j = 0; j < number_of_horizontal_sings; j++)
		{
			color = map.getPixel(j*PRECISION, i*PRECISION);
			array[i][j] = get_sign(color);
		}
	}

	return array;
}


char get_sign(sf::Color color)
{
	double r = color.r / 255.0;
	double g = color.g / 255.0;
	double b = color.b / 255.0;

	double max;
	double min;
	char maxChar;
	{
		if (r > g)
		{
			min = g;
			max = r;
			maxChar = 'r';
		}
		else
		{
			min = r;
			max = g;
			maxChar = 'g';
		}

		if (max < b)
		{
			max = b;
			maxChar = 'b';
		}

		if (min > b)
		{
			min = b;
		}
	}

	double delta = max - min;

	double lightness = (max + min)*0.5;
	double hue;
	double saturation;

	if (delta == 0)
	{
		hue = 0;
		saturation = 0;
	}
	else if (maxChar == 'r')
	{
		hue = 60 * (g - b) / delta;
		saturation = delta/(1-std::abs(2*lightness-1));
	}
	else if (maxChar == 'g')
	{
		hue = 60 * ((b - r) / delta + 2);
		saturation = delta / (1 - std::abs(2 * lightness - 1));
	}
	else if (maxChar == 'b')
	{
		hue = 60 * ((r - g) / delta + 4);
		saturation = delta / (1 - std::abs(2 * lightness - 1));
	}

	if (hue < 0)
	{
		hue += 360;
	}

	if (hue == 0)
	{
		if (lightness == 1) //color white
		{
			return 32;
		}
		else if (lightness == 0) //color black
		{
			return 33;
		}
	}

	double section =  360 / 59.0;	//59 unique characters
	char sign = static_cast<char>(34 + hue/ section); //ASCII from 34 to 90
	return sign; 
}


void save_text(string file_name, char ** text_array, int map_horizontal_size, int map_vertical_size)
{
	fstream file;
	file.open(DIR_NAME + file_name, ios::out);

	for (int i = 0; i < map_vertical_size; i++)
	{
		for (int j = 0; j < map_horizontal_size; j++)
		{
			file << text_array[i][j];
		}
		file << endl;
	}

	file.close();

	cout << "Successfully created .txt file" << endl << endl;

	for (int i = 0; i < map_vertical_size; i++)
	{
		delete[] text_array[i];
	}
	delete[] text_array;

	system("pause");
}