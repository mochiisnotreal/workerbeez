/*
v2.0
--------------------------------------------

Purpose: The inventory management program to be implemented by “Worker Beez” will:
    Allow the Distributors to input the following information: client’s name, client’s address, product’s name, product’s detail, product's price, currency used, product’s weight,
product’s category, Distributor’s name, whether the company is in-house or out house.
    The system will store this information then organize them all together based on how they were enter and
based on whether it is an out-house or an in-house company so the appropriate area at the ‘Worker Beez’ branch 
can be searched to get the correct goods.
    Edits can also be made to the txt. file outside of the program if specific areas needs to edited.
the program also allows the user to search up data correlating with there ID. Ids are used in search-ups.

--------------------------------------------
*/

#include <stdbool.h> //Library for using the boolean function
#include <stdio.h> //Library for input and output operations.
#include <stdlib.h> //Library for utility functions like system("cls").
#include <string.h> //Library for string manipulations like the use of strcmp in int main().
#include <ctype.h> //Library for characters testing and manipulation like the use of toupper().
#include <time.h> //Library that handles time and date related operations.

//Both initializations are use to control conditions in processes
int MAX = 0;
int JK = 0;

struct Lister //This struct Lister is a composite data type that allows you to group together variables of different data types under a single name.
{
    char buyer_name[30]; //string storage for Buyer's name.
    char product_name[40]; //string storage for product's name.
    char product_details[10000]; //string storage for product's details.
    char distributor_name[40]; //string storage for Distributor's Name.
    char location[11]; //string storage for to store 'in-house' or 'out-house' to describe the describe the distributors.
    float price; //a float storage to store the product's price.
    float product_weight; //A float storage to store the product's weight.
    char address[60]; //string storage for the buyer's address.
    char currency[4]; //string storage for the price currency.
    int product_category; //integer storage for selection of the product category.
    int ID; //integer storage for the ID number of each entry.
    time_t time; //a storage that stores the current time when entry has been entered.
};

//The creation of a list called Node that contains data and ability to link to other nodes
typedef struct Node{ 
    struct Lister Saves; //initializing a list (Lister) called Saves
    struct Node* next; //creates another Node called next
}Node;

//The creation of a structure called stk to be used as the stack
typedef struct{
    Node*top; //this creates another Node called top
}stk;

struct Lister catalog; //Initializing a global list Lister called catalog

int numListings = 0; //initialize an integer variable, numListings, to equal 0.
int IDcounter = 1; //initialize an integer variable, IDcounter, to equal 0.
char ch; //initialize an character variable, ch.

const char* LastIDFilename = "LastID.txt"; //this declares a constant pointer to a character array (const char*) named LastIDFilename. It is assigned the string literal "LastID.txt". Represents the filename where the last used ID is stored or retrieved.
const char* filename = "All_Records.txt"; //this declares a constant pointer to a character array (const char*) named filename. It is assigned the string literal "catalog.txt". Represents the filename where catalog data is stored or retrieved.

//This function initializes a new Stack
stk* initialize(){
    stk* Stack = (stk*)malloc(sizeof(stk)); //This allocates dynamic memory to the Stack so it's size isn't fixed
    Stack->top = NULL; //This makes the Stack-> equal to empty
    return Stack; //returns the the created stack/
}

//a function used to open "LastID.txt" to store data from IDcounter into file
void ID()
{
    FILE* file = fopen(LastIDFilename, "w"); //opens "LastID.txt" and is open using write method. It is for writing a single line that can be overwritten.
    if (file == NULL) {
        perror("Failed to open file for writing");
        return;
    }
    fprintf(file, "%d", IDcounter); //this save the data from IDcounter to the file.
    fclose(file); //closes the file.
}

//Created function to upload data entered to a created database, with parameters
void Data_Uploads(stk*stack){
    FILE* file1 = fopen("stack.dat", "wb"); //Opens a dat file called "stack.dat" as writing binary
    if (file1 == NULL) {
        //prints error if the condition of the file1 being empty is true
        perror("Failed to open file for writing");
        return;
    }

    Node*temp = stack->top; //initialize a Node called temp to equal to stack-top


    while (temp!=NULL){
        //do the following while Node called temp in not empty
        fwrite(&temp->Saves, sizeof(temp->Saves), 1, file1); //writes the data from temp->Saves to the .dat file
        temp = temp->next; //the temp is equal to the next linked Node
    }
    fclose(file1); //closing of the file
}

//A function that loads in the saved data from the databases to be used with its parametres
void Data_Booted(stk*stack){
    FILE* file = fopen(LastIDFilename, "r"); //Opens file that LastIDFilename points to as reading mode
    if (file == NULL) { //if it is empty do
        //print text then returns
        perror("Failed to open file for reading");
        return;
    }

    fscanf(file, "%d", &IDcounter); //saves data from file to IDcounter
    fclose(file); //closes file

    FILE* file1 = fopen("stack.dat", "rb"); //Opens file1 called "stack.dat" as read binary
    if (file1 == NULL) { //if it is empty do
        //prints the text then returns
        perror("Failed to open file for reading");
        return;
    }
    
    Node*temp = NULL; //initialize a temp Node called temp
    
    while(1){ //infinite while loop
        Node*new=(Node*)malloc(sizeof(Node)); //initialize a Node called new with dynamic memory
        if (fread(&new->Saves, sizeof(new->Saves), 1, file1) != 1){ //if the reading of data from file to new->Saves is not true, or present do
            free(new);
            break; //exit the inifinite loop
        }
        new->next = NULL; //the next node that new points to is made empty

        if (temp == NULL){ //if temp is empty do
            stack->top = new; //since that means the stack is new, the to top of the stack points to new 
        }else{
            temp->next = new; //temp links the current node to a newly allocated node called new
        }

        temp = new; //temps current pointer points to new
    }
    fclose(file1); //closes file1
}

/*This line declares a function named Readabletime that takes a time_t variable named time as
an argument and returns a pointer to a character (char*).*/
char* Readabletime(time_t time)
{
    static char buffs[20]; //This line declares a static character array (char buffs[20]) with a size of 20. The static keyword here means that the variable will retain its value between function calls.
    struct tm* timestamp = localtime(&time); //This line declares a pointer to a structure of type tm named timestamp and assigns it the result of the localtime function. The localtime function takes a time_t variable as an argument and converts it to a structure representing the local time.
    strftime(buffs, sizeof(buffs), "%Y-%m-%d %H:%M:%S", timestamp); //This line uses the strftime function to format the time information from the timestamp structure and store it in the buffs array. The format string "%Y-%m-%d %H:%M:%S" specifies the desired format for the output.
    return buffs; //This line returns a pointer to the buffs array.
}

//this function removes the data at the of the stack with needed parameters
void Pop(stk *stack, struct Lister *Popo)
{
    Node *POPPED; //creates a node called POPPED
    if (stack->top == NULL){ //checks if the stack->top from parameter is empty to do
        printf("Stack is empty\n"); //display text
        return; //return exits the stack
    }
    POPPED = stack->top; //POPPED points stack->top, it is equal to the top of the stack
    *Popo = POPPED->Saves; //Popo is equal to the Lister Saves data from POPPED
    stack->top = stack->top->next; //stack->top points to the data that is immediately before the present node 
    free(POPPED); //this erases the data of POPPED since its temp otherwise it takes up data
}

//Function to save data to the txt file
void SaveToMainFile(struct Lister *catalog) 
{   //open a file using its pointer in appending mode
    FILE* file = fopen(filename, "a");
    if (file == NULL) { //checks if file is empty
        printf("Error opening the file for writing. There is no saved information\n");
        return; // Exit the void to go back to the menu
    }

    for (int k = 0; k < 210; ++k) //for loop used to produce horizontal line
    {
        fprintf(file,"_"); //displays the symbol on screen
    }
    fprintf(file, "\n\n");

    //fprintf is used to save data to file.
    fprintf(file, "ID Number is => %d\n", catalog->ID); //saves the displayed text and catalog ID information to file then bring the cursor below that line.
    fprintf(file, "Timestamp => %s\n", Readabletime(catalog->time)); //saves the displayed text and catalog time from the Readabletime pointer information to file then bring the cursor below that line.
    fprintf(file, "The buyer's name => %s\n", catalog->buyer_name); //saves the displayed text and catalog buyer_name information to file then bring the cursor below that line.
    fprintf(file, "The buyer's address => %s\n", catalog->address); //saves the displayed text and catalog address information to file then bring the cursor below that line.
    fprintf(file, "The distributor's name => %s\n", catalog->distributor_name);//saves the displayed text and catalog distributor_name information to file then bring the cursor below that line.
    fprintf(file, "The company is => %s\n", catalog->location);//saves the displayed text and catalog location information to file then bring the cursor below that line.
    fprintf(file, "The product's name is => %s\n", catalog->product_name); //saves the displayed text and catalog product_name information to file then bring the cursor below that line.
    fprintf(file, "The product's price => $%.2f %s\n", catalog->price, catalog->currency); //saves the displayed text and catalog currency information to file then bring the cursor below that line.

    switch (catalog->product_category) //a switch case to check product category selection with a number.
    {
    case 1: //if the entered data is equal to 1.
        fprintf(file, "The product's category is 'Electronics'\n"); //saves the text to the file and skips the line.
        break; //ends case 1.

    case 2: //if the entered data is equal to 2.
        fprintf(file, "The product's category is 'Toys and Games'\n"); //saves the text to the file and skips the line.
        break; //ends case 2.

    case 3: //if the entered data is equal to 3.
        fprintf(file, "The product's category is 'Health & Wellness Products'\n"); //saves the text to the file and skips the line.
        break; //ends case 3.

    case 4: //if the entered data is equal to 4.
        fprintf(file, "The product's category is 'Sports & Outdoor Items'\n"); //saves the text to the file and skips the line.
        break; //ends case 4.

    case 5: //if the entered data is equal to 5.
        fprintf(file, "The product's category is 'Library of Books'\n"); //saves the text to the file and skips the line.
        break; //ends case 5.

    case 6: //if the entered data is equal to 6.
        fprintf(file, "The product's category is 'Whole Foods'\n"); //saves the text to the file and skips the line.
        break; //ends case 6.
    
    case 7: //if the entered data is equal to 7.
        fprintf(file, "The product's category is 'Home Accessories'\n"); //saves the text to the file and skips the line.
        break; //ends case 7.
    }

    fprintf(file, "The product's weight is => %.2f lbs\n\n", catalog->product_weight); //saves the displayed text and catalog product_weight information to file then bring the cursor below that line.
    fprintf(file, "The product's details is:\n"); //saves the displayed text to file
    fprintf(file, "%s\n", catalog->product_details); //saves the catalog product_details information to file
    ///fprintf(file, "\n"); //save 2 skip spaces in file

    fclose(file); //closes the file.


    FILE* file1 = fopen("Inhouse.txt","a");
    if (strcmp(catalog->location, "IN-HOUSE") == 0) //checks if catalog[i].location data is equal to the string "IN-HOUSE"
    {
        for (int k = 0; k < 210; ++k) //for loop used to produce horizontal line
        {
            fprintf(file,"_"); //displays the symbol on screen
        }

        fprintf(file1, "\n\n"); //save 2 skipped lines to the file
        fprintf(file1, "ID Number is => %d\n", catalog->ID); //saves the displayed text and catalog ID information to file then bring the cursor below that line.
        fprintf(file1, "Timestamp => %s\n", Readabletime(catalog->time)); //saves the displayed text and catalog time from the Readabletime pointer information to file then bring the cursor below that line.
        fprintf(file1, "The buyer's name => %s\n", catalog->buyer_name); //saves the displayed text and catalog buyer_name information to file then bring the cursor below that line.
        fprintf(file1, "The buyer's address => %s\n", catalog->address); //saves the displayed text and catalog address information to file then bring the cursor below that line.
        fprintf(file1, "The distributor's name => %s\n", catalog->distributor_name);//saves the displayed text and catalog distributor_name information to file then bring the cursor below that line.
        fprintf(file1, "The company is => %s\n", catalog->location);//saves the displayed text and catalog location information to file then bring the cursor below that line.
        fprintf(file1, "The product's name is => %s\n", catalog->product_name); //saves the displayed text and catalog product_name information to file then bring the cursor below that line.
        fprintf(file1, "The product's price => $%.2f %s\n", catalog->price, catalog->currency); //saves the displayed text and catalog currency information to file then bring the cursor below that line.

        switch (catalog->product_category) //a switch case to check product category selection with a number.
        {
        case 1: //if the entered data is equal to 1.
            fprintf(file1, "The product's category is 'Electronics'\n"); //saves the text to the file and skips the line.
            break; //ends case 1.

        case 2: //if the entered data is equal to 2.
            fprintf(file1, "The product's category is 'Toys and Games'\n"); //saves the text to the file and skips the line.
            break; //ends case 2.

        case 3: //if the entered data is equal to 3.
            fprintf(file1, "The product's category is 'Health & Wellness Products'\n"); //saves the text to the file and skips the line.
            break; //ends case 3.

        case 4: //if the entered data is equal to 4.
            fprintf(file1, "The product's category is 'Sports & Outdoor Items'\n"); //saves the text to the file and skips the line.
            break; //ends case 4.

        case 5: //if the entered data is equal to 5.
            fprintf(file1, "The product's category is 'Library of Books'\n"); //saves the text to the file and skips the line.
            break; //ends case 5.

        case 6: //if the entered data is equal to 6.
            fprintf(file1, "The product's category is 'Whole Foods'\n"); //saves the text to the file and skips the line.
            break; //ends case 6.
        
        case 7: //if the entered data is equal to 6.
            fprintf(file1, "The product's category is 'Home Accessories'\n"); //saves the text to the file and skips the line.
            break; //ends case 7.
        }

        fprintf(file1, "The product's weight is => %.2f lbs\n\n", catalog->product_weight); //saves the displayed text and catalog product_weight information to file then bring the cursor below that line.
        fprintf(file1, "The product's details is:\n"); //saves the displayed text to file
        fprintf(file1, "%s\n", catalog->product_details); //saves the catalog product_details information to file
        
        MAX = 1;  
    }
    fclose(file1); //closes the file.

    FILE* file3 = fopen("Outhouse.txt","a");
    if (strcmp(catalog->location, "OUT-HOUSE") == 0) //checks if catalog[i].location data is equal to the string "OUT-HOUSE"
    {
        for (int k = 0; k < 210; ++k) //for loop used to produce horizontal line
        {
            fprintf(file,"_"); //displays the symbol on screen
        }

        fprintf(file3, "\n\n"); //save 2 skipped lines to the file
        fprintf(file3, "ID Number is => %d\n", catalog->ID); //saves the displayed text and catalog ID information to file then bring the cursor below that line.
        fprintf(file3, "Timestamp => %s\n", Readabletime(catalog->time)); //saves the displayed text and catalog time from the Readabletime pointer information to file then bring the cursor below that line.
        fprintf(file3, "The buyer's name => %s\n", catalog->buyer_name); //saves the displayed text and catalog buyer_name information to file then bring the cursor below that line.
        fprintf(file3, "The buyer's address => %s\n", catalog->address); //saves the displayed text and catalog address information to file then bring the cursor below that line.
        fprintf(file3, "The distributor's name => %s\n", catalog->distributor_name);//saves the displayed text and catalog distributor_name information to file then bring the cursor below that line.
        fprintf(file3, "The company is => %s\n", catalog->location);//saves the displayed text and catalog location information to file then bring the cursor below that line.
        fprintf(file3, "The product's name is => %s\n", catalog->product_name); //saves the displayed text and catalog product_name information to file then bring the cursor below that line.
        fprintf(file3, "The product's price => $%.2f %s\n", catalog->price, catalog->currency); //saves the displayed text and catalog currency information to file then bring the cursor below that line.

        switch (catalog->product_category) //a switch case to check product category selection with a number.
        {
        case 1: //if the entered data is equal to 1.
            fprintf(file3, "The product's category is 'Electronics'\n"); //saves the text to the file and skips the line.
            break; //ends case 1.

        case 2: //if the entered data is equal to 2.
            fprintf(file3, "The product's category is 'Toys and Games'\n"); //saves the text to the file and skips the line.
            break; //ends case 2.

        case 3: //if the entered data is equal to 3.
            fprintf(file3, "The product's category is 'Health & Wellness Products'\n"); //saves the text to the file and skips the line.
            break; //ends case 3.

        case 4: //if the entered data is equal to 4.
            fprintf(file3, "The product's category is 'Sports & Outdoor Items'\n"); //saves the text to the file and skips the line.
            break; //ends case 4.

        case 5: //if the entered data is equal to 5.
            fprintf(file3, "The product's category is 'Library of Books'\n"); //saves the text to the file and skips the line.
            break; //ends case 5.

        case 6: //if the entered data is equal to 6.
            fprintf(file3, "The product's category is 'Whole Foods'\n"); //saves the text to the file and skips the line.
            break; //ends case 6.
        
        case 7: //if the entered data is equal to 6.
            fprintf(file3, "The product's category is 'Home Accessories'\n"); //saves the text to the file and skips the line.
            break; //ends case 7.
        }

        fprintf(file3, "The product's weight is => %.2f lbs\n\n", catalog->product_weight); //saves the displayed text and catalog product_weight information to file then bring the cursor below that line.
        fprintf(file3, "The product's details is:\n"); //saves the displayed text to file
        fprintf(file3, "%s\n", catalog->product_details); //saves the catalog product_details information to file
        MAX = 0;
    }
    fclose(file3); //closes the file.*/
}

//This function adds data to the top of the stack using parameters
void push(stk*stack, struct Lister pushing_Item){
    Node *newNode = (Node*)malloc(sizeof(Node)); //creates a node called newNode with dynamic memory
    if (!newNode){ //check if newNode was not created to do
        printf("Stack overflow\n");
        return; //exits the function
    }
    newNode->Saves = pushing_Item; //saves data from the Lister called pushing_Item from the parameter to newNode
    newNode->next = stack->top; //newNode->next link to a newly created node
    stack->top = newNode; //top of the stack points to newNode to equal to the data to be saved on the next cycle
}

//A boolean function to check if a Node is empty using a parameter
bool ISempty(stk*stack){
    Node* Check_Everything = stack->top; //A created Node is created equal to the top of the stack
    if (Check_Everything == NULL) return true; //checks if it is empty to equal true
    else return false; //if not, then false
}

//Function to enter information
void GettingData(stk*stack)
{
    struct Lister newLister; // This line declares a variable named newLister

    printf("Enter customer's name\n"); //displays the text to prompt the user to enter data.
    printf("(Type '@' to cancel entry): "); //displays text on screen
    if (scanf(" %[^\n]%*c", newLister.buyer_name) == 0 || strcmp(newLister.buyer_name, "@") == 0) //checks if data in saved is invalid or the data saved is equal to '@'
    {
        printf("Data entry cancelled! Press 'ENTER' key to return to menu..."); //displays text on screen
        getchar(); //saves the Enter Key
        return; //to return to back to the menu
    }

    printf("\n"); //skips the line

    printf("Enter customer's address\n"); //displays the text to prompt the user to enter data.
    printf("(Type '@' to cancel entry): "); //displays text on screen
    if (scanf(" %[^\n]%*c", newLister.address) == 0 || strcmp(newLister.address, "@") == 0) //checks if data in saved is invalid or the data saved is equal to '@'
    {
        printf("Data entry cancelled! Press 'ENTER' key to return to menu..."); //displays text on screen
        getchar(); //saves the Enter Key
        return; //to return to back to the menu
    }

    printf("\n"); //skips the line

    printf("Enter Distributor's name\n"); //displays the text to prompt the user to enter data.
    printf("(Type '@' to cancel entry): "); //displays text on screen
    if (scanf(" %[^\n]%*c", newLister.distributor_name) == 0 || strcmp(newLister.distributor_name, "@") == 0) //checks if data in saved is invalid or the data saved is equal to '@'
    {
        printf("Data entry cancelled! Press 'ENTER' key to return to menu..."); //displays text on screen
        getchar(); //saves the Enter Key
        return; //to return to back to the menu
    }

    printf("\n"); //skips the line

    do { //beginning of a repeat until loop
        char ans[4]; //declaring ans as a string
        printf("Is the distributor the native company or outside company producer? \n"); //displays the text to prompt the user to enter data.
        printf("Type 'in' for 'in-house' or 'out' for 'out-house'\n"); //displays text on screen
        printf("(Type '@' to cancel entry): "); //displays text on screen
        scanf("%s3", ans); //saves data to ans

        for (int k = 0; k < 3; k++) //using for loop to go to each indivitual characters of the ans array
        {
            ans[k] = toupper(ans[k]); //to change each the letter to capital and save it back to the ans
        }

        if (strcmp(ans, "IN") == 0) //check if data is equal to 'IN'
        {
            strcpy(newLister.location, "IN-HOUSE"); //saves the string to newLister.location
            break;
        } 
        else if (strcmp(ans, "OUT") == 0) { //check if data is equal to 'OUT'
            strcpy (newLister.location, "OUT-HOUSE"); //saves the string to newLister.location
            break;
        }
        else if (strcmp(ans, "@") == 0) //check if data is equal to '@'
        {
            printf("Data entry cancelled! Press 'ENTER' key to return to menu..."); //displays text on screen
            // Clear the input buffer to handle excess characters
            while (getchar() != '\n');
            getchar(); //saves the Enter Key
            return; //to return to back to the menu
        }
        else //default resort if conditions above are not met
        {
            printf("Invalid entered data, please enter correct data\n\n"); //displays text on screen
        }
    } while (1); //while (1) is a infinite loop
        
    printf("\n"); //skips the line

    printf("Enter the currency used for purchase (e.g. KWD, BHD, OMR, JOD, GBP, KYD, EUR, CHF, USD or BSD)\n"); //displays the text to prompt the user to enter data.
    printf("(Type '@' to cancel entry): "); //displays text on screen   
    if (scanf(" %3s", newLister.currency) == 0 || strcmp(newLister.currency, "@") == 0) //checks if data in saved is invalid or the data saved is equal to '@'
    {
        printf("Data entry cancelled! Press 'ENTER' key to return to menu..."); //displays text on screen
        // Clear the input buffer to handle excess characters
        while (getchar() != '\n');
        getchar(); //saves the Enter Key
        return; //to return to back to the menu
    }
    for (int k = 0; k < 3; k++) //using for loop to go to each indivitual characters of the ans array
    {
        newLister.currency[k] = toupper(newLister.currency[k]); //to change each the letter to capital and save it back to the ans
    }
        
    printf("\n"); //skips the line
    // Clear the input buffer to handle excess characters
    while (getchar() != '\n');

    printf("Enter the product's name\n"); //displays the text to prompt the user to enter data.
    printf("(Type '@' to cancel entry): "); //displays text on screen
    if (scanf(" %[^\n]%*c", newLister.product_name) == 0 || strcmp(newLister.product_name, "@") == 0) //checks if data in saved is invalid or the data saved is equal to '@'
    {
        printf("Data entry cancelled! Press 'ENTER' key to return to menu..."); //displays text on screen
        getchar(); //saves the Enter Key
        return; //to return to back to the menu
    }
        
    printf("\n"); //skips the line
        
    //repeat until loop to use to repeat message if data entered is invalid.
    do {
        printf("Enter the price of the product (Type '0' to cancel entry): ");
            
        if (scanf("%f", &newLister.price) != 1) { //check if data entered is not true
            printf("Invalid input. Please enter a number.\n\n"); //displays text on screen and skip 2 line
            while (getchar() != '\n'); // Clear the input buffer
        } 
        else if (newLister.price == 0) { //checks if data entered is equal to 0
            printf("Data entry cancelled! Press 'ENTER' key to return to the menu..."); //displays text on screen
            while (getchar() != '\n'); // Clear the input buffer
            getchar(); //saves the Enter Key
            return; // To return to the menu
        }
        else { //a default resort if conditions are not met above
            break; // Ends the infinite while loop
        }
    } while (1); //while (1) is a infinite loop

    while (getchar() != '\n'); // Clear the input buffer
    printf("\n"); //skips the line

    while (1) //while (1) is a infinite created loop to do processes until it is true to break.
    {
        printf("What category is the product from? Type the number corresponding with the category:\n"); //displays the text to prompt the user to enter data and skips line.
        printf("1. Electronics\n"); //displays the text and bring cursor to next line.
        printf("2. Toys & Games\n"); //displays the text and bring cursor to next line.
        printf("3. Health & Wellness Products\n"); //displays the text and bring cursor to next line.
        printf("4. Sports & Outdoor Items\n"); //displays the text and bring cursor to next line.
        printf("5. Library of Books\n"); //displays the text and bring cursor to next line.
        printf("6. Whole Foods\n"); //displays the text and bring cursor to next line.
        printf("7. Home Accessories\n"); //displays the text and bring cursor to next line.
        printf("(Type '0' to cancel entry): ");
        if (scanf("%d", &newLister.product_category) < 1 || (newLister.product_category > 7)) //to save entered data
        {
            printf("The entered data is invalid, please enter a number from 1 - 7\n\n"); //displays the text on ths screen and skips 2 lines.
            while (getchar() != '\n'); // Clear the input buffer
                
        }
        else if (newLister.product_category == 0) //checks if data entered is equal to 0.
        {
            printf("Data entry cancelled! Press 'ENTER' key to return to menu..."); //displays text on screen
            while (getchar() != '\n'); // Clear the input buffer
            getchar(); //saves the Enter Key
            return; //to return to back to the menu            
        }
            
        else //a default resort if conditions above are not met
        {
            break; //ends the infinite while loop
        }
    }
        
    while (getchar() != '\n'); // Clear the input buffer
    printf("\n"); //skips the line

    //repeat until loop to use to repeat message if data entered is invalid.
    do
    {
        printf("Enter the weight of the product. It will be measured in (lbs)\n"); //displays the text then a line is skipped.
        printf("(Type '0' to cancel entry): "); //displays the text. It prompts the user to enter data.
        if (scanf("%f", &newLister.product_weight) != 1) //checks if entered data is invalid
        {
            printf("The entered data is invalid, please enter a number\n\n"); //displays the text on ths screen and skips 2 lines
            while (getchar() != '\n'); // Clear the input buffer
        }
        else if (newLister.product_weight == 0) //checks if entered data is equal to 0
        {
            printf("Data entry cancelled! Press 'ENTER' key to return to menu..."); //displays text on screen
            while (getchar() != '\n'); // Clear the input buffer
            getchar(); //saves the Enter Key
            return; //to return to back to the menu   
        }
        else //a default resort if conditions above are not met
        {
            break; //ends the infinite while loop
        }
    } while (1); //while (1) is a infinite created loop to do processes until it is true to break.

    while (getchar() != '\n'); // Clear the input buffer
    printf("\n"); //skips the line
    
    newLister.product_details[0] = '\0'; //sets it to be empty to  prevent data overflow or data being present in other cycles

    printf("Enter the product's details (up to 10,000 characters can be entered)\n"); //displays the text then a line is skipped.
    printf("(Type '@' to cancel entry):\n "); //displays the text. It prompts the user to enter data.   
    while (1) //creation of a infinite for while loop.
    { //creation of a infinite for while loop.
        char line[1024]; // Declare a string called line with 1000 characters
        char k; // Declare a character k
        int EmptyLine;

        fgets(line, sizeof(line), stdin); // Save data to line using fgets. This prevents overflow of data

        //to check if there a skipped line present
        if (strcmp(line, "\n") == 0) {
            EmptyLine++; //it is incremented by 1
        }else{
            EmptyLine = 0; //value is set be 0
        }

        //to check if there 2 skipped lines present to break the loop
        if (EmptyLine == 2)
        {
            break; // Exit the infinite loop if data in line is equal to a new line
        }

        /*this extracts a character from line then saves it k, the space between (" %) tells the program to skip
        spaces until a character is found. The if statement checks if this process was successful*/
        if (sscanf(line, " %c", &k) == 1) {
            if (k == '@') {
                printf("Data entry cancelled! Press 'ENTER' key to return to menu...");
                while (getchar() != '\n'); // Clear the input buffer
                getchar(); //saves the Enter Key
                return; //To return to the menu
            }
        }
        // Concatenate line and newLister.product_details, data is added to newLister.product_details.
        strcat(newLister.product_details, line);
    }

    newLister.ID = IDcounter++; //save data from IDcounter to newlister.ID
    newLister.time = time(NULL); //save data from the processes done in char* Readabletime to newlister.tme

    ID(); //opens this function to save current ID
    push(stack, newLister); //Opens this function to push the acquired data, newLister, to the stack
    SaveToMainFile(&newLister); //Opens the function to save data to the txt files
    Data_Uploads(stack);// Opens function to upload updated stack to the database

    printf("{..Data has been saved to the main file..}\n"); //displays the texts then skips the line.
    
    if (MAX = 1){ //check if MAX is equal to 1 display text
        printf("[..Data has been saved to the IN-HOUSE file..]"); //displays the texts then skips the line.
    }
    if (MAX = 0){ //check if MAX is equal to 0 display text
        printf("[..Data has been saved to the OUT-HOUSE file..]"); //displays the texts then skips the line.
    }

    printf("\n[..response has been accepted with ID: %d..]", newLister.ID); //prints the text and data in newLister.ID thenn skips line.
    getchar(); //this waits for the user to press 'ENTER KEY'
}

//Function to erase all the data
void EraseFiles()
{
    FILE *file = fopen(filename, "w"); //open txt file, using pointer filename in write mode.
    fclose(file); //closes the file

    FILE *file1 = fopen("Outhouse.txt", "w"); //open txt file, Outhouse in write mode.
    fclose(file1); //closes the file

    FILE *file2 = fopen("Inhouse.txt", "w"); //open txt file, Inhouse in write mode.
    fclose(file2); //closes the file

    if (JK != 0){ //checks if JK is not equal to 0
        FILE *file3 = fopen(LastIDFilename, "w"); //open txt file, using pointer LastIDFilename in write mode.
        IDcounter = 1; //saving value 1 to IDcounter
    
        fclose(file3); //closes the file
    }
}

//Function to erase all the data from the stack using a parameter
void clearStackFILE(stk*stack){
    FILE* file = fopen("stack.dat", "wb"); //Open file called "stack.dat" in writing binary
        Node*temp; //creates a node called temp

        while (stack->top != NULL){ //while stack-> is not empty
            temp = stack->top; //Node called temp is equal to the top of the stack
            stack->top = stack->top->next; //top of the stack is equal to the data it points to in the stack, 
            free(temp); //frees up the storage in temp
        }
    fclose(file); //closing file
}

stk* clear_stack(stk*stack){
    Node*temp; //creates a node called temp

    while (stack->top != NULL){ //while stack-> is not empty
        temp = stack->top; //Node called temp is equal to the top of the stack
        stack->top = stack->top->next; //top of the stack is equal to the data it points to in the stack, 
        free(temp); //frees up the storage in temp
    }
    return stack;
}

//Removing the last entered data saved in the stack
void Removelast(stk*stack){
    struct Lister Removed; //creating a new Lister structure called Removed
    if (IDcounter > 1){ IDcounter--; } //do the following of decreasing IDcounter by 1 if IDcounter is greater 1
    ID(); //calls up on the function
    
    if (!ISempty(stack)){ //checks if stack from parameter is not empty to do the following
        Pop(stack, &Removed); //calls on function to remove data top data from stack

        printf("\nThe data last entered has been removed:\n\n");

        //This prints all the data in the Lister Removed to show the data that was removed from the stack
        printf("ID Number is => %d\n", Removed.ID); 
        printf("Timestamp => %s\n", Readabletime(Removed.time)); 
        printf("The buyer's name => %s\n", Removed.buyer_name); 
        printf("The buyer's address => %s\n", Removed.address); 
        printf("The distributor's name => %s\n", Removed.distributor_name);
        printf("The company is => %s\n", Removed.location);
        printf("The product's name is => %s\n", Removed.product_name); 
        printf("The product's price => $%.2f %s\n", Removed.price, Removed.currency);

        switch (Removed.product_category) //a switch case to check product category selection with a number.
        {
        case 1: //if the entered data is equal to 1.
            printf("The product's category is 'Electronics'\n");
            break; //ends case 1.

        case 2: //if the entered data is equal to 2.
            printf("The product's category is 'Toys and Games'\n"); 
            break; //ends case 2.

        case 3: //if the entered data is equal to 3.
            printf("The product's category is 'Health & Wellness Products'\n"); 
            break; //ends case 3.

        case 4: //if the entered data is equal to 4.
            printf("The product's category is 'Sports & Outdoor Items'\n"); 
            break; //ends case 4.

        case 5: //if the entered data is equal to 5.
            printf("The product's category is 'Library of Books'\n"); 
            break; //ends case 5.

        case 6: //if the entered data is equal to 6.
            printf("The product's category is 'Whole Foods'\n"); 
            break; //ends case 6.
            
        case 7: //if the entered data is equal to 6.
            printf("The product's category is 'Home Accessories'\n"); 
            break; //ends case 6.
        }

        printf("The product's weight is => %.2f lbs\n\n", Removed.product_weight);
        printf("The product's details is:\n"); 
        printf("%s", Removed.product_details);
    }else{ //if it is empty it does the following
        printf("\n{.{Stack is Empty}.}\n");
    }
}

//function to display saved data in files, first is for the pointer, const char* filename
void DisplayFileContents(const char* filename)
{
    char line[1024]; //declaring a line as an array of 1024 characters
    FILE* file = fopen(filename, "r");
    
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("\n{.{File is Empty}.}\n");
        printf("{...Press 'ENTER' key to go back to the menu...}"); //displays the text on the screen
    }else{
        printf("%s", line); //displays the data in line on the screen
        
        //This while loop continues to read lines from file using fgets and save it to line.
        //The size of line above is used as a criteria for the amount of characters in each line
        while (fgets(line, sizeof(line), file)) {
            
            printf("%s", line); //displays the data in line on the screen
        }

        for (int i = 0; i < 210; ++i) //for loop for i is less than 80 it should do processes below
        {
            printf("_"); //displays the symbol on screen
        }

        printf("\n\n{...Press 'ENTER' key to go back to the menu...}"); //displays the text on the screen
    }
    
    fclose(file); //closes the file
    scanf("%c", &ch); //save single character
}

//Function to Create a copy of data from stack to another. Data copied is saved inversely
void Copying_stack(stk*stack, stk** Copie){
    //Makes Copie stack equal to the result of the clear_stack using Copie in parameter
    //Done to ensure  that Destination stack is empty to avoid new data piling on the old
    *Copie = clear_stack(*Copie);
    Node*Temp = stack->top; //Node Temp is equal to Stack->top and all it points to

    while(Temp != NULL){ //do the following while Temp is not Empty
        push(*Copie, Temp->Saves); //pushes the data of Temp->Saves to the Copie Stack 
        Temp = Temp->next; //the current Temp now points to next data that is linked in Temp
    }
}

//function to do search-ups to find bunch of information based on there ID
void search(stk*stack, int srch)
{
    int found = 0; //initialize a integer found equaling 0
    stk* stack11 = initialize(); //initializes a new stack called 'stack11'
    Copying_stack(stack, &stack11); //copies data from stack in the parameter to the newly created stack11

    FILE* file = fopen("search.txt","w"); //opens the file "search.txt" in writing mode

    printf("Enter the ID number correlating with entered data to search it up: "); //displays sentence on screen
        
    if (scanf("%d", &srch) != 1) { //checks if data entered is not true
        while (getchar() != '\n'); // Clear the input buffer
        printf("Entered data is invalid\n"); //displays text on screen and skip line
        printf("{...Press 'ENTER' key to go back to the menu...}"); //prints the text
        return; //go back to the menu
    }

    printf("\n"); //skips a line.
    struct Lister catalog; //creates a new Lister called catalog

    while(!ISempty(stack11)){ //while stack11 is not empty
        Pop(stack11, &catalog); //removes data from the top of the stack11

        if (catalog.ID == srch) //checks if data in catalog.ID is equal to srch
        {
            for (int i = 0; i < 210; ++i) //for loop for i is less than 80 it should do processes below
            {
                fprintf(file,"_"); //save character to file
            }
            fprintf(file, "\n\n"); //skip a line in the file
            //fprintf is used to save data to file.
            fprintf(file, "ID Number is => %d\n", catalog.ID); //saves the displayed text and catalog ID information to file then bring the cursor below that line.
            fprintf(file, "Timestamp => %s\n", Readabletime(catalog.time)); //saves the displayed text and catalog time from the Readabletime pointer information to file then bring the cursor below that line.
            fprintf(file, "The buyer's name => %s\n", catalog.buyer_name); //saves the displayed text and catalog buyer_name information to file then bring the cursor below that line.
            fprintf(file, "The buyer's address => %s\n", catalog.address); //saves the displayed text and catalog address information to file then bring the cursor below that line.
            fprintf(file, "The distributor's name => %s\n", catalog.distributor_name);//saves the displayed text and catalog distributor_name information to file then bring the cursor below that line.
            fprintf(file, "The company is => %s\n", catalog.location);//saves the displayed text and catalog location information to file then bring the cursor below that line.
            fprintf(file, "The product's name is => %s\n", catalog.product_name); //saves the displayed text and catalog product_name information to file then bring the cursor below that line.
            fprintf(file, "The product's price => $%.2f %s\n", catalog.price, catalog.currency); //saves the displayed text and catalog currency information to file then bring the cursor below that line.

            switch (catalog.product_category) //a switch case to check product category selection with a number.
            {
            case 1: //if the entered data is equal to 1.
                fprintf(file, "The product's category is 'Electronics'\n"); //saves the text to the file and skips the line.
                break; //ends case 1.

            case 2: //if the entered data is equal to 2.
                fprintf(file, "The product's category is 'Toys and Games'\n"); //saves the text to the file and skips the line.
                break; //ends case 2.

            case 3: //if the entered data is equal to 3.
                fprintf(file, "The product's category is 'Health & Wellness Products'\n"); //saves the text to the file and skips the line.
                break; //ends case 3.

            case 4: //if the entered data is equal to 4.
                fprintf(file, "The product's category is 'Sports & Outdoor Items'\n"); //saves the text to the file and skips the line.
                break; //ends case 4.

            case 5: //if the entered data is equal to 5.
                fprintf(file, "The product's category is 'Library of Books'\n"); //saves the text to the file and skips the line.
                break; //ends case 5.

            case 6: //if the entered data is equal to 6.
                fprintf(file, "The product's category is 'Whole Foods'\n"); //saves the text to the file and skips the line.
                break; //ends case 6.
                
            case 7: //if the entered data is equal to 6.
                fprintf(file, "The product's category is 'Home Accessories'\n"); //saves the text to the file and skips the line.
                break; //ends case 6.
            }

            fprintf(file, "The product's weight is => %.2f lbs\n\n", catalog.product_weight); //saves the displayed text and catalog product_weight information to file then bring the cursor below that line.
            fprintf(file, "The product's details is:\n"); //saves the displayed text to file
            fprintf(file, "%s", catalog.product_details); //saves the catalog product_details information to file
            
            found = 1; 
            break; //exits the while loop
        }
    }
    fclose(file); //closing file

    if (found == 0) { //check if found is equal to 0
        printf("[{No information found for ID %d}]", srch); //display text on the screen
    }

    DisplayFileContents("search.txt"); //calls on the function to display text on screen
}


//starting point of program execution
int main()
{
    stk*stack = initialize(); //creating a Stack called stack
    char choice; //declares choice as a character
    char ch; //declares ch as a character
    int srch; //declares srch as integer

    Data_Booted(stack); //Calls on the functure to used data that was saved in file to use in current processes

    while (1) //this is an infinite while loop. while 1 is infinite
    {
        for (int i = 0; i < 80; ++i) //for loop for i is less than 80 it should do processes below
        {
            printf("-"); //displays the symbol on screen
        }

        printf("\n"); //skips a line

        printf("|                                  "); //displays the symbol and spaces on screen
        printf("WORKER BEEZ"); //displays text on screen
        printf("                                 |"); //displays the symbol and spaces on screen
        
        printf("\n"); //skips a line

        for (int i = 0; i < 80; ++i) //for loop for i is less than 80 it should do processes below
        {
            printf("-"); //displays the symbol on screen
        }

        printf("\n"); //skips a line

        printf("|                                     "); //displays the symbol and spaces on screen
        printf("MENU"); //displays text on screen
        printf("                                     |"); //displays the symbol and spaces on screen

        printf("\n"); //skips a line

        for (int i = 0; i < 80; ++i) //for loop for i is less than 80 it should do processes below
        {
            printf("-"); //displays the symbol on screen
        }
        
        printf("\n"); //skips a line

        printf("| (A)......Add New Information                                                 |\n"); //displays the string of characters on the screen then skips a line
        printf("| (B)......Display ALL Records                                                 |\n"); //displays the string of characters on the screen then skips a line
        printf("| (C)......Display Only In-house Records                                       |\n"); //displays the string of characters on the screen then skips a line
        printf("| (D)......Display Only Out-house Records                                      |\n"); //displays the string of characters on the screen then skips a line
        printf("| (E)......Erasing Data                                                        |\n"); //displays the string of characters on the screen then skips a line
        printf("| (F)......Entry Look-Up                                                       |\n"); //displays the string of characters on the screen then skips a line
        printf("| (G)......Terminate Processes                                                 |\n"); //displays the string of characters on the screen then skips a line
        
        for (int i = 0; i < 80; ++i) //for loop for i is less than 80 it should do processes below
        {
            printf("-"); //displays the symbol on screen
        }
        printf("\n"); //skips a line

        printf("(There is no entry limit!)\n\n"); //displays the string of characters on the screen then skips 1 lines

        printf("Enter a letter shown above (A, B, C, D, E, F or G) => "); //displays the string of characters on the screen
        scanf(" %c", &choice);
        printf("\n\n");
        while(getchar() != '\n'); // Clear the input buffer to handle excess characters
        choice = toupper(choice); //the toupper changes the common letters in "choice" to capital and saves them back to "choice"


        switch (choice) //evaluates the value of choice
        {
        case 'A': //if the value in choice is equal to A it does the processes below
            system("cls"); //clears the screen
            for (int i = 0; i < 210; ++i) //for loop for i is less than 210 it should do processes below
            {
                printf("-"); //displays the symbol on screen
            }

            printf("\n"); //skips a line

            printf("|                                                                                                  "); //displays the symbol and spaces on screen
            printf("ENTRY OF DATA"); //displays text on screen
            printf("                                                                                                 |"); //displays the symbol and spaces on screen
            
            printf("\n"); //skips a line

            for (int i = 0; i < 210; ++i) //for loop for i is less than 210 it should do processes below
            {
                printf("-"); //displays the symbol on screen
            }

            printf("\n"); //skips a line
            
            GettingData(stack); //calls up on the void function
            break; //ends the case

        case 'B': //if the value in choice is equal to B it does the processes below
            system("cls"); //clears the screen
            for (int i = 0; i < 210; ++i) //for loop for i is less than 210 it should do processes below
            {
                printf("-"); //displays the symbol on screen
            }

            printf("\n"); //skips a line

            printf("|                                                                                          "); //displays the symbol and spaces on screen
            printf("DATA OF ALL COLLECTED RECORDS"); //displays text on screen
            printf("                                                                                         |"); //displays the symbol and spaces on screen
            
            printf("\n"); //skips a line

            for (int i = 0; i < 210; ++i) //for loop for i is less than 210 it should do processes below
            {
                printf("-"); //displays the symbol on screen
            }

            printf("\n"); //skips a line

            DisplayFileContents(filename);//calls up on the void function
            break; //ends the case

        case 'C': //if the value in choice is equal to C it does the processes below
            system("cls"); //clears the screen
            for (int i = 0; i < 210; ++i) //for loop for i is less than 210 it should do processes below
            {
                printf("-"); //displays the symbol on screen
            }

            printf("\n"); //skips a line

            printf("|                                                                                  "); //displays the symbol and spaces on screen
            printf("ALL COLLECTED RECORDS FOR THE INHOUSE COMPANY"); //displays text on screen
            printf("                                                                                 |"); //displays the symbol and spaces on screen
            
            printf("\n"); //skips a line

            for (int i = 0; i < 210; ++i) //for loop for i is less than 210 it should do processes below
            {
                printf("-"); //displays the symbol on screen
            }

            printf("\n"); //skips a line
            DisplayFileContents("Inhouse.txt");//calls up on the void function
            break; //ends the case

        case 'D': //if the value in choice is equal to D it does the processes below
            system("cls"); //clears the screen
            for (int i = 0; i < 210; ++i) //for loop for i is less than 210 it should do processes below
            {
                printf("-"); //displays the symbol on screen
            }

            printf("\n"); //skips a line

            printf("|                                                                                  "); //displays the symbol and spaces on screen
            printf("ALL COLLECTED RECORDS FOR OUTHOUSE COMPANIES"); //displays text on screen
            printf("                                                                                  |"); //displays the symbol and spaces on screen
            
            printf("\n"); //skips a line

            for (int i = 0; i < 210; ++i) //for loop for i is less than 210 it should do processes below
            {
                printf("-"); //displays the symbol on screen
            }

            printf("\n"); //skips a line
            
            DisplayFileContents("Outhouse.txt");//calls up on the void function
            break; //ends the case

        case 'E': //if the value in choice is equal to E it does the processes below
            system("cls"); //clears the screen

            int r; //Declares r as integer
            int exitLoop = 0; //initializes an integer, exitLoop as 0
            do{
                for (int i = 0; i < 210; ++i) //for loop for i is less than 210 it should do processes below
                {
                    printf("-"); //displays the symbol on screen
                }

                printf("\n|                                                                                                  "); //displays the symbol and spaces on screen
                printf("ERASING DATA"); //displays text on screen
                printf("                                                                                                  |\n"); //displays the symbol and spaces on screen


                for (int i = 0; i < 210; ++i) //for loop for i is less than 210 it should do processes below
                {
                    printf("-"); //displays the symbol on screen
                }

                printf("\n1..Delete Last Entered Data");
                for (int i = 0; i < 22; ++i) //for loop for i is less than 22 it should do processes below
                {
                    printf(" "); //displays the symbol on screen
                }
                printf("|\n");

                for (int i = 0; i < 50; ++i) //for loop for i is less than 50 it should do processes below
                {
                    printf("-"); //displays the symbol on screen
                }

                printf("\n2..Delete All Data");
                for (int i = 0; i < 31; ++i) //for loop for i is less than 31 it should do processes below
                {
                    printf(" "); //displays the symbol on screen
                }
                printf("|\n");

                for (int i = 0; i < 50; ++i) //for loop for i is less than 50 it should do processes below
                {
                    printf("-"); //displays the symbol on screen
                }
                
                printf("\n3..Return to Menu");
                for (int i = 0; i < 32; ++i) //for loop for i is less than 32 it should do processes below
                {
                    printf(" "); //displays the symbol on screen
                }
                printf("|\n");

                for (int i = 0; i < 50; ++i) //for loop for i is less than 50 it should do processes below
                {
                    printf("-"); //displays the symbol on screen
                }

                printf("\n\nEnter the number to do the command: ");
                scanf("%d", &r);

                char choice2 [4]; //a character called choice2 is declared having [4] array spaces
                switch (r) //Do a switch case using r
                {
                    case 1: // if r is equal to 1 do
                        printf("\n\n");
                        for (int i = 0; i < 70; ++i) //for loop for i is less than 70 it should do processes below
                        {
                            printf("-"); //displays the symbol on screen
                        }

                        printf("\n"); //skips a line

                        printf("|                         "); //displays the symbol and spaces on screen
                        printf("ERASING LAST DATA"); //displays text on screen
                        printf("                          |"); //displays the symbol and spaces on screen
                        
                        printf("\n"); //skips a line

                        for (int i = 0; i < 70; ++i) //for loop for i is less than 70 it should do processes below
                        {
                            printf("-"); //displays the symbol on screen
                        }

                        printf("\n"); //skips a line

                        while (1){
                            printf("\nAre you sure you want to delete the last entered data?\n"); //diplays the text on screen
                            printf("Type YES or NO: "); //diplays the text on screen
                            scanf(" %3s", choice2); //save the data in choice2
                            
                            printf("\n");

                            //while(getchar() != '\n'); // Clear the input buffer to handle excess characters
                            
                            //for loop for while i is less than 3 processes are done.
                            //It increments by 1 each looping. It is used to go through each letter in choice2
                            for (int i = 0; i < 3; i++)
                            {
                                //change the letter in choice2 to capital then saves it back to choice2
                                choice2[i] = toupper(choice2[i]);
                            }
                        
                            if (strcmp(choice2, "YES") == 0) //checks if the string in choice2 is equal to "YES"
                            {
                                EraseFiles(); //calls on this function to erase all the data in the files
                                Removelast(stack); //removes the last entered data in the stack
                                stk*Copy = initialize(); //creates a stack called Copy

                                Copying_stack(stack,&Copy); //calls on the function to Copy contents from stack to Copy
                                Node*Node = Copy->top; //create a node, Node, to equal to Copy->top and the contents it links to
                                while (Node != NULL){ //while Node is not Empty do 
                                    SaveToMainFile(&Node->Saves); //Calls on the function to Save data from Node->Saves to the the text files in text
                                    Node = Node->next; //Node is equal to the link that links to the next data of Node
                                }
                                
                                 /*Saves the contents from Copy to stack. this is done to revert the order of data saved in stack
                                 sooo stack can have data in correct order*/
                                Copying_stack(Copy,&stack);
                                Data_Uploads(stack); //Calls on function to upload updated content to the database
                                free(Copy); //the frees up the memory of Copy to save data
                                break; // exits infinite loop
                            }
                            else if (strcmp(choice2, "NO") == 0) //checks if the string in choice2 is equal to "NO"
                            {
                                printf("Erasing data has been cancelled\n"); //displays the text on screen
                                break; // exits infinite loop
                            }
                            else //a default resort if conditions above are not met
                            {
                                printf("Invalid input\n");  //displays the text on screen
                                printf("Enter 'YES' or 'NO' to proceed...\n\n"); //displays the text on screen
                            }
                        }
                        exitLoop = 1;
                        break;
                    
                    case 2:
                        JK = 1;
                        printf("\n\n");
                        for (int i = 0; i < 70; ++i) //for loop for i is less than 80 it should do processes below
                        {
                            printf("-"); //displays the symbol on screen
                        }

                        printf("\n"); //skips a line

                        printf("|                          "); //displays the symbol and spaces on screen
                        printf("ERASING ALL DATA"); //displays text on screen
                        printf("                          |"); //displays the symbol and spaces on screen
                        
                        printf("\n"); //skips a line

                        for (int i = 0; i < 70; ++i) //for loop for i is less than 80 it should do processes below
                        {
                            printf("-"); //displays the symbol on screen
                        }

                        printf("\n"); //skips a line

                        char choice2[4]; //declares choice2 as a string

                        while (1){
                            printf("Are you sure you want to erase all data?\n"); //diplays the text on screen
                            printf("Type YES or NO: "); //diplays the text on screen
                            scanf(" %3s", choice2); //save the data in choice2
                            
                            printf("\n");

                            //while(getchar() != '\n'); // Clear the input buffer to handle excess characters
                            
                            //for loop for while i is less than 3 processes are done.
                            //It increments by 1 each looping. It is used to go through each letter in choice2
                            for (int i = 0; i < 3; i++)
                            {
                                //change the letter in choice2 to capital then saves it back to choice2
                                choice2[i] = toupper(choice2[i]);
                            }
                        
                            if (strcmp(choice2, "YES") == 0) //checks if the string in choice2 is equal to "YES"
                            {
                                EraseFiles(); //calls on this function
                                clearStackFILE(stack);
                                break;
                            }
                            else if (strcmp(choice2, "NO") == 0) //checks if the string in choice2 is equal to "NO"
                            {
                                printf("Erasing data has been cancelled\n"); //displays the text on screen
                                break;
                            }
                            else //a default resort if conditions above are not met
                            {
                                printf("Invalid input\n");  //displays the text on screen
                                printf("Enter 'YES' or 'NO' to proceed...\n\n"); //displays the text on screen
                            }
                        }
                        JK = 0;
                        exitLoop = 1;
                        break; //end the case

                    case 3: //do the following if r equals 3
                        exitLoop = 1; 
                        break; //ends the case

                    default: //default case if all the above aren't possible
                        printf("Invalid input. Please enter 1, 2 or 3");
                        while(getchar() != '\n'); //clears input buffer
                        getchar(); //accepts the enter key
                        system("cls"); //clears screen
                        break; //exits default case
                }
            } while (!exitLoop); //do this repeat loop while exitLoop isn't equal to one
                
            printf("\n{...Press 'ENTER' key to go back to the menu...}"); //displays the text on screen
            while(getchar() != '\n');
            getchar(); //to capture enter key
            break; //ends the case

        case 'F': //if the value in choice is equal to F it does the processes below
            system("cls"); //clears the screen
            for (int i = 0; i < 210; ++i) //for loop for i is less than 210 it should do processes below
            {
                printf("-"); //displays the symbol on screen
            }

            printf("\n"); //skips a line

            printf("|                                                                                                 "); //displays the symbol and spaces on screen
            printf("ENTRY LOOK-UP"); //displays text on screen
            printf("                                                                                                  |"); //displays the symbol and spaces on screen
            
            printf("\n"); //skips a line

            for (int i = 0; i < 210; ++i) //for loop for i is less than 210 it should do processes below
            {
                printf("-"); //displays the symbol on screen
            }

            printf("\n"); //skips a line
            search(stack, srch);
            scanf("%c", &ch); //save single character
            break; //ends the case

        case 'G': //if the value in choice is equal to G it does the processes below
            system("cls"); //clears the screen
            do {    
                for (int i = 0; i < 210; ++i) //for loop for i is less than 210 it should do processes below
                {
                    printf("-"); //displays the symbol on screen
                }

                printf("\n"); //skips a line

                printf("|                                                                                               "); //displays the symbol and spaces on screen
                printf("TERMINATING PROGRAM"); //displays text on screen
                printf("                                                                                              |"); //displays the symbol and spaces on screen
                
                printf("\n"); //skips a line

                for (int i = 0; i < 210; ++i) //for loop for i is less than 210 it should do processes below
                {
                    printf("-"); //displays the symbol on screen
                }

                printf("\n"); //skips a line
            
            
                printf("The program will END. Are you sure you want to terminate the program?\n\n");  //displays the text on screen
                printf("Type 'Y' to terminate or 'N' to return to the menu...");  //displays the text on screen
                scanf(" %c", &ch); //save single character
                while (getchar() != '\n'); //clears input buffer
                printf("\n"); //skips a line

                ch = toupper(ch); //changes character to a capital letter

                if (ch == 'Y'){ //checks if character is equal to capital Y
                    return 0; //ends the entire program
                } else if (ch == 'N'){ //checks if character is equal to capital N
                    break; //exits the infinite loop
                }else{ //last resort condition
                    printf("Invalid input\n");  //displays the text on screen
                    printf("{...Press 'ENTER' key to enter correct input...}"); //displays the text on screen
                    getchar(); //to capture enter key
                    system("cls"); //clears screen
                }
            } while (1); //infinite repeat until loop
            break; //exits the case 

        default: //default case if all the above aren't possible
            printf("You entered an invalid letter. Enter A, B, C, D, E, F or G above\n");  //displays the text on screen
            printf("{...Press 'ENTER' key to enter correct input...}");  //displays the text on screen
            scanf("%c", &ch); //save single character
            break; //ends the case
        }
        system("cls"); //used to clear the screen
    } //Bracket the closes main case
} //Bracket that closes main infinite while loop

/*
The program exe requires 432KB of storage,
A Lenovo Device was used to run the exe so there may be crashes on other devices since it was never tested,
Atleast 1.2GHz processors is needed to run the exe,
It needs a minimum of 1G of ram to run exe file.
*/ 