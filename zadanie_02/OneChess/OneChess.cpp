/*

Wykonał: Jakub Szubartowski (A01/C01/L02)   I Informatyka
                email: 31024@s.pm.szczecin.pl
                  ZADANIE: PR0BLEM HETMANOW

*/

                                                         //// include section
#include <direct.h>                                      // directory and file managment
#include <windows.h>                                     // windows functions like keyboard input
#include <iostream>                                      // i-o functions
#include <vector>                                        // dynamic arrays
#include <fstream>                                       // file i-o functions
#include <iomanip>                                       // output formatting
#include <string>                                        // strings
#include <thread>                                        // thread managment (time only)
#include <sstream>                                       // StringStream

                                                         //// basic namespace
using namespace std;

                                                         //// main options?
                                                         ////const
const int maxBoardSize = 32;                             // max board size
const wchar_t title[] = L"OneChess";                     // innit title variable

                                                         //// numeral type
short startingX = 0;                                     // starting position -X
short startingY = 0;                                     // - Y
short boardSize = 8;                                     // default board size
int solveSpeed = 50;                                     // default gap between rec (ms)

                                                         //// t/f type
bool isStarted = true;                                   // control flags
bool isActive = true;                                    // ========
bool firstStart = true;                                  // ========
bool turboMode = false;                                  // ========

                                                         //// strings
string status = "NaN";                                   // default progress info

                                                         //// blackbox options
const string folderName = "runs";                        // main archive folder name
const string extension = ".dat";                         // archive extension name
string exFilename = "OneChess";                          // default file name
string filePath;                                         // NAME RESERVATION
string fileName;                                         // ========
vector<string> steps;                                    // ========
int stepsCount = 0;                                      // isSafe function counter var


class Blackbox {                                                                                                             //// archive class
public:
    ofstream file;                                                                                                           // output file stream obj

    void innit(bool close){                                                                                                  //// initialization function
        isActive = true;                                                                                                     // set archive status
        fileName = exFilename + extension;
        filePath = folderName + "/" + fileName;
        if(close){ saveStepsToFile(); }else { file.open(filePath); }                                                         // open/close sequence
        stepsCount = 0;

        char dir[256];                                                                                                       // create 'runs' dir if not exist
        sprintf_s(dir, 256, "%s", folderName.c_str());
        if (_mkdir(dir) == -1){}
    }
    
    void saveStep(string moveType, int placeIDX, int placeIDY) {                                                             //// funtion for saving move to array
        stepsCount++;                                                                                                        // increase step counter
        string step = "(" + to_string(placeIDX + 1) + ", " + to_string(placeIDY + 1) + ") -> " + moveType;          // move formating
        steps.push_back(step);                                                                                               // send move to array
    }

    void saveStepsToFile() {                                                                                                 //// save array in .dat file
        if (file.is_open()) {                                                                                                // check if file is open
            for (const auto& step : steps) {
                file << step << endl;                                                                                        // send move to last line of file
            }
            steps.clear();                                                                                                   // clear move array
            file.close();                                                                                                    // close file
        }
    }

    void printStatus() {                                                                                                     //// funtion for printing status and instructions
        if(isActive){ status = "is running"; } else { status = "has stopped"; }                                              // is chessboard active?
        cout << "Steps: " << stepsCount << endl;                                                                             // steps counter
        if(!turboMode){ cout << "solveSpeed (ms): " << solveSpeed << endl; }                                                 // wait gap
        cout << "Program " << status << "." << endl << endl;                                                                 // print active status
        if(!turboMode){ cout << "(I)ncrease solveSpeed, (D)ecrease solveSpeed, (T)erminate" << endl; }                       // print instructions
    }
};


class Chessboard{                                                                                                            //// chessboard class

public:
    vector<vector<bool>> board;                                                                                              // 2D vector
    ofstream file;                                                                                                           // archive file
    Blackbox bbox;                                                                                                           // archive func
    Chessboard(int size) : board(size, vector<bool>(size, false)) {}                                                // fill board with flase (true - queen standing)

    bool isSafe(int row, int col) {                                                                                          //// check if move is safe
        for (int i = 0; i < boardSize; ++i) {                                                                                // check row and col
            if (board[row][i] || board[i][col]) {                                                                            // ========
                bbox.saveStep("NOT SAFE", row, col);                                             // send decision to archive
                return false;                                                                                                // illegal move
            }
        }
        
        for (int i = row, j = col; i >= 0 && j >= 0; --i, --j) {                                                             // check przekątna
            if (board[i][j]) {
                bbox.saveStep("NOT SAFE", row, col);                                             // send decision to archive
                return false;                                                                                                // illegal move
            }
        }
        for (int i = row, j = col; i < boardSize && j >= 0; ++i, --j) {                                                      // and another przekątna
            if (board[i][j]) {
                bbox.saveStep("NOT SAFE", row, col);                                             // send decision to archive
                return false;                                                                                                // illegal move
            }
        }

        bbox.saveStep("SAFE", row, col);                                                         // send decision to archive
        return true;                                                                                                         // legal move! place queen
    }

    bool solve(int col) {                                                                                                    //// RECURSIVE FUNCTION (!)
        if(firstStart && startingX != 0 && startingY != 0) { board[startingY - 1][startingX - 1] = true; }                   // place first queen if different than 0, 0
        
        if (col >= boardSize) {                                                                                              // check if N queens placed
            isActive = false;                                                                                                // set active to false
            system("cls");                                                                                         // clear console
            printSolution();                                                                                                 // print solution
            return true;
        }

        for (int i = 0; i < boardSize; ++i) {                                                                                // check cell one by one in row
            if (isSafe(i, col)) {                                                                                        // check if place is safe
                board[i][col] = true;                                                                                        // place queen
                bbox.saveStep("PLACED", i, col);                                                 // send decision to archive
                
                if(!turboMode){                                                                                              // just dont print board when "only solution" selected
                    system("cls");                                                                                 // clear console
                    printSolution();                                                                                         // print solution
                    this_thread::sleep_for(chrono::milliseconds(solveSpeed));                                       // wait N ms between isSafe check
                }else { system("cls"); bbox.printStatus(); }                                                       // if "only solution" then refresh only status
                
                if (solve(col + 1)) { if(turboMode){ system("cls"); } return true; }                               // recursive call (!)
                if(turboMode){ system("cls"); }                                                                    // clear console
                board[i][col] = false;                                                                                       // remove queen from 1-n col when out of options
                bbox.saveStep("REMOVED", i, col);                                                // send decision to archive
            }
        }

        return false;                                                                                                        // return false if solution is not found
    }

    void printSolution() {                                                                                                   //// funtion for printing chessboard
            if(stepsCount % 1 == 0){                                                                                         // (and 'i', 'd', 't' key press check)
                for (int key = 8; key <= 190; key++) {                                                                       // to decrese processor load change 1 to other number 
                    if (GetAsyncKeyState(key) & 0x8000) {                                                                    // get key code
                        switch(key){                                                                                         // switch function because is faster than if else
                        case 73:                                                                                             // 'i' key
                            solveSpeed += 10;                                                                                // increase gap by 10
                            break;

                        case 68:                                                                                             // 'd' key
                            if (solveSpeed >= 20) { solveSpeed -= 10; }                                                      // decrease gap just dont go to negative
                            break;

                        case 84:                                                                                             // 't' key
                            isActive = false;                                                                                // set flags to false
                            isStarted = false;                                                                               // ========
                            bbox.saveStep("TERMINATED", -2, -2);                                 // send decision to archive
                            this_thread::sleep_for(chrono::milliseconds(200));                                  // go to sleep
                            exit(0);                                                                                   // exit with code '0'

                        default:
                            continue;                                                                                        // default, not used
                        }
                    }
                }
            }
        
        cout << "     ";                                                                                                     // spaces for allign
        for (int i = 0; i < boardSize; ++i) {                                                                                // loop through each col (first row with num)
            cout << setw(2) << setfill('0') << i + 1 << "  ";                                                            // print col num with format
        }
        cout << endl;                                                                                                        // next line
        cout << "   +";                                                                                                      // plus with border
        for (int i = 0; i < boardSize; ++i) {                                                                                // ========
            cout << "---+";                                                                                                  // ========
        }
        cout << endl;                                                                                                        // next line

        for (int i = 0; i < boardSize; ++i) {                                                                                // loop through each row
            cout << setw(2) << setfill('0') << i + 1 << " |";                                                            // row num with padding
            for (int j = 0; j < boardSize; ++j) {                                                                            // loop through each col 
                cout << " " << (board[i][j]? "H" : " ") << " |";                                                             // "H" : " "   <- first for occupied, sec for empty
            }
            cout << endl;                                                                                                    // next line
            if (i < boardSize - 1) {                                                                                         // check if last row
                cout << "   +";                                                                                              // start row border
                for (int k = 0; k < boardSize; ++k) {
                    cout << "---+";                                                                                          // next row border
                }
                cout << endl;                                                                                                // next row
            }
        }
        cout << "   +";                                                                                                      // start last row border
        for (int i = 0; i < boardSize; ++i) {                                                                                // loop through each col
            cout << "---+";                                                                                                  // next border of last row
        }
        cout << endl << endl;                                                                                                // next line x2
        bbox.printStatus();                                                                                                  // print status (stepCount, isActive)
    }

    void addSolution(){                                                                                                      //// add finished chessboard to archive file

        file.open(filePath, ios::app);                                                                                 // open archive file in append mode
        if (!file.is_open()) {                                                                                               // check if open
            cerr << "Cannot save solution to output file!" << endl;                                                          // print error if closed
        }else{
            
            stringstream ss;                                                                                                 // create a stringstream to temporarily hold the output
            streambuf* coutbuf = cout.rdbuf();                                                                               // save the original cout buffer
            cout.rdbuf(ss.rdbuf());                                                                                   // redirect cout to the stringstream
            file << endl << endl;                                                                                            // add blank lines on append
            printSolution();                                                                                                 // print chessboard direct to archive file
            cout.rdbuf(coutbuf);                                                                                             // restore the original cout buffer
            file << ss.str();                                                                                                // write the contents of the stringstream to archive file
            
        }
        file.close();                                                                                                        // close archive file
    }
};

class Options                                                                                                                //// options menu
{
public:
    Blackbox bbox;
    
    void runChessboard() {                                                                                                   // run solver
        bbox.innit(false);                                                                                              // innit BlackBox (archive module)
        Chessboard chessboard(boardSize);                                                                                    // innit chessboard

        if (chessboard.solve(0)) {                                                                                       // check if solution exists
            if(turboMode){ chessboard.printSolution(); }                                                                     // if turboMode active print innit board
            cout << endl << "Solution found!" << endl;                                                                       // solution found
        } else {
            cout << "Solution NOT found!" << endl;                                                                           // solution NOT found
        }
        bbox.saveStepsToFile();                                                                                              // save steps from vector to file
        chessboard.addSolution();                                                                                            // add finished chessboard to end of archive file
        bbox.innit(true);                                                                                               // deactivate archive module
        firstStart = true;                                                                                                   // reset start flag
    }
    
    static void logo()                                                                                                       //// func for printing logo
    {

        system("cls");
        cout << endl << "MADE BY: Jakub Szubartowski (A01/C01/L02)   I Informatyka" << endl;
        cout << "               email: 31024@s.pm.szczecin.pl" << endl;
        cout << "                  TASK: PR0BLEM HETMANOW" << endl << endl;
        
    }

    void menu()                                                                                                              //// menu function
    {                                                                                                                        // print menu
        cout << endl << "[RUN (step by step)] -> 1" << endl;                                                                 // RUN displaying every step 
        cout << "[RUN (answer only)] -> 2" << endl;                                                                          // RUN displaying only solution
        cout << "[OPTIONS] -> 3" << endl;                                                                                    // change default options
        cout << "[EXIT] -> 0" << endl << endl;                                                                               // exit from program

        int option;                                                                                                          // choice validation module
        while (true){
            cout << "[CHOICE] -> ";
            cin >> option;
            if (option >= 0 && option <= 3){ break; } else{
                cout << "SELECT AGAIN" << endl;
                continue;
            }
        }

        switch(option)                                                                                                       // switch for menu
        {
        case 1:
            runChessboard();                                                                                                 // run solver step by step
            break;

        case 2:
            turboMode = true;                                                                                                // set flag
            runChessboard();                                                                                                 // run solver, solution only
            turboMode = false;                                                                                               // reset flag
            break;

        case 3:
            change_values();                                                                                                 // change default options
            break;

        default:                                                                                                             // '0' to exit or any other key
            isStarted = false;                                                                                               // get out of while loop
            break;
        }
    }

    void change_values(){                                                                                                    // func for changing default values
        string new_filename;                                                                                                 // reserving name

        cout << endl << "[OPTIONS]: " << endl;                                                                               // print options
        cout << "[BOARD: SIZE(now: " << boardSize << ")] -> 1" << endl;                                                      // board size
        cout << "[SOLVER: STEEP SPEED(now: " << solveSpeed << ")] -> 2" << endl;                                             // gap
        cout << "[EXPORT: FILENAME(now: '" << exFilename << "')] -> 3" << endl;                                              // archive file name
        cout << "[STARTING POSITION(now: "; if(startingX == 0 && startingY == 0){cout << "auto)] -> 4" << endl;              // starting position (EXPERIMENTAL might not work)
        }else { cout << "X: " << startingX << " Y: " << startingY << ")] -> 4" << endl; }                                    // ========
        cout << "[BACK] -> 0" << endl << endl;                                                                               // back to menu

        int option;                                                                                                          // choice validation module
        while (true){
            cout << "[CHOICE] -> ";
            cin >> option;
            if (option >= 0 && option <= 4){ break; } else{
                cout << "SELECT AGAIN" << endl;
                continue;
            }
        }

        switch(option)                                                                                                       // switch for default options
        {

        case 1:                                                                                                              // change board size
            short new_size_value;
            cout << "CURRENT BOARD SIZE: " << boardSize << endl;
            cout << "ENTER NEW VALUE (short, positive && not zero, MAX " << maxBoardSize << "): ";
            cin >> new_size_value;
            if (new_size_value > 0 && new_size_value <= maxBoardSize){ boardSize = new_size_value; }else{ cout << "WRONG VALUE!" << endl; }
            break;

        case 2:                                                                                                              // change gap time
            unsigned short new_time_value;
            cout << "CURRENT STEEP SPEED: " << solveSpeed << endl;
            cout << "ENTER NEW STEEP SPEED (short, positive && not zero, MAX 65530): ";
            cin >> new_time_value;
            if (new_time_value > 0 && new_time_value <= 65530){ solveSpeed = new_time_value; }else{ cout << "WRONG VALUE!" << endl; }
            break;

        case 3:                                                                                                              // change archive module default file name
            cout << "CURRENT FILENAME: '" << exFilename << "'" << endl;
            cout << "ENTER NEW VALUE (string): ";
            cin.ignore();
            getline(cin, new_filename);
            exFilename = new_filename;
            break;

        case 4:                                                                                                              // change starting position (EXPERIMENTAL)
            short new_X_value;
            short new_Y_value;
            
            cout << "CURRENT BOARD SIZE X: " << startingX << " Y: " << startingY << endl;                                    // display current starting position
            cout << "ENTER NEW X VALUE (short, positive, MAX " << boardSize << "): ";                                        // change X value
            cin >> new_X_value;
            if (new_X_value <= boardSize){ startingX = new_X_value; }else{ cout << "WRONG VALUE!" << endl; }

            cout << "ENTER NEW Y VALUE (short, positive, MAX " << boardSize << "): ";                                        // change Y value
            cin >> new_Y_value;
            if (new_Y_value <= boardSize){ startingY = new_Y_value; }else{ cout << "WRONG VALUE!" << endl; }
            break;

        case 0:
            cout << endl;                                                                                                    // go back to main menu
            break;

        default:
            cout << "DEFAULT OPTION - NOT USED" << endl;                                                                     // not used option
            break;
            
        }
    }
};


int main(){                                                                                                                  // main function, entry point of program
    SetConsoleTitle(title);                                                                                                  // change title of consol to 'OneChess'
    Options opt;                                                                                                             // create instance of options
    opt.logo();                                                                                                              // print logo
    while (isStarted){ opt.menu(); }                                                                                         // loop program when flag is set to true
    
    return 0;                                                                                                                // return 0
}
