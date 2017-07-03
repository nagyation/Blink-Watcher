#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <syslog.h>

//Directory to be watched
#define WATCH_DIR "/home/nagy/workspace/C/focus/"

//Excutable file name to be run at this directory
#define EXECUTABLE_FILE "focus"

/* static void daemonize(void) */
/* { */
/*     pid_t pid, sid; */

/*     /\* already a daemon *\/ */
/*     if ( getppid() == 1 ) return; */

/*     /\* Fork off the parent process *\/ */
/*     pid = fork(); */
/*     if (pid < 0) { */
/*         exit(EXIT_FAILURE); */
/*     } */
/*     /\* If we got a good PID, then we can exit the parent process. *\/ */
/*     if (pid > 0) { */
/*         exit(EXIT_SUCCESS); */
/*     } */

/*     /\* At this point we are executing as the child process *\/ */

/*     /\* Change the file mode mask *\/ */
/*     umask(0); */

/*     /\* Create a new SID for the child process *\/ */
/*     sid = setsid(); */
/*     if (sid < 0) { */
/*         exit(EXIT_FAILURE); */
/*     } */

/*     /\* Change the current working directory.  This prevents the current */
/*        directory from being locked; hence not being able to remove it. *\/ */
/*     if ((chdir("/")) < 0) { */
/*         exit(EXIT_FAILURE); */
/*     } */

/*     /\* Redirect standard files to /dev/null *\/ */
/*     freopen( "/dev/null", "r", stdin); */
/*     freopen( "/dev/null", "w", stdout); */
/*     freopen( "/dev/null", "w", stderr); */

/* } */

int main (){

//    daemonize();
    // file descriptor and watch descriptor
    int fd,wd;

    syslog (LOG_NOTICE, "Blink Watcher Started");
    closelog();
    
    fd = inotify_init();
    if (fd == -1)
    {
	fprintf(stderr, "inotify_init failed");
	exit(EXIT_FAILURE);
    }
    
    wd = inotify_add_watch(fd,WATCH_DIR,IN_CREATE | IN_CLOSE_WRITE);
    if (wd == -1)
    {
	fprintf(stderr, "add watch failed");
	exit(EXIT_FAILURE);
    }

    struct inotify_event *nevent = malloc(sizeof(struct inotify_event));

    int err;
    while (1){

	err = read(fd,nevent,sizeof(struct inotify_event) + FILENAME_MAX + 1);
	if(err == -1) // if was an error try again
	  continue;

	// for non daemon debuging
	// Name of the file
	printf("Name %s\n",nevent->name);
	
	//print 1 or 0 for true and false if created or modified
	printf("is create %d\n", nevent->mask == IN_CREATE );
	printf("is modifed %d\n", nevent->mask == IN_CLOSE_WRITE );

	// make Makefile at the watch_dir
	system("make -C " WATCH_DIR);
	system("chmod 777 " WATCH_DIR EXECUTABLE_FILE);
	// kill the already running app
	system("pkill " EXECUTABLE_FILE);
	// run the executable file
	//system(WATCH_DIR  EXECUTABLE_FILE "&");
    }
    free(nevent);
}
