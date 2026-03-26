#include "includes/defs.h"


// client.c
// Will change back to this , rn just testing it 
// void *receiveFromDashboard(int sockfd, int brake_coid, int throttle_coid, int steering_coid) {

int sockfd;
struct sockaddr_in dest;
name_attach_t *attach;

void shutdown_client(int signo) {
    exit(0);  // triggers atexit handlers safely
}

void cleanup_client(void) {
    printf("[CLIENT] Exiting, detaching name...\n");
    name_detach(attach, 0);
}

void main_qnx_receiver(int sockfd, int brake_coid , int drive_coid) {
    char buffer[1024];
    
    while (1) {
        int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, NULL, NULL);
        if (n < 0) continue;
        buffer[n] = '\0';

        // parse JSON into msg_packet
        msg_packet p;
        json_to_msg_packet(buffer, &p);

<<<<<<< HEAD
        //These are the controls that the dashboard is sending 
        // route to correct process
<<<<<<< HEAD
<<<<<<< HEAD

        // Data from dashboard to webot.c for sending to webots
        if (strcmp(p.msg_type, "Webots") == 0) {
=======
        if (strcmp(p.msg_type, "BrakingInput") == 0) {
>>>>>>> 9068e1e ( removed data not included in the telemetry the dashboard is waiting on)
=======

        // Data from dashboard to webot.c for sending to webots
        if (strcmp(p.msg_type, "Webots") == 0) {
>>>>>>> e81f985 ( Work in progress just fixing some stuff , abit broken atm)
=======
        //From here then on send the data to appropriate processes , braking etc 
        if (strcmp(p.subsys, "Brake") == 0) {
            MsgSend(brake_coid, &p, sizeof(p), NULL, 0);
            printf("[CLIENT] : Received Braking data from Dashboard ");
        }
        else if (strcmp(p.subsys, "Driving") == 0) {
>>>>>>> 10b03d4 ( Added changes related to client , yet to fix driving subsystems code)
            
            MsgSend(drive_coid, &p, sizeof(p), NULL, 0);
            printf("[CLIENT] : Received driving data from Dashboard ");
        }
        else if (strcmp(p.subsys, "Mode") == 0) {
            
<<<<<<< HEAD
            MsgSend(brake_coid, &p.msg, sizeof(p.msg), NULL, 0);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
            printf("[CLIENT] : Received data from dashboard for webots.c");
=======
            printf("[CLIENT] : Received data from dashboard for webots.c");
=======
            // TODO Mode process for this to be implemented
            printf("[CLIENT] : Received Mode data from Dashboard ");
>>>>>>> 10b03d4 ( Added changes related to client , yet to fix driving subsystems code)
        }


<<<<<<< HEAD
            MsgSend(brake_coid, &p.msg, sizeof(p.msg), NULL, 0);
            printf("[CLIENT] : Received data from webots api  ");
>>>>>>> e81f985 ( Work in progress just fixing some stuff , abit broken atm)
        }

        // Data coming  from  webots ( the input controls basically)
        else if (strcmp(p.msg_type, "vehicle_telem") == 0) {
             
            if (strcmp(p.msg_type, "Braking") == 0) {
                
                //From here then on send the data to appropriate processes , braking etc 
                
                MsgSend(brake_coid, &p.msg, sizeof(p.msg), NULL, 0);
                printf("[CLIENT] : Received braking data from Dashboard.");
            }

            MsgSend(brake_coid, &p.msg, sizeof(p.msg), NULL, 0);
            printf("[CLIENT] : Received data from webots api  ");
        }
=======
            printf("[CLIENT] : Received braking data from Dashboard.")
=======
            printf("[CLIENT] : Received braking data from Dashboard.");
>>>>>>> c72a477 (all systems being spawned, checking in, and being cleaned up correctly)
        }
>>>>>>> 9068e1e ( removed data not included in the telemetry the dashboard is waiting on)
        // else if (strcmp(p.msg_type, "ThrottleInput") == 0)
        // {
            
        //     MsgSend(throttle_coid, &p.msg, sizeof(p.msg), NULL, 0);
        //     printf("[CLIENT] : Received throttle data from Dashboard.")
        // }
        // else if (strcmp(p.msg_type, "SteeringInput") == 0)
        // {
            
        //     MsgSend(steering_coid, &p.msg, sizeof(p.msg), NULL, 0);
        //     printf("[CLIENT] : Received steering data from Dashboard.")
        // }
        // else if (strcmp(p.msg_type, "Heartbeat") == 0)
        // { 
        // //    TBD
        //     printf("[CLIENT] : Received Heartbeat data from Dashboard.")
        // }
=======
        // Testing comment to identify source of data 

        if (strcmp(p.origin, "VehicleData") == 0) {
            printf("[CLIENT] : Received Vehicle Data from Webots ");

        }
        else if (strcmp(p.origin, "UserInput") == 0) {
            printf("[CLIENT] : Received User input data from dashboard");
        } 
>>>>>>> 10b03d4 ( Added changes related to client , yet to fix driving subsystems code)

    }      
    
}


//TODO if client is a process it must start checking in with watchdog potentially to prevent it from dying as well

// ------------------------------------------------------------------------------------------------------------------------
void connect_to_processes(int *brake_coid , int *driving_coid){
    while (*brake_coid == -1) {
        *brake_coid = name_open("braking_system", 0);
        if (*brake_coid == -1) {
            printf("[CLIENT] Waiting for braking...\n");
            sleep(1);
        }
    }
    printf("[CLIENT] Connected to braking\n");

    while (*driving_coid == -1) {
        *driving_coid = name_open("driving_system", 0);
        if (*driving_coid == -1) {
            printf("[CLIENT] Waiting for driving system...\n");
            sleep(1);
        }
    }
    printf("[CLIENT] Connected to driving\n");



}


int main(int argc, char *argv[]) {

    attach = name_attach(NULL, "client", 0);
    if (!attach) {
        printf("[CLIENT] name_attach failed\n");
        return -1;
    }
    atexit(cleanup_client);
    signal(SIGTERM, shutdown_client);
    signal(SIGINT,  shutdown_client);
    printf("[CLIENT] Registered as client\n");

    
    // Connect to all the proccess required
    
    // int mode_coid = -1;
    int brake_coid = -1;
    int driving_coid = -1;

    connect_to_processes(&brake_coid , &driving_coid);


    // setup UDP socket for receiving from Python    
    receiver_setup();

    printf("[CLIENT] Listening on port %d\n", LISTEN_PORT);

    // start receiving
   
    //Receives all incoming traffic into QNX port , contains all process id's that would be needed in transfer of data
    main_qnx_receiver(sockfd, brake_coid , driving_coid );


    return 0;
}


/*
// MODELING FOR REC FOR THE JSON BIT:

receive(){
    ccheck source:
        if from dash
            check subsytem()
                msgsend  to proper subsytem dependent}

        if from vehicle 
            check subsystem()
                msgsend to proper subsytem dependent } 

*/


/*
-> Client has to connect with all the processes it will want to communicate with.
-> This means to pass the coid of each of thee processes into the main receive function or atleast an array of them
-> Best case is to do this once since we dont have too many processes and its a processes that only happens oce when starting up 
    the client process.
-> The data structure being sent has to align with what the function on the other end expects in terms of subsys , type and data

-> These processes include:
        - Braking
        - Driving
        - Steering

    */