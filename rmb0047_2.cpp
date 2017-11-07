/*
 * Program:   rmb0047_1.cpp
 * Name:      Rebecca Brannan
 * Date:      10/27/2017
 * E-mail:    rmb0047@auburn.edu
 * 
 * Description:  This is the Distributed TigerBook Social Network where users can broadcast, unicast, or multicast messages.
 * */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

string messagebuffer = "";
ofstream outputFile;
fstream readwriteFile;
stringstream sstream;
std::vector<std::string> usermessages;
std::vector<std::string> homepagemessages;
int times = 0;

/*
 * Function:    	findFile(string file)     
 * Inputs:			a string named file
 * Outputs:			a boolean value
 * Description:		Finds if a file already exists.
 * */
bool findFile(string file) {
	ifstream ifile(file.c_str());
	return ifile;
}

/*
 * The User class.
 * Gives each user a file and can associate the user with a group.
 * */
class User {
	public:
	string username;
	string filename;
	string group;
	void setGroup(string);
	string getGroup();
	User() {}
	User(string nameIn) {
		username = nameIn;
		filename = username + ".txt";
		bool foundFile = findFile(filename.c_str());
		if (!foundFile){
		outputFile.open(filename.c_str());
		outputFile.close();
	  }
		bool isFound = findFile("time.txt");
		if (!isFound) {
		outputFile.open("time.txt");
		outputFile << times;
		outputFile.close();
		}
	}
	string getFileName();
};

/*
 * Function:       getFileName()    
 * Inputs:		   none
 * Outputs:		   A string representing the filename.
 * Description:    Returns a file name;
 * */
string User::getFileName(){
	return filename;
}

/*
 * Function:       setGroup()   
 * Inputs:		   A string representing group name.
 * Outputs:		   none
 * Description:    Adds a user to a group.
 * */
void User::setGroup(string groupname) {
	group = groupname;
}

/*
 * Function:     getGroup()    
 * Inputs:	     none
 * Outputs:		 Returns a string representing the group name.
 * Description:  Gets the user's group.
 * */
string User::getGroup() {
	return group;
}

/*
 * The Group Class.
 * Creates a file when a user creates a Group.
 * */
class Group {
	public:
	string filename;
	string groupname;
	std::vector<User> members;
	string getGroupName();
	string getFileName();
	Group () {}
	Group (string nameIn) {
		groupname = nameIn;
		filename = groupname + ".txt";
		outputFile.open(filename.c_str());
		outputFile.close();
	}
};

/*
 * Function:      getGroupName()     
 * Inputs:        none
 * Outputs:       Returns a string representing the groupname.
 * Description:   Gets a group's name.
 * */
string Group::getGroupName() {
	return groupname;
}

/*
 * Function:      getFileName()       
 * Inputs:	      none
 * Outputs:       Returns a string representing the filename.
 * Description:   Gets a group's file name.
 * */
string Group::getFileName() {
	return filename;
}


/*
 * The System class.
 * This class sets a user as logged in and keeps a vector or users and groups.
 * */
class System {
	public:
	std::vector<User> users;
	std::vector<Group> groups;
	void addUser(User u);
	void addGroup(Group g);
	void setLoggedInUser(User u);
	User loggedinuser;
};

/*
 * Function:     addUser(User u);    
 * Inputs:       A User object.
 * Outputs:		 none
 * Description:  Adds a user to the system's vector of users.
 * */ 
void System::addUser(User u) {
	users.push_back(u);
}

/*
 * Function:     addGroup(Group g)    
 * Inputs:       A Group object.
 * Outputs:      none
 * Description:  Adds a group to the system's vector of groups.
 * */
void System::addGroup(Group g) {
	groups.push_back(g);
}

/*
 * Function:     setLoggedInUser(User u)     
 * Inputs:       A User object
 * Outputs:      none
 * Description:  sets the system's loggedinuser to the current user
 * */
void System::setLoggedInUser(User u) {
	loggedinuser = u;
}



System sys;


/*
 * The Menu class.
 * This class is responsible for displaying the home and wall pages.
 * */
class Menu {
	public:
	void displayHomePage();
	void displayWallPage();
};

/*
 * Function:      sortbyTimeStamp(string t1, string t2)     
 * Inputs:        a string named t1 and a string named t2
 * Outputs:       a boolean value
 * Description:   Returns true or false depending on if the timestamp of one message is less than another.
 * */
bool sortbyTimeStamp(string t1, string t2) {
	size_t pos1 = t1.find("{!") + 2;
	size_t pos2 = t1.find("!}");
	string s = t1.substr(pos1, pos2 - pos1);
	size_t pos3 = t2.find("{!") + 2;
	size_t pos4 = t2.find("!}");
	string s2 = t2.substr(pos3, pos4 - pos3);
	return s < s2;
}

/*
 * Function:         displayWallPage()
 * Inputs:			 None
 * Outputs:			 None	
 * Description:      Displays messages the user has sent.
 * */
void Menu::displayWallPage() {
	string output = "";
	string str = "";
	string substr = "";
	string time = "";
	ifstream in(sys.loggedinuser.filename.c_str());
	while(getline(in, str)) {
		size_t pos1 = str.find("<!") + 2;
		size_t pos2 = str.find("!>");
		substr = str.substr(pos1, pos2 - pos1);
		if (substr == sys.loggedinuser.username) {
			string linetopush = str + "(" + sys.loggedinuser.username + ")";
			usermessages.push_back(linetopush);
		} 		
	}
	in.close();
	string group = sys.loggedinuser.getGroup();
	string f = group + ".txt";
	string line = "";
	if (group != "") {
		ifstream in(f.c_str());
			while (getline(in, line)) {
				size_t pos1 = line.find("<!") + 2;
				size_t pos2 = line.find("!>");
				string subline = line.substr(pos1, pos2 - pos1);
				if (subline == sys.loggedinuser.username) {
					string linetopush = line + "(" + group + ")";
					usermessages.push_back(linetopush);
		}
			}
		in.close();
	}
	
	ifstream ifs("_All.txt");
	while (getline(in, line)) {
		size_t position1 = line.find("<!") + 2;
		size_t position2 = line.find("!>");
		string sub = line.substr(position1, position2 - position1);
		if (sub == sys.loggedinuser.username) {
			string linetopush = line + "(#All)";
			usermessages.push_back(linetopush);
		}
	}
	ifs.close();

	std::sort(usermessages.begin(), usermessages.end(), sortbyTimeStamp);
	for (std::vector<std::string>::iterator it = usermessages.begin(); it != usermessages.end(); ++it) {
		string s = *it;
		string s2 = s;
		string s3 = s;
		size_t namePos = s.find("<!") + 2;
		size_t namePos2 = s.find("!>");
		string name = s.substr(namePos, namePos2 - namePos);
		size_t messagePos = s2.find("!>") + 2;
		size_t messagePos2 = s2.find("(");
		string message = s2.substr(messagePos, messagePos2 - messagePos);
		size_t recipPos = s3.find("(");
		size_t recipPos2 = s3.find(")");
		string recip = s3.substr(recipPos, recipPos2 - recipPos + 1);
		output += name + recip + "\n" + message + "\n\n";
	
	}
	cout << "\n===========================================================\n";
	cout << "|                " + sys.loggedinuser.username + "'s Wall Page                     |";
	cout << "\n===========================================================\n";
	cout << output << endl;
	cout << "\n===========================================================\n";
	cout << "|    End of   " + sys.loggedinuser.username + "'s Wall Page                     |";
	cout << "\n===========================================================" << endl;
	
}

/*
 * Function:        displayHomePage()     
 * Inputs:          none 
 * Outputs:         none
 * Description:     Displays a user's home  page.
 * */
void Menu::displayHomePage() {
	string line = "";
	string str = "";
	string output = "";
	
	cout << "\n===========================================================\n";
	cout << "|                " + sys.loggedinuser.username + "'s Home Page                     |";
	cout << "\n===========================================================\n" << endl;
	
	ifstream ifs("_All.txt");
	while (getline(ifs, str)) {
		string linetopush = str + "(#All)";
		homepagemessages.push_back(linetopush);	
	}
	ifs.close();
	
	string usergroup = sys.loggedinuser.group;
	if (usergroup != "") {
		string groupfile = usergroup + ".txt";
		ifstream  istream(groupfile.c_str());
		while (getline(istream, line)) {
		string linetopush = line + "(" + usergroup + ")";
		homepagemessages.push_back(linetopush);	
		}
		istream.close();
	}
	
	string linefile = "";
	string userfile = sys.loggedinuser.username + ".txt";
	ifstream stream(userfile.c_str());
	while (getline(stream, linefile)) {
		string linetopush = linefile + "(" + sys.loggedinuser.username + ")";
		homepagemessages.push_back(linetopush);
	}
	
	std::sort(homepagemessages.begin(), homepagemessages.end(), sortbyTimeStamp);
	for (std::vector<std::string>::iterator it = homepagemessages.begin(); it != homepagemessages.end(); ++it) {
		string s = *it;
		string s2 = s;
		string s3 = s;
		size_t namePos = s.find("<!") + 2;
		size_t namePos2 = s.find("!>");
		string name = s.substr(namePos, namePos2 - namePos);
		size_t messagePos = s2.find("!>") + 2;
		size_t messagePos2 = s2.find("(");
		string message = s2.substr(messagePos, messagePos2 - messagePos);
		size_t recipPos = s3.find("(");
		size_t recipPos2 = s3.find(")");
		string recip = s3.substr(recipPos, recipPos2 - recipPos + 1);
		output += name + recip + "\n" + message + "\n\n";
	}
	cout << output << endl;
	cout << "\n===========================================================\n";
	cout << "|    End of   " + sys.loggedinuser.username + "'s Home Page                     |";
	cout << "\n===========================================================" << endl;
	
}

Menu menu;



/*
 * Function:         multicastMessage
 * Inputs:			 none
 * Outputs:			 none
 * Description:		 Creates a multicast message
 * */
void multicastMessage() {
	times++;
	outputFile.open("time.txt");
	outputFile << times;
	outputFile.close();
	string recipientgroup = "";
	string groupmessage = "";
	string multicast = "";
	string filename;
	sstream << times;
	string time = sstream.str();
	cout << "\n\nEnter the group name: ";
	cin >> recipientgroup;
	Group gr;
	filename = recipientgroup + ".txt";
	bool isFound = findFile(filename);
	if (isFound) {
	cout << "\n\nEnter the message: ";
	do {
		cin.clear();
		cin.ignore(1000, '\n');
		cin >> multicast;
		if (multicast != "^!") {
			groupmessage += multicast + "\n";
		}
	}
	while (multicast != "^!");
	readwriteFile.open(filename.c_str(), ios::app);
	readwriteFile << "{!" + time + "!}<!" + sys.loggedinuser.username + "!>" + groupmessage;
	readwriteFile.close();
	cout << "\n===========================================================\n";
	cout << "|      " << sys.loggedinuser.username << " multicasted a message              |\n";
	cout << "===========================================================\n" << endl;
	} else {
		cout << "\n\nThat group does not exist." << endl;
	}
}

/*
 * Function:     broadcastMessage()    
 * Inputs:		 none
 * Outputs:		 none
 * Description:	 Allows the user to broadcast a message to everyone.
 * */
void broadcastMessage() {
	times++;
	outputFile.open("time.txt");
	outputFile << times;
	outputFile.close();
	string message = "";
	string broadcast = "";
	sstream << times;
	string time = sstream.str();
	string filename = "_All.txt";
	cout << "\n\nEnter the message: ";
	bool isFound = findFile(filename);
	do {
		cin.clear();
		cin.ignore(1000, '\n');
		cin >> broadcast;
		if (broadcast != "^!") {
			message += broadcast + "\n";
		}
	}
	while (broadcast != "^!");
	if (isFound) {
	readwriteFile.open(filename.c_str(), ios::app);
	readwriteFile << "{!" + time + "!}<!" + sys.loggedinuser.username + "!>" + message;
	readwriteFile.close();
	cout << "\n===========================================================\n";
	cout << "|      " << sys.loggedinuser.username << " broadcasted a message              |\n";
	cout << "===========================================================\n" << endl;
	} else {
		outputFile.open(filename.c_str());
		outputFile.close();
		readwriteFile.open(filename.c_str(), ios::app);
		readwriteFile << "{!" + time + "!}<!" + sys.loggedinuser.username + "!>" + message;
		readwriteFile.close();
	}
}

/*
 * Function:          unicastMessage
 * Inputs:			  none
 * Outputs:           none
 * Description:       Creates a unicast message.
 * */
void unicastMessage() {
	times++;
	outputFile.open("time.txt");
	outputFile << times;
	outputFile.close();
	string recipient = "";
	string message = "";
	string unicast = "";
	string file;
	sstream << times;
	string time = sstream.str();
	User r;
	cout << "\n\nEnter the recipient user name: ";
	cin >> recipient;
	file = recipient + ".txt";
	bool isFound = findFile(file);
	if (isFound) {
	cout << "\n\nEnter the message: ";
	do {
		cin.clear();
		cin.ignore(1000, '\n');
		cin >> unicast;
		if (unicast != "^!") {
			message += unicast  + "\n";
		}
	}
	while (unicast != "^!");
	readwriteFile.open(file.c_str(), ios::app);
	readwriteFile << "{!" + time + "!}<!" + sys.loggedinuser.username + "!>" + message;
	readwriteFile.close();
	cout << "\n===========================================================\n";
	cout << "|      " << sys.loggedinuser.username << " unicasted a message               |\n";
	cout << "===========================================================\n" <<endl;
	} else {
		cout << "\n\nThat user does not exist." << endl;
	}
}

/*
 * Function:        createUser()  
 * Inputs:			none
 * Outputs:			none
 * Description:		Creates a new user.
 * */
void createUser() {
		string username = "";
		cout << "\n\nPlease enter the user name: ";
		cin >> username;
		User user(username);
		sys.setLoggedInUser(user);
		sys.addUser(user);
		cout << "\n===========================================================\n";
		cout << "|      Welcome to Distributed TigerBook System, " + username + "     |\n";
		cout << "===========================================================\n" << endl;
}

/*
 * Function:        createGroup()   
 * Inputs:          none
 * Outputs:         none
 * Description:     Creates a group object.
 * */
void createGroup() {
	string newgroupname = "";
	cout << "\n\nPlease enter the group name: ";
	cin >> newgroupname;
	Group group(newgroupname);
}

/*
 * Function:         joinGroup()    
 * Inputs:           none 
 * Outputs:          none
 * Description:      Adds a user to a group.
 * */
void joinGroup() {
	string groupname = "";
	cout << "\n\nPlease enter the group name: ";
	cin >> groupname;
	string filename = groupname + ".txt";
	bool isFound = findFile(filename);
	if (isFound) {
		sys.loggedinuser.setGroup(groupname);
		cout << "\n===========================================================\n";
		cout << "|      " << sys.loggedinuser.username << " is in " << groupname << " group               |\n";
		cout <<  "===========================================================\n" << endl;
	}
	else {
		cout << "\nThat group does not exist." <<endl;
	}
}


/*
 * Function:        switchMenu()    
 * Inputs:          none
 * Outputs:         none
 * Description:     Produces the menu options for the user.
 * */
void switchMenu() {
	char option;
		cout << "\n\nBroadcast (b), Multicast (m), Unicast (u), Wall page (w), Home page (h), Create new group (g),\n";
		cout << "Join a group (j), Quit (q)\n\n";
		cout << "Choose an option: ";
		cin >> option;

		switch (option) {
		case 'b':
			broadcastMessage();
			switchMenu();
			break;
		case 'm':
			multicastMessage();
			switchMenu();
			break;
		case 'u':
			unicastMessage();
			switchMenu();
			break;
		case 'w':
			menu.displayWallPage();
			switchMenu();
			break;
		case 'h':
			menu.displayHomePage();
			switchMenu();
			break;
		case 'g':
			createGroup();
			switchMenu();
			break;
		case 'j':
			joinGroup();
			switchMenu();
			break;
		case 'q':
			cout << "===========================================================\n";
			cout << "|   Thank you for using Distributed TigerBook Social Network            |\n";
			cout << "===========================================================\n";
			break;
		default:
			cout << "Not a valid entry. Please try again." << endl;
			switchMenu();
		}
}

/*
 * Function:          main()  
 * Inputs:            none
 * Outputs:           none
 * Description:       The main method for the Distributed TigerBook Social Network.
 * */
int  main() {
	string name;
	User user;
	cout << "===========================================================\n";
	cout << "|          Distributed TigerBook Social Network!                |\n";
	cout << "===========================================================\n";
	
	createUser();
	switchMenu();
	
}
