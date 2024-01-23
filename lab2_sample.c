/* sample.c */
/*Tristan Ingram-Reeve*/
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>
#include <grp.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>

/*********************************************************
 * Code from ChatGPT, takes a permission as input and
 * translates to the human readable read, write, execute 
 * permissions. The S_IRUSER etc. can be found on the 
 * stat(2) manual page. 
*********************************************************/
void permission_to_string(mode_t permission, char* result) {
    //use sprintf to print to put into result the formatted string
    sprintf(result, "%c%c%c%c%c%c%c%c%c",
            //can user read, write, execute
            (permission & S_IRUSR) ? 'r' : '-',
            (permission & S_IWUSR) ? 'w' : '-',
            (permission & S_IXUSR) ? 'x' : '-',
            //can group read, write, execute
            (permission & S_IRGRP) ? 'r' : '-',
            (permission & S_IWGRP) ? 'w' : '-',
            (permission & S_IXGRP) ? 'x' : '-',
            //can others read, write, execute
            (permission & S_IROTH) ? 'r' : '-',
            (permission & S_IWOTH) ? 'w' : '-',
            (permission & S_IXOTH) ? 'x' : '-');
}

/*********************************************
 * This struct initializes variables that the 
 * struct machine will print. All the strings
 * are initialized to a fixed array size of 50
 * to make sure they have space. 
*********************************************/
struct machine{
    char user[50];
    int user_id;
    char group[50];
    int group_id;
    char home[50];
    char home_permission[50];
    int home_permission_num;
    char login_shell[50];
    char host[50];
    char system_name[50];
    char system_release[50];
};

/*******************************************
 * This helper method "creates the machine"
 * This method is where each of the struct's
 * variables with a value to print. 
*******************************************/
struct machine* create_machine(){

    //this struct is to obtain the user's 
    //name, directory, id, and shell the 
    //user is using
    struct passwd* passwd = getpwuid(getuid());

    //filepath and result are used when obtaining
    //the home permissions
    const char* filepath = "/home/ingramrt";
    char result[10];

    //group struct gets the group id and group name
    struct group* group = getgrgid(getgid());

    //stat struct gets the permissions of the user
    struct stat st;

    //utsname struct gets the hostname, system_name, and the system_release
    struct utsname utsname_data;

    //allocating space for the machine 
    struct machine* machine = malloc(sizeof(struct machine));

    //copy username into machine user and user id into machine user id
    strcpy(machine->user, passwd->pw_name);
    machine->user_id = passwd->pw_uid;
    
    strcpy(machine->group, group->gr_name);
    machine->group_id = group->gr_gid;

    strcpy(machine->home, passwd->pw_dir);

    //must call stat to retrieve the file permissions using st.st_mode
    //calls permission_to_string to get the human readable permissions
    stat(filepath, &st);
    permission_to_string(st.st_mode, result);
    strcpy(machine->home_permission, result);
    machine->home_permission_num = st.st_mode;

    strcpy(machine->login_shell, passwd->pw_shell);

    //uname allows the programmer to obtain the host and the system name
    uname(&utsname_data);
    strcpy(machine->host, utsname_data.nodename);

    strcpy(machine->system_name, utsname_data.sysname);
    strcpy(machine->system_release, utsname_data.release);

    return machine;
}

//helper method that prints the different components of machine
void print_machine(struct machine* machine){
    printf("About me\n========\n");
    printf("Unix User            :  %s (%d)\n", machine->user, machine->user_id);
    printf("Name                 :  Tristan Ingram-Reeve\n");
    printf("Unix Group           :  %s (%d)\n", machine->group, machine->group_id);
    printf("Unix Home            :  %s\n", machine->home);
    printf("Home Permissions     :  %s (%d)\n", machine->home_permission, machine->home_permission_num);
    printf("Login Shell          :  %s\n\n", machine->login_shell);
    printf("About this machine\n==================\n");
    printf("Host                 :  %s\n", machine->host);
    printf("System               :  %s %s\n", machine->system_name, machine->system_release);
}

//main that creates a mahcine then prints its components to the screen
int main()
{
    struct machine* machine = create_machine();
    print_machine(machine);
    free(machine);
    return 0;
}
