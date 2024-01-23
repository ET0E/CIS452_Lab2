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

struct machine* create_machine(){
    struct passwd* passwd = getpwuid(getuid());

    const char* filepath = "/home/ingramrt";
    char result[10];

    struct group* group = getgrgid(getgid());

    struct stat st;

    struct utsname utsname_data;

    struct machine* machine = malloc(sizeof(struct machine));

    strcpy(machine->user, passwd->pw_name);
    machine->user_id = passwd->pw_uid;

    //machine->group = group->gr_name;
    strcpy(machine->group, group->gr_name);
    machine->group_id = group->gr_gid;

    //machine->home = passwd->pw_dir;
    strcpy(machine->home, passwd->pw_dir);

    stat(filepath, &st);
    permission_to_string(st.st_mode, result);
    strcpy(machine->home_permission, result);
    machine->home_permission_num = st.st_mode;

    strcpy(machine->login_shell, passwd->pw_shell);

    int host = uname(&utsname_data);
    strcpy(machine->host, utsname_data.nodename);

    strcpy(machine->system_name, utsname_data.sysname);
    strcpy(machine->system_release, utsname_data.release);

    return machine;
}

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

int main()
{
    struct machine* machine = create_machine();
    print_machine(machine);
    //char *me;
    //me = getenv("USER");
    //printf ("My login id is %s\n", me);
    //free(machine->home_permission);
    //free(machine->host);
    //free(machine->system_name);
    //free(machine->system_version);
    free(machine);
    return 0;
}
