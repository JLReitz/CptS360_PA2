#ifndef PA2_H
#define PA2_H

//#define DEBUG_MKDIR

typedef struct NODE
{
    char name[64];
    char type;
    struct NODE *p_child, *p_sibling, *p_parent;
} Node;

// Global Variables ***************************************************************************

Node * _root, * _cwd; //root and CWD pointers
char _line[128]; //user input line
char _command[16], _pathname[64]; //user inputs
char _dirname[64], _basename[64]; //string holders
char _global_temp[128]; //Global temporary string

// Functions **********************************************************************************

void initialize()
{
    _root = malloc(sizeof(Node));

    strcpy(_root->name, "/");
    _root->type = 'D';
    _root->p_child = 0;
    _root->p_parent = 0;
    _root->p_sibling = 0;

    _cwd = _root;
}

//Parses '_line[]' and places the given command and pathname into their respective buffers
//  -   A string must be read into '_line[]' before calling this function for it to be effective
void parse_line()
{
    sscanf(_line, "%s %s", _command, _pathname);
}

void find_dirname(char * pathname)
{
    int i;

    //Copy 'pathname' into a temporary string
    char str[64] = "";
    strcpy(str, pathname);

    //Find the last '/' character
    for(i=strlen(str); str[i]!='/'; i--)
    {}

    str[i] = '\0';

    strcpy(_dirname, str);
}

void find_basename(char * pathname)
{
    int i, j = 0;

    //Copy 'pathname' into a temporary string
    char str[64] = "", str2[64] = "";
    strcpy(str, pathname);

    //Find the last '/' character
    for(i=strlen(str); str[i]!='/'; i--)
    {}

    i++;

    for(i; i<strlen(str); i++)
    {
        str2[j++] = str[i];
    }

    strcpy(_basename, str2);
}

void get_basename(char * pathname)
{
    int i, j = 0;

    //Copy 'pathname' into a temporary string
    char str[64] = "", str2[64] = "";
    strcpy(str, pathname);

    //Find the last '/' character
    for(i=strlen(str); str[i]!='/'; i--)
    {}

    i++;

    for(i; i<strlen(str); i++)
    {
        str2[j++] = str[i];
    }

    strcpy(_global_temp, str2);
}

Node * search_dir(char * dirname, char * error)
{
    char * str = "";
    Node ** curr; //Node pointer for the current location within the search function

    //Search for the directory to do work on
    if(dirname[0] == '/') //If the given directory is absolute
    {
        curr = _root; //Set 'curr' to '_root'
        str = strtok(dirname, "/"); //Run strtok once to remove the first '/'
        str = strtok(0, "/"); //Now run it again to grab the next directory name
    }
    else
    {
        curr = _cwd; //Set 'curr' to '_cwd'
        str = strtok(dirname, "/"); //Run strtok to grab the next directory name
    }

    if(str == "\0")
        return curr;

    //Now that that has been initialized, run through the rest of the filesystem until that
    //  directory is found or the end is reached
    while(*curr)
    {
        //Run through every sibling
        while(*curr)
        {
            //Grab the basename of the current node
            get_basename((*curr)->name);

            //Check to see if this is the next directory name
            if(!(strcmp(str, _global_temp)))
            {
                str = strtok(0, "/"); //Find the next directory to move to

                //If it is, check to make sure it's a directory
                if((*curr)->type == 'D')
                {
                    if(!str) //If we have reached the end of 'dirname'
                        return curr;
                    else
                        curr = &((*curr)->p_child); //Go to that child

                    break;
                }
                else
                {
                    strcpy(error, "t"); //Incompatible type error
                    return 0;
                }
            }
            else
            {
                //Else, go to the next sibling
                curr = &((*curr)->p_sibling);
            }
        }
    }

    strcpy(error, "p");
    return 0;
}

int check_for_child(Node ** curr, char * basename, char * error)
{
    //Run through every sibling
    while(*curr)
    {
        //Grab the basename of the current node
        get_basename((*curr)->name);

        //Check to see if this is the next directory name
        if(!(strcmp(basename, _global_temp)))
        {
            //If it is, check to make sure it's a directory
            if((*curr)->type == 'D')
            {
                strcpy(error, "s"); //Same type error
                return 1;
            }
            else
            {
                strcpy(error, "t"); //Incompatible type error
                return 1;
            }
        }
        else
        {
            //Else, go to the next sibling
            curr = &((*curr)->p_sibling);
        }
    }

    return 0;
}

Node * make_node(char * name, char type, Node * parent)
{
    Node * newNode = malloc(sizeof(Node));

    strcpy(newNode->name, name);
    newNode->type = type;
    newNode->p_parent = parent;
    newNode->p_child = 0;
    newNode->p_sibling = 0;
}

void menu()
{

}

void mkdir(char * pathname)
{
    char * error = "";
    Node * action_dir, ** child_dir;

    //Break up 'pathname' into '_dirname' and '_basename'
    find_dirname(_pathname);
    find_basename(_pathname);

#ifdef DEBUG_MKDIR
    printf("Dirname: %s\n", _dirname);
    printf("Basename: %s\n", _basename);
#endif

    if(_dirname == "\0")
        action_dir = _root;
    else
        action_dir = search_dir(_dirname, error);

    if(action_dir)
    {
        if(check_for_child(&action_dir, _basename, error))
        {
            if(!(strcmp("s", error)))
                printf("There already exists a directory with this name\n");
            if(!(strcmp("t", error)))
                printf("There already exists a file with this name\n");
        }
        else
        {
            *child_dir = action_dir->p_child;

            while((*child_dir)->p_sibling)
                *child_dir = (*child_dir)->p_sibling;

            (*child_dir)->p_sibling = make_node(_pathname, 'D', *child_dir);
        }
    }
    else
    {
        if(!(strcmp("t", error)))
                printf("There already exists a file in the specified pathname with this name\n");
        if(!(strcmp("p", error)))
            printf("There is no pathname that matches the specified location\n");
    }
}

void rmdir()
{

}

void ls()
{

}

void cd()
{

}

void pwd()
{
    printf("%s\n", _cwd->name);
}

void creat()
{

}

void rm()
{

}

void reload()
{

}

void save()
{

}

//Determines whether a valid file command is stored within a c string.
//  been given
//  [INPUT] -- 'command' -- char * -- string containing the command
//  [OUTPUT] -- int -- returns an index value for the given command
int findCommand(char * command)
{
    int i = 0;
    char * cmd[] = {"menu", "mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm", "reload", 
                    "save", "quit", 0};

    while(cmd[i])
    {
        if (strcmp(command, cmd[i])==0)
            return i;

        i++;
    }

    return -1;
}


#endif