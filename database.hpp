#ifndef DATABASE	
#define DATABASE
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <string>
#include <string.h>
#include <openssl/md5.h>
#include <iomanip>
#include <map>

	using namespace std;

typedef unsigned int uint;

#define START 33
#define END 126
#define START_CHAR (char) START
#define END_CHAR (char) END

bool is_munged(char c);
char munge_char(char c);
string next_munge(string original, string cur);
bool munged_string(string s);
string mungest_string(string s);

string next_string(string cur);
char next_char(char cur);

string my_compute_hash(string passwd);




 map<char, char> munge{{'A','@'},{'B','8'},{'C','('},{'D','6'},{'E','3'},{'F','#'},{'G','9'},{'H','#'},{'I','1'},{'K','<'},{'L','I'},
 	{'N','^'},{'O','0'},{'Q','9'},{'S','$'},{'T','7'},{'V','>'},{'X','%'},{'Y','?'},{'@','4'},{'#','4'},{'$','5'},{'>','<'},
 	{'1','!'},{'4','^'},{'7','+'},{'8','3'}};
class User {
public:
	string last_name, first_name, user_name, hash_val, salt;
	uint uin;

	User(string last="",string first="", string user="",uint u=0, string hash="",string s="");

	bool authenticate(string user, string passwd);

	bool operator< (const User &u1);
	bool operator== (const User &u1);

	friend istream &operator>>(istream &input, User &U);
	friend ostream &operator<<(ostream &output, const User &U);

};

class Database{
public:
	vector<User> db;
	ofstream out;

	Database(string input_file);

	bool authenticate(string user, string passwd);
	void pass_match(string hash);
	void brute_find(string out_file);
	void dict_find(string dict, string out_file);
	void expanded_dict_find(string dict, string out_file);
	void print_user_hash();

	friend ostream &operator<<(ostream &output, const Database &d);


};
User::User(string last,string first, string user,uint u, string hash,string s){
	last_name = last;
	first_name = first;
	user_name = user;
	uin = u;
	hash_val = hash;
	salt = s;
}

bool User::authenticate(string user, string passwd){
	string temp = my_compute_hash(passwd+salt);

	return 	temp == hash_val;
}

bool User::operator< (const User &u1){
		return user_name < u1.user_name;
	}
	bool User::operator== (const User &u1){
		return user_name == u1.user_name;
	}


istream &operator>>(istream &input, User &U){
	string line;
	if(!getline(input, line))
		return input;

	for (int i = 0; i < line.length(); i++){
    if (line[i] == ',')
        line[i] = ' ';
		}
		istringstream iss(line);
		iss>>U.last_name>>U.first_name>>U.user_name>>U.uin>>U.hash_val;
		if(U.hash_val.size() == 3){
			U.salt = U.hash_val;
			iss>>U.hash_val;
		}


	return input;
}

ostream &operator<<(ostream &output, const User &U){
	output<<U.last_name<<","<<U.first_name<<","<<U.user_name<<","
		<<U.uin<<",";
	if(U.salt.length() > 0)
		output<<U.salt;
	output<<","<<U.hash_val;

	return output;
}
Database::Database(string input_file){
	ifstream file(input_file.c_str());

	while(file.good()){
		User tmp = User();
		file>>tmp;
		db.push_back(tmp);
	}

	if(db.back().user_name.size() < 1)
		db.pop_back();

	sort(db.begin(), db.end());

	file.close();
}

void Database::print_user_hash(){
	ofstream os("john_use.txt");
	for(auto it = db.begin(); it != db.end(); ++it)
		os<<it->user_name<<":"<<it->hash_val<<endl;


}

void Database::pass_match(string passwd){
	string hash = my_compute_hash(passwd);
	string r_hash = hash;

	for(auto it = db.begin(); it != db.end(); ++it){
		if(it->salt.length()>0){
			hash = my_compute_hash(passwd+it->salt);
			if(it->hash_val == hash)
				out<<it->user_name<<":"<<passwd<<endl;
			hash = r_hash;
		}
		else if(it->hash_val == hash)
			out<<it->user_name<<":"<<passwd<<endl;
	}
}

void Database::brute_find(string out_file=""){
	string current;
	current = START_CHAR;
	out.open(out_file.c_str());

	while(true){
		pass_match(current);
		current = next_string(current);
	}
	out.close();
}

void Database::dict_find(string dict, string out_file=""){
	ifstream ifile(dict.c_str());
	string temp;

	out.open(out_file.c_str());


	while(ifile.good()){
		getline(ifile, temp);
		pass_match(temp);
	}

	ifile.close();
	out.close();
}

void Database::expanded_dict_find(string dict, string out_file=""){
	ifstream ifile(dict.c_str());
	string temp, cur, m;

	out.open(out_file.c_str());


	while(ifile.good()){
		getline(ifile, temp);
		pass_match(temp);
		cur = temp;
		m = mungest_string(temp);

		while(cur != m){
			cur = next_munge(temp, cur);
			pass_match(cur);
		}
	}

	ifile.close();
	out.close();
}

bool Database::authenticate(string user, string passwd){
	User searching = User("temp","temp",user);
	vector<User>::iterator it;

	it = find(db.begin(), db.end(), searching);

  	if (it!=db.end())
    	return it->authenticate(user,passwd);
    else 
    	return false;

}

ostream &operator<<(ostream &output, const Database &d){
	for(auto it = d.db.begin(); it != d.db.end(); ++it)
		output<<*it<<endl;

	return output;
}
string next_munge(string original, string cur){
	int i = cur.length() -1;
	do{
		if(is_munged(cur[i]))
			cur[i] = original[i];
		else
			cur[i] = munge_char(cur[i]);
		--i;
	}while(cur[i+1] == original[i+1] && i >= 0);
	
	return cur;
}

bool munged_string(string s){
	bool final = true;
	for(int i =0; i < s.length(); ++i){
		if(!is_munged(s[i]))
			final = false;
	}
	return final;
}

string mungest_string(string s){
	char c;
	for(int i = 0; i < s.size(); ++i){
		c = munge_char(s[i]);
		while(s[i] != c){
			s[i] = c;
			c = munge_char(s[i]);
		}
	}
	return s;
}

bool is_munged(char c){
	if (munge_char(c) == c)
		return true;
	else
		return false;
}


char munge_char(char c){
	if(isalpha(c) && islower(c))
		c = toupper(c);
	else{ 
		if(munge.count(c) > 0)
			c = munge[c];
	}

	return c;
}

bool same_chars(const std::string& s) {
    return s.find_first_not_of(s[0]) == std::string::npos;
}


string next_string(string cur){
	int i = cur.length() -1;
	do{
		cur[i] = next_char(cur[i]);
		--i;
	}while(cur[i+1] == START_CHAR && i >= 0);

	if (same_chars(cur) && cur[0] == '!')
		cur = cur+START_CHAR;

	return cur;
}

char next_char(char cur){
	if(cur == END_CHAR)
		cur = START_CHAR;
	else
		++cur;
	return cur;
}



string rand_word(string file, int length){
	//srand(time(NULL));
	vector<string> dict;
	ifstream is(file.c_str());   
   	string line, word, temp;
   	int chars_left = length;


  	while (getline(is , line))
    	dict.push_back(line);

    while(chars_left > 0){
    	temp = dict[rand()%dict.size()];
    	if(chars_left == temp.length()){
    		chars_left -= temp.length();
    		word += temp;
    	}
    	else if (chars_left > temp.length() && chars_left > 6){
    		chars_left -= temp.length();
    		word += temp;
    	}
    }
  	return word;
}
string add_special_num(string s){
	string next = s;
	int r, count = 0;

	while(s == next && count != s.length()*5){
		++count;
		r = rand() % s.length();

		if(islower(next[r]) && !is_munged(toupper(next[r])) ){
			while(!is_munged(next[r]) )
					next[r] = munge_char(next[r]);
		}

	}

	return next;
}

string add_capital(string s){
	string next = s;
	int r;

	while(s == next){
		r = rand() % s.length();
		if( islower(next[r]))
			next[r] = toupper(next[r]);

	}
	return next;

}

string gen_password(string dict, int length, int capitals=1, int nonalpha=1){
	srand(time(NULL));
	string word = rand_word(dict, length);
	for(int i = 0; i < capitals; ++i)
		word = add_capital(word);

	for(int i = 0; i < nonalpha; ++i)
		word = add_special_num(word);
	
	return word;
}


string my_compute_hash(string passwd) {
	MD5_CTX context;
	unsigned char digest[16];
	const char* string = reinterpret_cast<const char*>(passwd.c_str());

	MD5_Init(&context);
	MD5_Update(&context, string, strlen(string));
	MD5_Final(digest, &context);

	char md5string[33];
	for(int i = 0; i < 16; ++i)
    	sprintf(&md5string[i*2], "%02x", (unsigned int)digest[i]);

    return md5string;
}
#endif