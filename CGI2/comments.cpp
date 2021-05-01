#include <iostream>
#include<fstream>
#include <time.h>
#include "string.h"
#include "cgi.h"

using namespace std;
const char* header = "Comments";


struct Comment {
	char name[26];
	char text[1001];
	__time32_t time;
};
struct tm newtime;

void show_comment_form() {
	cout << "<form action='comments.cgi' method='post'>" <<
		"<p>¬ведите ваше им€:</p>" <<
		"<p><input maxlength=25 size = 30 name='name'></p>" <<
		"<p>¬ведите текст комментари€:</p>" <<
		"<p><textarea name='comment' rows=5 maxlength=1000></textarea></p>" <<
		"<p><input type='submit'></p>" <<
		"</form>";
}

void show_comment(const Comment& comment /*char* line*/) {

	char buffer[32];
	errno_t errNum;
	_localtime32_s(&newtime, &comment.time);
	errNum = asctime_s(buffer, 32, &newtime);

	/*istringstream input(line);
	char* name = new char[26];
	char* text = new char[1001];
	input >> name;
	input >> text;
	time_t sec = time(0);
	*/

	cout << "<div class='comments'><p>" << buffer << "</p>" << "<p>" << comment.name << "</p>";
	for (int i = 0; comment.text[i]; i++) {
		if (comment.text[i] == '<') {
			cout << "&lt;";
		}
		else if (comment.text[i] == '>') {
			cout << "&gt;";
		}
		else if (comment.text[i] == '\n') {
			cout << "<br>";
		}
		else {
			cout << comment.text[i];
		}
	}
	cout << "<hr>";
}

void show_comments() {
	ifstream f("static/comments.dat", ios_base::binary);
	if (f.is_open()) {
		while (!f.eof()) {
			Comment comment;
			f.read((char*)&comment, sizeof(Comment));
			show_comment(comment);
		}
	}
	else {
		cout << "Cannot open comment file";
	}
	f.close();
}

void get_comment_data(char*& data) {
	if (!is_get()) {
		delete[] data;
		size_t buf_size = get_content_length();
		data = new char[buf_size + 1];
		fread_s(data, buf_size + 1, sizeof(char), buf_size, stdin);
		data[buf_size] = 0;

		char* name = new char[26];
		char* comment = new char[1001];

		get_param_value(name, "name", data);
		get_param_value(comment, "comment", data);

		cout << "<h2>" << name << "</h2>";
		cout << "<h2>" << comment << "</h2>";
		cout << sizeof(Comment);

		ofstream f("static/comments.dat", ios_base::app | ios_base::binary);
		if (f.is_open()) {
			__time32_t aclock;
			errno_t errNum;
			_time32(&aclock);

			Comment new_comment;
			strcpy_s(new_comment.name, name);
			strcpy_s(new_comment.text, comment);
			new_comment.time = _time32(&aclock);
			f.write((char*)&new_comment, sizeof(Comment));
		}
		else {
			cout << "Cannot open comment's file";
		}
		f.close();
		delete[] name; delete[] comment;
		
	}
}

void show_content() {
	char* data = nullptr;
	get_comment_data(data);
	show_comment_form();
	show_comments();
}

int main() {

	cout << "Content-type: text/html; charset=Windows-1251\n\n";
	ifstream f;
	f.open("template/main.html");
	if (f.is_open())
	{
		while (!f.eof())
		{
			char* line = new char[65536];
			f.getline(line, 65535);
			if (strcmp(line, "<!--header-->") == 0)
				cout << header;
			else if (strcmp(line, "<!--menu-->") == 0)
				show_menu();
			else if (strcmp(line, "<!--content-->") == 0)
				show_content();
			else
			{
							
			}
				cout << line;
		}
		f.close();
	}
	else
	{
		cout << "Can't open template file :(";
	}

	return 0;
}