#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/applications_pool.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

extern char **environ;
long int delay_in_microsecs = 10000000; // default wait time of 10 secs
long int delay_in_subsequent_loops = 985000; // 0.01s = 10ms = 10000, 0.985s = 985000
bool first_time_access = true;

void to_wait_or_not_to_wait()
{
    char command[50];

    if( access( "/home/vcap/app/first.txt", F_OK ) != -1 ) {
    // file exists, so this App Instance has been accessed before
    usleep(delay_in_subsequent_loops); // sleeps for 0.985s
    first_time_access = false;
    } else
    {
    // file doesn't exist, 1st time access to App Instance
    usleep(delay_in_microsecs); /* wait delay_in_microseconds before continuing */
    strcpy(command, "touch /home/vcap/app/first.txt" ); // create file
    system(command);
    first_time_access = true;
    }
};

class hi : public cppcms::application 
{
public: 
       hi(cppcms::service &srv) : 
           cppcms::application(srv) 
    {
        dispatcher().assign("/health",&hi::health,this);
        mapper().assign("health","/health");

        dispatcher().assign("/",&hi::welcome,this);
        mapper().assign("");

        mapper().root("/");
    }
    /* virtual void main(std::string url); */

    void health()
    {
    to_wait_or_not_to_wait();
    response().out() << ":-) <br/>\n"; 
    response().out() << "<a href='" << url("/") << "'>Go back</a>";
    };

    void welcome()
    {
    int j=1,i,mem_size;
    char *s = *environ;
    char out_str[2000];
    char *ptr;

    to_wait_or_not_to_wait();

    response().out()<<
        "<html>\n"
        "<body>\n"
        "<font color=\"black\">\n";

    if (!first_time_access) sprintf(out_str,"<h2> Not the 1st time this App Instance has responded. </h2>\n"); 
      // else sprintf(out_str,"<h2> I waited %ld seconds before responding for the 1st time. </h2>\n", delay_in_microsecs/1000000); 
       else sprintf(out_str,"<h2><font color=\"red\">I waited %ld seconds before responding for the 1st time.</font></h2>\n", delay_in_microsecs/1000000); 

    response().out() << out_str;

    for (; s; j++) { // print all environment variables                                                       
                     sprintf(out_str,"<h2> %s  </h2>\n", s);  /* fflush(stdout); */
                     switch (j) { case  1: 
                                  case 14: 
                                  case 24: response().out() << out_str; 
                                };
                     s = *(environ+j);
                     // response().out() << out_str; s = *(environ+j);  // without the if (strstr()), will print all environment variables 
                   }  

  /* srand((unsigned int)time(NULL)); mem_size=rand() % 300 + 20; */
  /* mem_size=100; */ 

   mem_size=0;  

  /* response().out() << "<h2> I will enventually crash due to lack of memory </h2>\n";
     sprintf(out_str, "<h2> Chosen random amount of memory: %d MB </h2>\n",mem_size);
     response().out() << out_str;
  */
   /*
   if ((ptr=(char *)malloc(mem_size*1024*1024))) { sprintf(out_str,"<h2> %d MB of additional memory reserved </h2>\n",mem_size); 
                                                   response().out() << out_str;
                                                   for (i=0; i<mem_size*1024*1024; i++) *ptr++='0';
                                                 }
              else response().out() << "<h2> Failed to reserve memory </h2>\n";
   */
   response().out() <<
        "</font>\n"
        "</body>\n"
        "</html>\n";
   };
};

int main(int argc,char ** argv)
{
   try {
        cppcms::service srv(argc,argv);
        srv.applications_pool().mount(cppcms::applications_factory<hi>());
        srv.run();
    }
    catch(std::exception const &e) {
        std::cerr<<e.what()<<std::endl;
    }
}

