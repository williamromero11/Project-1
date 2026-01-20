## Project One: Campus Parking System

## Overview
A C program that simulates a university campus parking management system using dynamic memory allocation, pointers, and arrays of pointers. The system tracks multiple parking garages, registered vehicles, and processes various commands for parking management.

## Features
- Dynamic memory allocation for garages and vehicles
- Command processing system
- Real-time garage utilization tracking
- Vehicle registration and search functionality
- Memory leak-free implementation

## Commands Supported
- `PARK <LP> <G>` - Park vehicle in specified garage
- `UTILIZATION_REPORT` - Display garage utilization statistics
- `RESIZE <G> <nc>` - Resize garage capacity
- `SEARCH_OWNER <OWNER>` - Find vehicles by owner
- `RELOCATE <LP> <G>` - Move vehicle to different garage
- `COUNT_TOTAL` - Show total parked vehicles
- `REGISTER_VEHICLE <LP> <OWNER>` - Register new vehicle
- `REMOVE_VEHICLE_GARAGE <LP>` - Remove vehicle from garage
- `REMOVE_GARAGE <G>` - Remove garage from system

## Data Structures
```c
typedef struct RegisteredVehicle {
    char* license_plate;
    char* owner_name;
} RegisteredVehicle;

typedef struct Garage {
    char* garage_name;
    int total_capacity;
    int current_count;
    RegisteredVehicle** parked_vehicles;
} Garage;

typedef struct Campus {
    Garage** garages;
    int total_garages;
    RegisteredVehicle** registered_vehicles;
    int total_registered_vehicles;
} Campus;
```
## Key Functions Implemented
- Garage* createGarage(const char* name, int capacity)
- RegisteredVehicle* createRegisteredVehicle(const char* license, const char* owner)
- void parkVehicle(Garage* garage, RegisteredVehicle* vehicle)
- int removeVehicleFromGarage(Garage* garage, const char* license)
- void generateGarageUtilizationReport(const Campus* campus)

## How to Use
1. Compile the program: `gcc main.c -o parking_system`
2. Create an input file with test data
3. Run: `./parking_system < input.txt`
4. View results in terminal

## Input Format
First line: g v c (number of garages, vehicles, commands)
Next g lines: garage names and capacities
Next v lines: license plates and owners
Next c lines: commands to process

## Example
```
Input:
3 3 2
GARAGE_A 50
GARAGE_B 30
GARAGE_C 20
ABC123 John
XYZ789 Jane
DEF456 Bob
PARK ABC123 GARAGE_A
UTILIZATION_REPORT

Output:
PARKED
Garage: GARAGE_A, Capacity: 50, Occupied: 1, Utilization: 2.00%
Garage: GARAGE_B, Capacity: 30, Occupied: 0, Utilization: 0.00%
Garage: GARAGE_C, Capacity: 20, Occupied: 0, Utilization: 0.00%
Least Utilized: GARAGE_B
```
## Memory Management
All strings dynamically allocated with exact size needed
All memory properly freed using free() and freeAllMemory() function
No memory leaks (tested with Valgrind)
