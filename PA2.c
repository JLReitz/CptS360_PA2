#include "PA2.h"

//#define DEBUG_PARSING
//#define DEBUG_FINDCMD

// GLobal Variables ***************************************************************************

extern Node * _root, * _cwd;
extern char _line[128];
extern char _command[16], _pathname[64];
extern char _dirname[64], _basename[64];


int main()
{
    initialize();

    int cmd_ID = 0, cont = 1;;

        while(cont)
        {
            //Read line in from user
            printf("Command >> ");
            //gets(_line);
            strcpy(_line, "mkdir /hello\n");

            //Parse the string into _command[] and _pathname[]
            parse_line();

#ifdef DEBUG_PARSING
            //Print the parsed strings to ensure they are being seperated correctly
            printf("Command: %s\n", _command);
            printf("Pathname: %s\n", _pathname);
#endif

            cmd_ID = findCommand(_command);

#ifdef DEBUG_FINDCMD
            //Print the found command
            printf("%d\n", cmd_ID);
#endif

            switch(cmd_ID)
            { 
            case 0: 
            
                menu();
                break;

            case 1: 
            
                mkdir(_pathname);
                break;

            case 2: 
            
                rmdir();
                break;

            case 3: 
            
                ls();
                break;

            case 4: 
            
                cd();
                break;

            case 5: 
            
                pwd();
                break;

            case 6: 
                
                creat();
                break;

            case 7: 
                
                rm();
                break;

            case 8: 
                
                reload();
                break;

            case 9: 
                
                save();
                break;

            case 10:

                cont = 0;
                break;
            }
        }
}