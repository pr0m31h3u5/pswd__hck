
//Created by Mitchell Barker
//3-12-16
//Password breaking tools

//Compile with: g++ -std=c++11 main.cpp -lcrypto -lssl



#include "database.hpp"
#include <ctime>
	using namespace std;



int main(void){
	string user;
	string passwd;
	//authenticate();
	//cout<<"authen"<<endl;
	Database db = Database("./db.txt");

	//db.print_user_hash();
	// string s = "applex";
	// string m;

	// cout<<"username: ";
	// getline(cin, user);
	// cout<<"password: ";
	// cin>>passwd;
	/*cout<<munge_char('a')<<endl;
	cout<<munge_char('A')<<endl;
	cout<<munge_char('@')<<endl;
	cout<<munge_char('^')<<endl;*/
	//db.brute_find("brute.txt");
	//db.dict_find("words.txt", "w_cracked.txt");

	cout<<"Pass length: ";
	int i;
	cin>>i;
	cout<<"Your password: "<<gen_password("big_dict.txt",i, 2, 2)<<endl;

	// start = clock();
	// m = mungest_string(s);

	// for(int i = 0; i < 100000; ++i)
	// 	m == s;
	// cout<<"Time: "<<(clock() - start)/ (double)(CLOCKS_PER_SEC / 1000)<<endl;

	// vector<string> temp = children("apple");
	// for(int i = 0; i < temp.size(); ++i )
	// 	cout<<temp[i]<<endl;

	// string cur = "myfavoriteplacesonearth";
	// int count = 0;
	// while(!munged_string(cur)){
	// 	cur = next_munge("myfavoriteplacesonearth", cur);
	// 	cout<<cur<<endl;
	// 	++count;
	// }
	// cout<<count<<endl;



	// cout<<next_string("a")<<endl;
	// cout<<next_string("abcdef")<<endl;
	// cout<<next_string("a~")<<endl;
	// string hash = my_compute_hash("purplegiraffe");
	
	// User mitch = User("Barker","Mitchell","mbarker2", 721007778, hash);

	// db.db.push_back(mitch);

	// if(db.authenticate(user,passwd))
	// 	cout<<user<<" authenticated."<<endl<<"Password ACCEPTED"<<endl;
	// else
	// 	cout<<user<<" rejected."<<endl<<"Password DENIED"<<endl;



	return 0;
}