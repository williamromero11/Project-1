/* COP 3502C Fall 25 Assignment 1
This program is written by: William Romero*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_STRING_LENGTH = 22;

// Defining structure for Registered Vehicle
typedef struct RegisteredVehicle
{                        // for one registered vehicle
    char *license_plate; // to be used for string license plate# of the vehicle
    char *owner_name;    // to be used for storing owner name
} RegisteredVehicle;

// Definig structure for Garage
typedef struct Garage
{                                        // for one garage
    char *garage_name;                   // to be used for garage name
    int total_capacity;                  // total capacity of the garage
    int current_count;                   // number of vehicles in the garage in a particular time
    RegisteredVehicle **parked_vehicles; // list of parked vehicles in a particular time
} Garage;

// Defining structure for Campus
typedef struct Campus
{
    Garage **garages;                        // list of garages in campus
    int total_garages;                       // number of garages in the campus
    RegisteredVehicle **registered_vehicles; // list of registered vehicles
    int total_registered_vehicles;           // total number of registered vehicles
} Campus;

// Functions Declarations
Garage *createGarage(const char *name, int capacity);
RegisteredVehicle *createRegisteredVehicle(const char *license, const char *owner);
void registerVehicle(Campus *campus, const char *license, const char *owner);
void parkVehicle(Garage *garage, RegisteredVehicle *vehicle);
int removeVehicleFromGarage(Garage *garage, const char *license);
RegisteredVehicle *searchVehicleByLicense(const Campus *campus, const char *license);
int countTotalVehicles(const Campus *campus);
int resizeGarage(Garage *garage, int new_capacity);
int relocateVehicle(Campus *campus, const char *license, const char *target_garage_name);
void displayVehiclesByOwner(const Campus *campus, const char *owner_name);
int removeGarage(Campus *campus, const char *garage_name);
void generateGarageUtilizationReport(const Campus *campus);
void freeCampus(Campus *campus);

// Functions Implementations
Garage *createGarage(const char *name, int capacity)
{
    // Creates a new garage with the specified name and capacity and initializes other variables and makes parked vehicles array based on the capacity.

    Garage *garage = (Garage *)malloc(sizeof(Garage));
    garage->garage_name = strdup(name);
    garage->total_capacity = capacity;
    garage->current_count = 0;
    garage->parked_vehicles = (RegisteredVehicle **)calloc(capacity, sizeof(RegisteredVehicle *));
    return garage;
}

RegisteredVehicle *createRegisteredVehicle(const char *license, const char *owner)
{
    // Create a new vehicle with the given license plate and owner.

    RegisteredVehicle *vehicle = (RegisteredVehicle *)malloc(sizeof(RegisteredVehicle));
    vehicle->license_plate = strdup(license);
    vehicle->owner_name = strdup(owner);
    return vehicle;
}

void registerVehicle(Campus *campus, const char *license, const char *owner)
{
    // Registers a new vehicle on campus.

    RegisteredVehicle *vehicle = createRegisteredVehicle(license, owner);
    campus->total_registered_vehicles++;
    campus->registered_vehicles = (RegisteredVehicle **)realloc(campus->registered_vehicles,
                                                                campus->total_registered_vehicles * sizeof(RegisteredVehicle *));
    campus->registered_vehicles[campus->total_registered_vehicles - 1] = vehicle;
    printf("REGISTERED\n");
}

void parkVehicle(Garage *garage, RegisteredVehicle *vehicle)
{
    // Parks a vehicle in a specified garage. If the garage is full, it prints “FULL”. Otherwise, if the parking is successful, it prints “PARKED”.

    if (garage->current_count >= garage->total_capacity)
    {
        printf("FULL\n");
        return;
    }

    garage->parked_vehicles[garage->current_count] = vehicle;
    garage->current_count++;
    printf("PARKED\n");
}

int removeVehicleFromGarage(Garage *garage, const char *license)
{
    // Removes a vehicle from a specified garage. The function returns 1 if the removal is successful. Otherwise, it returns 0

    for (int i = 0; i < garage->current_count; i++)
    {
        if (strcmp(garage->parked_vehicles[i]->license_plate, license) == 0)
        {
            for (int j = i; j < garage->current_count - 1; j++)
            {
                garage->parked_vehicles[j] = garage->parked_vehicles[j + 1];
            }
            garage->current_count--;
            return 1;
        }
    }
    return 0;
}

RegisteredVehicle *searchVehicleByLicense(const Campus *campus, const char *license)
{
    // Searches for a vehicle by its license plate across all garages on campus. If the vehicle is found, it return the vehicle. Otherwise, it should return NULL.

    for (int i = 0; i < campus->total_registered_vehicles; i++)
    {
        if (strcmp(campus->registered_vehicles[i]->license_plate, license) == 0)
        {
            return campus->registered_vehicles[i];
        }
    }
    return NULL;
}

int countTotalVehicles(const Campus *campus)
{
    // Returns the total number of vehicles parked across all garages on campus.

    int total = 0;
    for (int i = 0; i < campus->total_garages; i++)
    {
        total += campus->garages[i]->current_count;
    }
    return total;
}

int resizeGarage(Garage *garage, int new_capacity)
{
    // Resizes a garage to a new capacity. If the new capacity is smaller than the current number of vehicles in that garage, then you should print a massage "FAIL. TOO SMALL TO ACCOMMODATE EXISTING VEHICLES.\n". The function returns 1 or 0 depending on whether the task was successful or not.

    if (new_capacity < garage->current_count)
    {
        printf("FAIL. TOO SMALL TO ACCOMMODATE EXISTING VEHICLES.\n");
        return 0;
    }

    garage->parked_vehicles = (RegisteredVehicle **)realloc(garage->parked_vehicles,
                                                            new_capacity * sizeof(RegisteredVehicle *));
    garage->total_capacity = new_capacity;
    printf("SUCCESS\n");
    return 1;
}

int relocateVehicle(Campus *campus, const char *license, const char *target_garage_name)
{
    // Relocates a vehicle from one garage to another. It shows appropriate message based on the fRELOCATE command discussed above.
    RegisteredVehicle *vehicle = NULL;
    Garage *source_garage = NULL;
    Garage *target_garage = NULL;

    // Finding the target garage
    for (int i = 0; i < campus->total_garages; i++)
    {
        if (strcmp(campus->garages[i]->garage_name, target_garage_name) == 0)
        {
            target_garage = campus->garages[i];
            break;
        }
    }

    if (!target_garage)
    {
        printf("%s NOT FOUND.\n", target_garage_name);
        return 0;
    }

    // Finding the vehicle and its current garage
    for (int i = 0; i < campus->total_garages; i++)
    {
        for (int j = 0; j < campus->garages[i]->current_count; j++)
        {
            if (strcmp(campus->garages[i]->parked_vehicles[j]->license_plate, license) == 0)
            {
                vehicle = campus->garages[i]->parked_vehicles[j];
                source_garage = campus->garages[i];
                break;
            }
        }
        if (vehicle)
            break;
    }

    if (!vehicle)
    {
        printf("%s NOT IN CAMPUS.\n", license);
        return 0;
    }

    if (target_garage->current_count >= target_garage->total_capacity)
    {
        printf("%s IS FULL.\n", target_garage_name);
        return 0;
    }

    // Removing from source garage and add to target garage
    removeVehicleFromGarage(source_garage, license);
    parkVehicle(target_garage, vehicle);
    printf("RELOCATION SUCCESSFUL.\n");
    return 1;
}

void displayVehiclesByOwner(const Campus *campus, const char *owner_name)
{
    // Displays all vehicles owned by a specified person if they are in campus. It means, same owner can have multiple vehicles and all of them can be on campus as well.

    int found = 0;
    for (int i = 0; i < campus->total_registered_vehicles; i++)
    {
        if (strcmp(campus->registered_vehicles[i]->owner_name, owner_name) == 0)
        {
            found = 1;
            int parked = 0;
            for (int j = 0; j < campus->total_garages; j++)
            {
                for (int k = 0; k < campus->garages[j]->current_count; k++)
                {
                    if (campus->garages[j]->parked_vehicles[k] == campus->registered_vehicles[i])
                    {
                        printf("%s %s\n", campus->registered_vehicles[i]->license_plate, campus->garages[j]->garage_name);
                        parked = 1;
                        break;
                    }
                }
                if (parked)
                    break;
            }
            if (!parked)
            {
                printf("%s NOT ON CAMPUS\n", campus->registered_vehicles[i]->license_plate);
            }
        }
    }
    if (!found)
    {
        printf("NO REGISTERED CAR BY THIS OWNER\n");
    }
}

int removeGarage(Campus *campus, const char *garage_name)
{
    // Removes a specified garage from the campus and frees the memory. Note that while removing a garage, you should not remove the remove the vehicles from the system. The function returns 1 or 0 depending on whether the task was successful or not.

    int garage_index = -1;
    for (int i = 0; i < campus->total_garages; i++)
    {
        if (strcmp(campus->garages[i]->garage_name, garage_name) == 0)
        {
            garage_index = i;
            break;
        }
    }

    if (garage_index == -1)
    {
        printf("%s NOT FOUND\n", garage_name);
        return 0;
    }

    // Freeing the memory for the garage
    free(campus->garages[garage_index]->garage_name);
    free(campus->garages[garage_index]->parked_vehicles);
    free(campus->garages[garage_index]);

    // Shifting the remaining garages
    for (int i = garage_index; i < campus->total_garages - 1; i++)
    {
        campus->garages[i] = campus->garages[i + 1];
    }

    campus->total_garages--;
    campus->garages = (Garage **)realloc(campus->garages, campus->total_garages * sizeof(Garage *));

    printf("%s REMOVED\n", garage_name);
    return 1;
}

void generateGarageUtilizationReport(const Campus *campus)
{
    // Generates a report on the utilization of all garages.
    double min_utilization = 100.0;
    const char *least_utilized = NULL;

    for (int i = 0; i < campus->total_garages; i++)
    {
        Garage *garage = campus->garages[i];
        double utilization = (double)garage->current_count / garage->total_capacity * 100;

        printf("Garage: %s, Capacity: %d, Occupied: %d, Utilization: %.2f%%\n",
               garage->garage_name, garage->total_capacity, garage->current_count, utilization);

        if (utilization < min_utilization)
        {
            min_utilization = utilization;
            least_utilized = garage->garage_name;
        }
    }

    if (least_utilized)
    {
        printf("Least Utilized: %s\n", least_utilized);
    }
}

void freeCampus(Campus *campus)
{
    // Free all memory occupied during program run.
    for (int i = 0; i < campus->total_garages; i++)
    {
        free(campus->garages[i]->garage_name);
        free(campus->garages[i]->parked_vehicles);
        free(campus->garages[i]);
    }
    free(campus->garages);

    for (int i = 0; i < campus->total_registered_vehicles; i++)
    {
        free(campus->registered_vehicles[i]->license_plate);
        free(campus->registered_vehicles[i]->owner_name);
        free(campus->registered_vehicles[i]);
    }
    free(campus->registered_vehicles);

    free(campus);
}

// Main Function

int main()
{

    // Reading and storing number of Garage, Number of registered Vehicles, Number of Commands to be processed
    int g, v, c;
    scanf("%d %d %d", &g, &v, &c);

    Campus *campus = (Campus *)malloc(sizeof(Campus));
    campus->total_garages = g;
    campus->total_registered_vehicles = v;
    campus->garages = (Garage **)malloc(g * sizeof(Garage *));
    campus->registered_vehicles = (RegisteredVehicle **)malloc(v * sizeof(RegisteredVehicle *));

    // Reading garage information
    for (int i = 0; i < g; i++)
    {
        char name[MAX_STRING_LENGTH];
        int capacity;
        scanf("%s %d", name, &capacity);
        campus->garages[i] = createGarage(name, capacity);
    }

    // Reading registered vehicle information
    for (int i = 0; i < v; i++)
    {
        char license[MAX_STRING_LENGTH], owner[MAX_STRING_LENGTH];
        scanf("%s %s", license, owner);
        campus->registered_vehicles[i] = createRegisteredVehicle(license, owner);
    }

    // Processing commands
    for (int i = 0; i < c; i++)
    {
        // Reading each command one by one
        char command[MAX_STRING_LENGTH];
        scanf("%s", command);

        // Processing Command PARK
        if (strcmp(command, "PARK") == 0)
        {
            char license[MAX_STRING_LENGTH], garage_name[MAX_STRING_LENGTH];
            scanf("%s %s", license, garage_name);
            RegisteredVehicle *vehicle = searchVehicleByLicense(campus, license);
            Garage *garage = NULL;
            for (int j = 0; j < campus->total_garages; j++)
            {
                if (strcmp(campus->garages[j]->garage_name, garage_name) == 0)
                {
                    garage = campus->garages[j];
                    break;
                }
            }
            if (vehicle && garage)
            {
                parkVehicle(garage, vehicle);
            }
        }
        else if (strcmp(command, "UTILIZATION_REPORT") == 0)
        {
            // Processing Command UTILIZATION_REPORT
            generateGarageUtilizationReport(campus);
        }
        else if (strcmp(command, "RESIZE") == 0)
        {
            // Processing Command RESIZE
            char garage_name[MAX_STRING_LENGTH];
            int new_capacity;
            scanf("%s %d", garage_name, &new_capacity);
            Garage *garage = NULL;

            for (int j = 0; j < campus->total_garages; j++)
            {
                if (strcmp(campus->garages[j]->garage_name, garage_name) == 0)
                {
                    garage = campus->garages[j];
                    break;
                }
            }

            if (garage)
            {
                resizeGarage(garage, new_capacity);
            }
        }
        else if (strcmp(command, "SEARCH_OWNER") == 0)
        {
            // Processing Command SEARCH_OWNER
            char owner[MAX_STRING_LENGTH];
            scanf("%s", owner);
            displayVehiclesByOwner(campus, owner);
        }
        else if (strcmp(command, "RELOCATE") == 0)
        {
            // Processing Command RELOCATE
            char license[MAX_STRING_LENGTH], target_garage[MAX_STRING_LENGTH];
            scanf("%s %s", license, target_garage);
            relocateVehicle(campus, license, target_garage);
        }
        else if (strcmp(command, "COUNT_TOTAL") == 0)
        {
            // Processing Command COUNT_TOTAL
            printf("%d\n", countTotalVehicles(campus));
        }
        else if (strcmp(command, "REGISTER_VEHICLE") == 0)
        {
            // Processing Command REGISTER_VEHICLE
            char license[MAX_STRING_LENGTH], owner[MAX_STRING_LENGTH];
            scanf("%s %s", license, owner);
            registerVehicle(campus, license, owner);
        }
        else if (strcmp(command, "REMOVE_VEHICLE_GARAGE") == 0)
        {
            // Processing Command REMOVE_VEHICLE_GARAGE
            char license[MAX_STRING_LENGTH];
            scanf("%s", license);
            int removed = 0;
            for (int j = 0; j < campus->total_garages; j++)
            {
                if (removeVehicleFromGarage(campus->garages[j], license))
                {
                    printf("REMOVED FROM %s\n", campus->garages[j]->garage_name);
                    removed = 1;
                    break;
                }
            }
            if (!removed)
            {
                printf("NOT FOUND IN CAMPUS\n");
            }
        }
        else if (strcmp(command, "REMOVE_GARAGE") == 0)
        {
            // Processing Command REMOVE_GARAGE
            char garage_name[MAX_STRING_LENGTH];
            scanf("%s", garage_name);
            removeGarage(campus, garage_name);
        }
    }

    // Cleaning up and freeing all allocated memory
    freeCampus(campus);

    return 0;
}