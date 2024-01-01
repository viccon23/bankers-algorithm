// Header files
#include <stdio.h>   // For standard input/output functions
#include <stdlib.h>  // For dynamic memory allocation functions (malloc, free, etc.)
#include <stdbool.h> // For booleans
#include <string.h>  // For strcmp

struct State {
    int *resource;
    int *available;
    int **claim;
    int **alloc;
};

    // FOR scanning line, size of input
    char buffer[256];
    struct State global_state;

    int numResources; // Number of resources
    int numProcesses; // Number of processes

bool isSafe(struct State state, int numProcesses, int numResources, int safeSeq[]) {
    int currentavail[numResources];
    int *rest = malloc(numProcesses * sizeof(int));
    
    // Initialize currentavail with available resources
    for (int j = 0; j < numResources; ++j) {
        currentavail[j] = state.available[j];
    }

    // Initialize rest array with all processes
    for (int i = 0; i < numProcesses; ++i) {
        rest[i] = 1;  // 1 indicates the process is still in the system
    }

    bool possible = true;
    int count = 0; // Counter for safe sequence
    while (possible) {
        bool found = false;

        // Find a process Pk in rest such that claim[k,*] - alloc[k,*] <= currentavail
    for (int k = 0; k < numProcesses; ++k) {
        if (rest[k]) {
            // Assume the number of resources is dynamic and stored in numResources
            bool canExecute = true;

            // Check the condition for each resource type
            for (int j = 0; j < numResources; ++j) {
                if (state.claim[k][j] - state.alloc[k][j] > currentavail[j]) {
                    // Process cannot execute if the condition is not satisfied for any resource type
                    canExecute = false;
                    break;
                }
            }

            if (canExecute) {
                // Found a process that can be executed
                found = true;
                
                // Simulate execution of Pk
                for (int j = 0; j < numResources; ++j) {
                    currentavail[j] += state.alloc[k][j];
                }
                
                // Mark the process as completed
                rest[k] = 0;
                safeSeq[count++] = k;
                
                break;  // Exit the loop once a process is found
            }
        }
    }
        if (!found) {
            // No process was found, set possible to false
            possible = false;
        }
    }

    // Check if all processes have been completed
    for (int i = 0; i < numProcesses; ++i) {
        if (rest[i] == 1) {
            free(rest);
            return false;
        }
    }

    // All processes have been completed, return true
    free(rest);
    return true;
}

// Function to validate if the entered number is positive
// Function to validate if the entered number is positive
int validatePositiveNumber() {
    int num;
    char input[20];
    while (1) {
        if (fgets(input, sizeof(input), stdin)) {
            // Try to convert the input to an integer
            if (sscanf(input, "%d", &num) == 1 && num > 0) {
                break; // Break the loop if a positive number is entered
            } else {
                printf("Invalid input. Please enter a positive number.\n");
            }
        }
    }
    return num;
}
int main() {
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // RESOURCE VECTOR
    printf("Enter the number of resources: ");
    numResources = validatePositiveNumber();

    int *R = (int *)malloc(numResources * sizeof(int)); // Resource Vector

    // Check for allocation success
    if (R == NULL) {
        printf("Memory allocation failed for Resource Vector.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the Resource Vector
    for (int j = 0; j < numResources; ++j) {
        // Initialize each entry R[j] with the available units of resource Rⱼ
        printf("Enter the number of units for resource R%d: ", j);
        R[j] = validatePositiveNumber();
    }

    // PROCESS VECTOR
    printf("Enter the number of processes: ");
    numProcesses = validatePositiveNumber();

    int **P = (int **)malloc(numProcesses * sizeof(int *)); // Maximum Claim Matrix

    // Check for allocation success
    if (P == NULL) {
        printf("Memory allocation failed for Maximum Claim Matrix.\n");
        exit(EXIT_FAILURE);
    }

 // Initialize global data structures dynamically
    global_state.resource = (int *)malloc(numResources * sizeof(int));
    global_state.available = (int *)malloc(numResources * sizeof(int));
    global_state.claim = (int **)malloc(numProcesses * sizeof(int *));
    global_state.alloc = (int **)malloc(numProcesses * sizeof(int *));

    for (int i = 0; i < numProcesses; i++) {
        global_state.claim[i] = (int *)malloc(numResources * sizeof(int));
        global_state.alloc[i] = (int *)malloc(numResources * sizeof(int));
    }


    // Initialize available resources with the initial values from R
    for (int j = 0; j < numResources; ++j) {
        global_state.available[j] = R[j];
    }

    // Claim Matrix Input
    for (int i = 0; i < numProcesses; ++i) {
        printf("Enter the maximum claim values for process %d:\n", i);
        for (int j = 0; j < numResources; ++j) {
            printf("Resource R%d: ", j);
            global_state.claim[i][j] = validatePositiveNumber();
        }
    }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERACTIVE SECTION
    printf("Enter commands (e.g., request(0, 0, 1) or release(1, 1, 2)):\n");
     while (1) {
        
        // Replace the newline character with null character
         buffer[strcspn(buffer, "\n")] = '\0';

        fgets(buffer, sizeof(buffer), stdin);

        if (strcasecmp(buffer, "exit\n") == 0) {
            break; // Exit the program
        }

        int Pnum, Rnum, unitNum;
        char command[8];

        sscanf(buffer, "%7s (%d, %d, %d)", command, &Pnum, &Rnum, &unitNum);

        if (strcmp(command, "request") == 0) {
            // Verify values to be non-negative
            if (unitNum < 0 || Pnum < 0 || Rnum < 0) {
                // Invalid input, display error message and prompt again
                printf("Invalid input. Please enter non-negative values for release.\n");
                continue;
            }
            // Determine if the request is less than the process maximum claim for that resource.
                if (unitNum + global_state.alloc[Pnum][Rnum] > global_state.claim[Pnum][Rnum]) {
                    // Check if the request can be satisfied with available resources
                    printf("Error: Process %d requesting more than maximum claim for resource %d.\n", Pnum, Rnum);
                } else if (unitNum > global_state.available[Rnum]) {// Check if the request asks for more units than there are available for resource Rnum
                    printf("Not granted, not enough resources for process %d (resource %d).\n", Pnum, Rnum);
                } else {
                    // Update allocation matrix and available resources
                    global_state.alloc[Pnum][Rnum] += unitNum;
                    global_state.available[Rnum] -= unitNum;
                    // Define new state
                    struct State newstate = global_state;  // Create a copy of the current state
                    // Check if the new state is safe
                    int safeSeq[numProcesses];
                    if (isSafe(global_state, numProcesses, numResources, safeSeq)) {
                        // Grant the request and output the safe sequence
                        printf("Request granted. Safe sequence: ");
                        for (int i = 0; i < numProcesses; ++i) {
                            printf("%d ", safeSeq[i]);
                        }
                        printf("\n");

                                    // Display all info just to get an idea of where user stands on resources.
                                printf("                  Current state:\n");
                        // Print resource vector
                        printf("Resource Vector: ");
                        for (int j = 0; j < numResources; ++j) {
                            printf("%d ", R[j]);
                        }
                        printf("\n");

                        // Print available resources
                        printf("Available Resources: ");
                        for (int j = 0; j < numResources; ++j) {
                            printf("%d ", global_state.available[j]);
                        }
                        printf("\n");

                        // Print claim matrix
                        printf("Claim Matrix:\n");
                        for (int i = 0; i < numProcesses; ++i) {
                            for (int j = 0; j < numResources; ++j) {
                                printf("%d ", global_state.claim[i][j]);
                            }
                            printf("\n");
                        }

                        // Print allocation matrix
                        printf("Allocation Matrix:\n");
                        for (int i = 0; i < numProcesses; ++i) {
                            for (int j = 0; j < numResources; ++j) {
                                printf("%d ", global_state.alloc[i][j]);
                            }
                            printf("\n");
                        }

                    } else {
                        // Restore the original state
                        global_state.alloc[Pnum][Rnum] -= unitNum;
                        global_state.available[Rnum] += unitNum;
                        // Deny the request if there is no safe sequence
                        printf("Request denied. No safe sequence, Deadlock warning.\n");
                    }
                }
            printf("\n");
            printf("Enter commands (e.g., request(0, 0, 1) or release(1, 1, 2)):\n");
        } else if (strcmp(command, "release") == 0) {
            // Verify values to be non-negative
            if (unitNum < 0 || Pnum < 0 || Rnum < 0) {
                // Invalid input, display error message and prompt again
                printf("Invalid input. Please enter non-negative values for release.\n");
                continue;
            }
            // Release logic
            if (unitNum > global_state.alloc[Pnum][Rnum]) {
                // Check if the release amount is greater than the allocated amount
                printf("Error: Process %d releasing more units than allocated for resource %d.\n", Pnum, Rnum);
                printf("Enter commands (e.g., request(0, 0, 1) or release(1, 1, 2)):\n");
            } else {
                // Release resources
                global_state.alloc[Pnum][Rnum] -= unitNum;
                global_state.available[Rnum] += unitNum;
                printf("Resources released successfully.\n");
                    // Display all info just to get an idea of where user stands on resources.
                                printf("                  Current state:\n");
                        // Print resource vector
                        printf("Resource Vector: ");
                        for (int j = 0; j < numResources; ++j) {
                            printf("%d ", R[j]);
                        }
                        printf("\n");

                        // Print available resources
                        printf("Available Resources: ");
                        for (int j = 0; j < numResources; ++j) {
                            printf("%d ", global_state.available[j]);
                        }
                        printf("\n");

                        // Print claim matrix
                        printf("Claim Matrix:\n");
                        for (int i = 0; i < numProcesses; ++i) {
                            for (int j = 0; j < numResources; ++j) {
                                printf("%d ", global_state.claim[i][j]);
                            }
                            printf("\n");
                        }

                        // Print allocation matrix
                        printf("Allocation Matrix:\n");
                        for (int i = 0; i < numProcesses; ++i) {
                            for (int j = 0; j < numResources; ++j) {
                                printf("%d ", global_state.alloc[i][j]);
                            }
                            printf("\n");
                        }
                        printf("\n");
                        printf("Enter commands (e.g., request(0, 0, 1) or release(1, 1, 2)):\n");
            }
        } else {
            printf("Invalid command. Use the format: request(i, j, k) or release(i, j, k).\n");
        }
    }
////////////////////////////////////////////////////////////////////////////////////////////////////////
// FREE MEMORY
   free(R);

    for (int i = 0; i < numProcesses; ++i) {
        free(P[i]);
    }
    free(P);

    free(global_state.resource);
    free(global_state.available);
    for (int i = 0; i < numProcesses; ++i) {
        free(global_state.claim[i]);
        free(global_state.alloc[i]);
    }
    free(global_state.claim);
    free(global_state.alloc);

////////////////////////////////////////////////////////////////////////////////////////////////////////

    return 0;
}
