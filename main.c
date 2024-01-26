#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "sys/wait.h"
#include "time.h"
#include "signal.h"
#include "sys/types.h"

#define HARCRA_FEL SIGUSR1

struct Oltas {
    char name[50];
    int year;
    char phone[11];
    int pay;
    int vaccinated;
};

void handler(int sign){
    printf("Harcra fel!\n");
}

int* create_pipe() {
    int* p = (int*)malloc(2*sizeof(int)); //pipe tömb
    if(pipe(p) == -1){ // create pipe
        printf("Hiba a csovezetek letrehozaskor\n");
        exit(EXIT_FAILURE);
    }
    return p;
}
void del_pipe(int descriptor[2]){
    free(descriptor); // malloc miatt 
    descriptor = NULL;
}

void add_new_line(struct Oltas *array, int *count);
void update_line(struct Oltas *array, int *count);
void delete_line(struct Oltas *array, int *count);
void list_all_data(struct Oltas *array, int *count);
void set_all_unvaccinated(struct Oltas *array, int *count);
void vaccination(struct Oltas *array, int *count);



int main(int argc, char** argv) {
    //we can handle max 1000 data in this program 
    struct Oltas tmp_oltasadat[1000];
    static int count = 0;

    //You have to give a filename, it can be either exist or not
    if (argc!=2) { 
        perror("You have to give a filename for the stored data");
        exit(1);
    }
    
    //Check if file is exist or not, if exist, read the data from it to temporary array of data
    if( access( argv[1], F_OK ) == 0 ) { // megnyitás / elérés
        FILE *infile;
        struct Oltas input;
        infile = fopen(argv[1], "r");
        int i = 0;
        while(fread(&input, sizeof(struct Oltas), 1, infile)) { // inputba (azaz egy adott oltas adatai) és filebol
            tmp_oltasadat[i] = input; // oltasok
            i++;
            printf("Just read person: %s\n", input.name);
        }
        fclose(infile);
    } 

    

    //how many lines of data we have, specially after reading data file
    while(tmp_oltasadat[count].name[0] != '\0') {
        count++;
    }
    //printf("We have %d number of lines \n",count);

    //now, you can decide to enter new data, update or list existing lines:
    while (1) {
        printf("\nWhat do you want? Type:\n new - new data,\n update - update data,\n del - delete data,\n list - list data,\n stop - stop program\n unvac - all person unvaccinated,\n vacc - start vaccination\n");
        char answer[10];
        scanf("%s", answer);
        if (strcmp(answer, "stop") == 0) {
            printf("You have chosen stop!\n");
            break;
        } else if (strcmp(answer, "new") == 0) {
            printf("You have chosen adding new data!\n");
            add_new_line(tmp_oltasadat,&count);
        } else if (strcmp(answer, "update") == 0) {
            printf("You have chosen updating data!\n");
            update_line(tmp_oltasadat,&count);
        } else if (strcmp(answer, "del") == 0) {
            printf("You have chosen deleting data!\n");
            delete_line(tmp_oltasadat,&count);
        } else if (strcmp(answer, "list") == 0) {
            printf("Listing all element!\n");
            printf("We have %d number of element..\n\n",count);
            list_all_data(tmp_oltasadat,&count);
        } else if (strcmp(answer, "unvac") == 0) {
            printf("Set all person unvaccinated!\n");
            printf("We have %d number of element..\n\n",count);
            set_all_unvaccinated(tmp_oltasadat,&count);
        } else if (strcmp(answer, "vacc") == 0) {
            printf("Vaccitation started!\n");
            printf("We have %d number of element..\n\n",count);
            vaccination(tmp_oltasadat,&count);
        } else { 
            //
        };
    }

    //After exit interaction loop, save the new data to the file
    printf("Saving data before existing...");
    FILE *outfile;
    struct Oltas temp_out;
    outfile = fopen (argv[1], "w");
    if (outfile == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }
    /*for (int i = 0; i < count; i++) {
        temp_out = tmp_oltasadat[i];
        fwrite (&temp_out, sizeof(struct Oltas), 1, outfile);
        printf("Saving person: %s\n",temp_out.name);
    }*/
    fwrite(tmp_oltasadat,sizeof(struct Oltas),count,outfile);
    if(fwrite != 0) 
        printf("contents to file written successfully !\n");
    else 
        printf("error writing file !\n");
  
    fclose (outfile);

    return 0;
}

void add_new_line(struct Oltas *array, int *count) {
    struct Oltas tmp;
    //char f_name[10];
    char l_name[50];
    printf("First name: ");
    scanf ("%s", tmp.name);
    printf("Last name: ");
    scanf ("%s", l_name);
    strcat(tmp.name, " ");
    strcat(tmp.name, l_name);
    printf("Birth year: ");
    scanf ("%d", &tmp.year);
    printf("Phone: ");
    scanf ("%s", tmp.phone);
    printf("Pay service? Type 1 for yes, 0 for no: ");
    scanf ("%d", &tmp.pay);
    printf("Vaccinated? Type 1 for yes, 0 for no: ");
    scanf ("%d", &tmp.vaccinated);
    array[*count] = tmp;
    *count = *count + 1;
}

//TODO: Asking the customer, which data he want to update
//TODO: scanf() did not work well with whitespaces, gets() also did not work
void update_line(struct Oltas *array, int *count) {
    char tmp_name[50];
    char l_name[50];
    struct Oltas tmp;
    printf("First name: ");
    scanf ("%s", tmp_name);
    printf("Last name: ");
    scanf ("%s", l_name);
    strcat(tmp_name, " ");
    strcat(tmp_name, l_name);
    printf("name: %s\n", tmp_name); // Beolvas egy teljes nevet összekonkatenációval
    int index = 0;
    for (index; index < *count; index++) {
        if (strcmp(array[index].name, tmp_name) == 0) { // amíg megtalálja a nevet a nevek között
            printf("Found a matching name..."); 
            break;
        }
    }
    if (index < *count) {
        printf("Please enter the new values\n");
        printf("Birth year: ");
        scanf ("%d", &tmp.year);
        printf("Phone: ");
        scanf ("%s", tmp.phone);
        printf("Pay service? Type 1 for yes, 0 for no: ");
        scanf ("%d", &tmp.pay);
        printf("Vaccinated? Type 1 for yes, 0 for no: ");
        scanf ("%d", &tmp.vaccinated);
        array[index].year = tmp.year;
        strcpy(array[index].phone,tmp.phone);
        array[index].pay = tmp.pay;
        array[index].vaccinated = tmp.vaccinated;
    } else {
        printf("I did not found a matching name.\n");
    }
}

//Could be trash in array after *count-nth element
void delete_line(struct Oltas *array, int *count) {
    char tmp_name[50];
    char l_name[50];
    struct Oltas tmp;
    printf("First name: ");
    scanf ("%s", tmp_name);
    printf("Last name: ");
    scanf ("%s", l_name);
    strcat(tmp_name, " ");
    strcat(tmp_name, l_name);
    printf("name: %s\n", tmp_name);
    int index = 0;
    for (index; index < *count; index++) {
        if (strcmp(array[index].name, tmp_name) == 0) {
            printf("Found a matching name...");
            break;
        }
    }
    if (index < *count) {
        for (index; index<*count-1; index++) {
            array[index] = array[index+1]; // Kellő mód tüntetem el
        }
        *count = *count-1;
    } else {
        printf("I did not found a matching name.\n");
    }
}

void list_all_data(struct Oltas *array, int *count) {
    for (int i = 0; i < *count; i++) {
        printf("Name: %s, birth year: %d, phone number: %s, paying: %d, vaccinated: %d\n",array[i].name,array[i].year,array[i].phone,array[i].pay, array[i].vaccinated);
    }
    printf("\n");
}

void set_all_unvaccinated(struct Oltas *array, int *count) {
    for (int i = 0; i < *count; i++) {
        array[i].vaccinated = 0;
    }
}
 
void vaccination(struct Oltas *array, int *count) {
    int unvaccinated = 0;
    //get the number of people who are not vaccinated
    for (int i = 0; i < *count; ++i) {
        if (array[i].vaccinated == 0) {
            unvaccinated++;
        }
    }
    
    
    
    pid_t bus1, bus2;
    signal(HARCRA_FEL,handler); //handler can handel the custom signal
    int* pipe1 = create_pipe();
    int* pipe2 = create_pipe();
    int* pipe1_back = create_pipe();
    int* pipe2_back = create_pipe();
    //int* succes_bus1 = 0;
    //int succes_bus2 = 0;
    char stop_str[50] = "stop";
    
    //decide if we need 0, 1 or 2 bus
    if (unvaccinated > 4) {
        bus1 = fork();
        srand(getpid()); //rand started for 1. child
        if (bus1 == 0) { //in first child 
            kill(getppid(),HARCRA_FEL);    
            //printf("First child: %d \n",getpid());
            sleep(1); //waiting for the first child's parent write in pipe
            close(pipe1[1]);
            //temprary array
            char tmp_array[5][50];
            for(int i = 0; i<5; i++) {
                read(pipe1[0],tmp_array[i],50);
            }
            close(pipe1[0]);
            close(pipe1_back[0]);
            for(int i = 0; i<5; i++) {
                
                if (rand()%100 < 90) {
                    //printf("%s\n", tmp_array[i]);    
                    write(pipe1_back[1],tmp_array[i],50);
                    
                }
            }
            
            write(pipe1_back[1],stop_str,50);
            close(pipe1_back[1]);
            exit(0);
        } else { //in first child's parent
            pause(); //get the first signal
            int limit = 0;
            int index = 0;
            close(pipe1[0]);
            while (limit < 5) { //if the bus1 started, we have at least 5 unvaccinated people
                if(array[index].vaccinated == 0) {
                    printf("%s was sent to bus1\n",array[index].name);
                    write(pipe1[1],array[index].name,50);
                    limit++;
                
                }
                index++;
            }
            close(pipe1[1]);
            //printf("Parent of first child: %d \n",getppid());
        }
        
    }
    
    if (unvaccinated > 9) {
        srand(time(0));
        bus2 = fork();
        if (bus2 == 0) { //in second child 
            kill(getppid(),HARCRA_FEL);
            sleep(3);
            close(pipe2[1]);
            //temprary array
            char tmp_array_bus2[5][50];
            for(int i = 0; i<5; i++) {
                read(pipe2[0],tmp_array_bus2[i],50);
            }
            close(pipe2[0]);
            close(pipe2_back[0]);
            for(int i = 0; i<5; i++) {
                if (rand()%100 < 90) {
                    //printf("%s\n", tmp_array_bus2[i]);    
                    write(pipe2_back[1],tmp_array_bus2[i],50);
                }
            }
            
            write(pipe2_back[1],stop_str,50);
            close(pipe2_back[1]);
            //printf("Second child: %d \n",getpid());
            exit(0);
                
        } else { //in second child's parent
            pause();  
            int limit = 0;
            int bus1_index = 0;
            int index = 0;
            close(pipe2[0]);
            while (limit < 5) { //if the bus1 started, we have to skip the first 5 unvaccinated people and max number of people we send is 5               
                    if(array[index].vaccinated == 0) {
                        //
                        //
                        bus1_index++;
                        if (bus1_index > 5) {
                            printf("%s was sent to bus2\n",array[index].name);
                            write(pipe2[1],array[index].name,50);
                            limit++;
                        }
                    }
                index++;
            }
            close(pipe2[1]);
            //printf("Parent of second child: %d \n",getppid());
        }
        
    }
    
    wait(&bus1);
    wait(&bus2);
    //if there was no vaccination, we have to stop the pipe reading
    if (unvaccinated < 5) {
        printf("Today we can't start a vaccination bus, sadly :(");
        write(pipe1_back[1],stop_str,50);
        write(pipe2_back[1],stop_str,50);
    }
    
    //read from 1. bus
    char tmp_name[50];
    close(pipe1_back[1]);
    
    while(strcmp(tmp_name,"stop") != 0) {
        read(pipe1_back[0],tmp_name,50);
        printf("Succesfully vaccinated: %s\n",tmp_name);
        for (int i = 0; i<*count; i++) {
            if (strcmp(tmp_name,array[i].name) == 0) {
                array[i].vaccinated = 1;
            }
        }
    } //
    close(pipe1_back[0]);
    
    //read from 2. bus
    char tmp_name_2[50];
    close(pipe2_back[1]);
    
    while(strcmp(tmp_name_2,"stop") != 0) {
        read(pipe2_back[0],tmp_name_2,50);
        printf("Succesfully vaccinated: %s\n",tmp_name_2);
        for (int i = 0; i<*count; i++) {
            if (strcmp(tmp_name_2,array[i].name) == 0) {
                array[i].vaccinated = 1;
            }
        }
    } //
    close(pipe2_back[0]);
    
    //printf("Children stopped running, I am the parent.");
}
