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
    //use sprintf to print to
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
    char* user;
    int user_id;
    char* group;
    int group_id;
    char* home;
    char* home_permission;
    int home_permission_num;
    char* login_shell;
    char* host;
    char* system_name;
    char* system_version;
};

struct machine* create_machine(){
    struct passwd* passwd = getpwuid(getuid());

    const char* filepath = "lab2_sample.c";
    char *result;

    struct group* group = getgrgid(getgid());

    struct stat st;

    struct utsname utsname_data;

    struct machine* machine = malloc(sizeof(struct machine));

    machine->user = passwd->pw_name;
    machine->user_id = passwd->pw_uid;

    machine->group = group->gr_name;
    machine->group_id = group->gr_gid;

    machine->home = passwd->pw_dir;

    stat(filepath, &st);
    permission_to_string(st.st_mode, result);
    machine->home_permission = strdup(result);
    machine->home_permission_num = st.st_mode;

    machine->login_shell = passwd->pw_shell;

    int host = uname(&utsname_data);
    machine->host = strdup(utsname_data.nodename);

    machine->system_name = strdup(utsname_data.sysname);
    machine->system_version = strdup(utsname_data.release);

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
    printf("System               :  %s %s\n", machine->system_name, machine->system_version);
}

int main()
{
    struct machine* machine = create_machine();
    print_machine(machine);
    //char *me;
    //me = getenv("USER");
    //printf ("My login id is %s\n", me);
    free(machine->home_permission);
    free(machine->host);
    free(machine->system_name);
    free(machine->system_version);
    free(machine);
    return 0;
}
