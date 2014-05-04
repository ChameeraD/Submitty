#include <stdlib.h>
#include <time.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../modules/modules.h"
#include "../validation/TestCase.h"

#include "config.h"

using std::ifstream;
using std::string;
using std::cout;
using std::endl;
using std::cerr;

string random_hash();
int execute(const string& cmd);
string to_string(int i);

int main(int argc, char* argv[]) {
	cout << "Running runner..." << endl;

	// Make sure arguments are entered correctly

	if (argc != 3) {
		cout << "Incorrect # of arguments:" << argc << endl;
		cout << "Usage : " << endl
				<< "     ./runner path/to/user submission_num" << endl;
		return 1;
	}

	// Initialize random number seed
	srand(time(NULL));

	// Extract Paths

	string user_path = argv[1];
	string submission_dir = argv[2];

	cout << "User path: " << user_path << endl;

	// Copy files to temporary directory
	// TODO this cpp file should not do this, it should be done at a higher
	// level, then this should be run at a lower permissions level
	string tmp_path = "/tmp/" + random_hash();

	if (execute("cp -r " + user_path + " " + tmp_path)) {
		cerr << "FAIL: Copy to tmp directory" << endl;
		return 2;
	}

	// Compile files in tmp directory with compile command from config

	if (execute(
			"cd " + tmp_path + "/" + submission_dir + "/FILES && "
					+ compile_command))
		return 3;

	// Create the .submit.out directory
	if (execute("cd " + tmp_path + "/" + submission_dir + "/FILES && mkdir .submit.out")){
		return 4;
	}

	// Run each test case and create output files

	for (unsigned int i = 0; i < num_testcases; i++) {
		string cmd = testcases[i].command();
		if (cmd != "") {
			if (execute(
					"cd " + tmp_path + "/" + submission_dir + "/FILES && " + cmd
							+ " 1>../.submit.out/test"+to_string(i)+"_cout.txt 2>../.submit.out/test"+to_string(i)+"_cerr.txt")){
				cerr << "ERROR RUNNING TEST CASE " << i << endl;
			}
		}
	}

	//TODO MAKE CALL TO VALIDATOR!!!

}

// Executes command (from shell) and returns error code (0 = success)
int execute(const string& cmd) {
	cout << "Executing command: " << cmd << endl;
	return system(cmd.c_str());
}

// Returns a random hash that is suitable to be used as a temporary
// directory name
string random_hash() {
	static const char alphanum[] = "0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
	string hash(16, ' ');
	for (int i = 0; i < 16; ++i) {
		hash[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	return hash;
}

string to_string(int i) {
	std::ostringstream tmp;
	tmp << i;
	return tmp.str();
}