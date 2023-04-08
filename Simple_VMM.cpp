#include <iostream>
#include <cstdint>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;


#define PRINT_VARIABLE_NAME(x) cout << #x << " = " << x << endl;

// Define MIPS opcodes
#define ADD "add"
#define ADDI "addi"
#define LW "lw"
#define LI "li"
#define SUB "sub"
#define MUL "mul"
#define AND "and"
#define OR "or"
#define ORI "ori"
#define XOR "xor"
#define SLL "sll"
#define SRL "srl"
#define DUMP_PROCESSOR_STATE "DUMP_PROCESSOR_STATE"



vector <string> read_assembly_file(string file_name){
        // Initialize an empty vector of strings to hold the extracted values
        vector<string> assembly_file;

        // Initialize two strings to hold the extracted values
        string vm_exec_slice_in_instructions;
        string vm_binary;

        // Open the input file
        ifstream file(file_name);

        // Check if the file was successfully opened
        if (!file.is_open()) {
            cerr << "Failed to open file" << std::endl;
        }

        // Read the file line by line
        string line;
        while (getline(file, line)) {
            // Find the position of the equal sign in each line
            size_t pos = line.find("=");

            // If the line contains the VM execution slice in instructions value, extract it and add it to the vector
            if (line.substr(0,pos) == "vm_exec_slice_in_instructions"){
                assembly_file.push_back(line.substr(pos+1));
            }
            // If the line contains the VM binary value, extract it and add it to the vector
            else if (line.substr(0,pos) == "vm_binary"){
                assembly_file.push_back(line.substr(pos+1));
            }
        }

        // Close the input file
        file.close();

        // Return the vector of extracted values
        return assembly_file; 

}

// This function reads the contents of a file with the given name
// and returns the file contents as a vector of strings.
vector<string> read_instruction_file(string file_name) {
    // Declare a vector of strings to hold the file contents.
    vector<string> instruction_file;
    
    // Open the file with the given name.
    ifstream file(file_name);
    
    // Check if the file was successfully opened.
    if (!file.is_open()) {
        // If the file failed to open, output an error message to standard error.
        cerr << "Failed to open file" << endl;
    }

    // Read each line of the file and add it to the vector of strings.
    string line;
    while (getline(file, line)) {
        instruction_file.push_back(line);
    }
    
    // Close the file.
    file.close();
    
    // Return the vector of strings containing the file contents.
    return instruction_file; 
}

// This function reads a string containing assembly code and returns a vector
// of strings, where each string is an opcode and its associated register(s).
vector<string> read_instruction(string assembly_code) {
    // Declare a vector of strings to hold the opcodes and registers.
    vector<string> opcode_registers;
    
    // Use an istringstream to read the assembly code string line by line.
    istringstream iss(assembly_code);
    string line;
    while(getline(iss, line)) {
        // Use another istringstream to parse each line into an instruction and its register(s).
        istringstream iss_line(line);
        string instruction, registers;
        iss_line >> instruction >> registers;
        
        // Add the instruction and register(s) to the vector of opcodes and registers.
        opcode_registers.push_back(instruction);
        opcode_registers.push_back(registers);
    }
    
    // Return the vector of opcodes and registers.
    return opcode_registers;
}

// This function takes a string of comma-separated registers and returns a vector
// of strings, where each string is a single register.
vector<string> extract_registers(string registers_string) {
    // Declare a vector of strings to hold the registers.
    vector<string> registers;
    
    // Use a stringstream to read the registers string.
    stringstream ss(registers_string);
    while(ss.good()) {
        // Read each comma-separated substring from the stringstream and add it to the vector of registers.
        string substring;
        getline(ss, substring, ',');
        registers.push_back(substring);
    }

    // Return the vector of registers.
    return registers;
}





// This is the definition of the VM class
class VM{
    public:
        // These variables represent the properties of the virtual machine
        string vm_binary_path;                       // The path to the binary file that contains the virtual machine's instructions
        int vm_exec_slice_in_instructions;           // The number of instructions that should be executed in a single slice
        int pc;                                      // The program counter that points to the current instruction
        int registers[32];                           // The virtual machine's registers
        int memory[1024];                            // The virtual machine's memory
        vector<string> instructions;                 // The virtual machine's instructions
        
        // This is the constructor for the VM class
        VM(int exec_slice_in_instructions, string binary_file){
            vm_binary_path = binary_file;
            vm_exec_slice_in_instructions = exec_slice_in_instructions;
            pc = 0;
            memset(registers, 0, sizeof(registers));
            memset(memory, 0, sizeof(memory));
            instructions = read_instruction_file(vm_binary_path);  // Read the instructions from the binary file and store them in the instructions vector
        }
        
        // This function prints out the properties of the virtual machine
        void printVM(){
            PRINT_VARIABLE_NAME(vm_binary_path);
            PRINT_VARIABLE_NAME(vm_exec_slice_in_instructions);
            PRINT_VARIABLE_NAME(pc);
        }
};


class VMM{
    public:
        vector<VM> Virtual_Machines;
       
        // This function prints all the virtual machines available in the system
        vector<VM> get_all_vms(){
            cout<<"-----All Virtual Machines-----"<<endl;
            for(int i=0;i<Virtual_Machines.size();i++){
                Virtual_Machines[i].printVM();
            }
            return Virtual_Machines;
        }

        // This function adds a virtual machine to the system
        void add_vm(VM vm){
            Virtual_Machines.push_back(vm);
        }

        // This function prints all the instructions of a given virtual machine
        vector<string> get_all_vm_instructions(VM vm){    
            for(int i=0;i<vm.instructions.size();i++){
                cout<<vm.instructions[i]<<endl;
            }
            return vm.instructions;
        }

        void execute(VM& vm,string instruction){
            string opcode = read_instruction(instruction)[0];
            string registers_string = read_instruction(instruction)[1];

            if( opcode == ADD ){
                //cout<<"Operation: "<<opcode<<endl;
                vector<string> registers_operands = extract_registers(registers_string);
                vm.registers[stoi(registers_operands[0].substr(1))] = vm.registers[stoi(registers_operands[1].substr(1))]  + vm.registers[stoi(registers_operands[2].substr(1))];          
            }else if( opcode == ADDI ){
                //cout<<"Operation: "<<opcode<<endl;
                vector<string> registers_operands = extract_registers(registers_string);
                vm.registers[stoi(registers_operands[0].substr(1))] = vm.registers[stoi(registers_operands[1].substr(1))]  + stoi(registers_operands[2]);                 
            }else if( opcode == LI ){
                //cout<<"Operation: "<<opcode<<endl;
                vector<string> registers_operands = extract_registers(registers_string);
                vm.registers[stoi(registers_operands[0].substr(1))] = stoi(registers_operands[1]);
            }else if( opcode == SUB ){
                //cout<<"Operation: "<<opcode<<endl;
                vector<string> registers_operands = extract_registers(registers_string);
                vm.registers[stoi(registers_operands[0].substr(1))] = vm.registers[stoi(registers_operands[1].substr(1))]  - vm.registers[stoi(registers_operands[2].substr(1))];                  
            }else if( opcode == MUL ){
                //cout<<"Operation: "<<opcode<<endl;
                vector<string> registers_operands = extract_registers(registers_string);
                vm.registers[stoi(registers_operands[0].substr(1))] = vm.registers[stoi(registers_operands[1].substr(1))]  * vm.registers[stoi(registers_operands[2].substr(1))];  
            }else if( opcode == AND ){
                //cout<<"Operation: "<<opcode<<endl;
                vector<string> registers_operands = extract_registers(registers_string);
                vm.registers[stoi(registers_operands[0].substr(1))] = vm.registers[stoi(registers_operands[1].substr(1))] & vm.registers[stoi(registers_operands[2].substr(1))];  
            }else if( opcode == OR ){
                //cout<<"Operation: "<<opcode<<endl;
                vector<string> registers_operands = extract_registers(registers_string);
                vm.registers[stoi(registers_operands[0].substr(1))] = vm.registers[stoi(registers_operands[1].substr(1))]  | vm.registers[stoi(registers_operands[2].substr(1))];  
            }else if( opcode == XOR ){
                //cout<<"Operation: "<<opcode<<endl;
                vector<string> registers_operands = extract_registers(registers_string);
                vm.registers[stoi(registers_operands[0].substr(1))] = vm.registers[stoi(registers_operands[1].substr(1))] ^ vm.registers[stoi(registers_operands[2].substr(1))];  
            }else if( opcode == ORI ){
                vector<string> registers_operands = extract_registers(registers_string);
                vm.registers[stoi(registers_operands[0].substr(1))] = vm.registers[stoi(registers_operands[1].substr(1))] | stoi(registers_operands[2]); 
            }else if( opcode == SLL ){
                //cout<<"Operation: "<<opcode<<endl;
                vector<string> registers_operands = extract_registers(registers_string);
                vm.registers[stoi(registers_operands[0].substr(1))] = vm.registers[stoi(registers_operands[1].substr(1))] << stoi(registers_operands[2]);                
            }else if( opcode == SRL ){
                //cout<<"Operation: "<<opcode<<endl;
                vector<string> registers_operands = extract_registers(registers_string);
                vm.registers[stoi(registers_operands[0].substr(1))] = vm.registers[stoi(registers_operands[1].substr(1))] >> stoi(registers_operands[2]);  
            }else if( opcode == DUMP_PROCESSOR_STATE ){
                for(int i=0;i<45;i++){cout<<"*";}cout<<endl;cout<<"PROCESSOR_STATE: "<<vm.vm_binary_path<<endl;for(int i=0;i<45;i++){cout<<"*";}cout<<endl;
                for(int i=0;i<32;i++){
                    cout<<"\t" << "$R" << i << "-->"<<vm.registers[i]<< endl;
                }
                for(int i=0;i<45;i++){cout<<"*";}cout<<endl;
            } 
        }

        void run_instruction_file(VM& vm){
            vector<string> instruction_file = vm.instructions;
            for(int idx=vm.pc; idx <vm.pc+2; idx++){
                execute(vm,instruction_file[idx]);
            }
        }
        void run_with_context_switch(){
            int total_instructions = 0;
            vector<int> vm_instruction_sizes;
            vector<int> vm_exec_in_instructions_sizes;
            for(int idx = 0; idx < Virtual_Machines.size(); idx++){
                total_instructions += Virtual_Machines[idx].instructions.size();
                vm_instruction_sizes.push_back(Virtual_Machines[idx].instructions.size());
                vm_exec_in_instructions_sizes.push_back(Virtual_Machines[idx].vm_exec_slice_in_instructions);
            }
            PRINT_VARIABLE_NAME(total_instructions);
            int vm_pointer = 0;
            int vm_count = Virtual_Machines.size();
            while(total_instructions > 0){
                // PRINT_VARIABLE_NAME(Virtual_Machines[vm_pointer%vm_count].instructions.size()-Virtual_Machines[vm_pointer%vm_count].pc);
                Virtual_Machines[vm_pointer%vm_count].printVM();
                total_instructions -=Virtual_Machines[vm_pointer%vm_count].vm_exec_slice_in_instructions;
                run_instruction_file(Virtual_Machines[vm_pointer%vm_count]);
                Virtual_Machines[vm_pointer%vm_count].pc += 2;
                vm_pointer++;
            }
        }

};



// This is the main function of the program
int main(int argc, char**argv) {

    // This vector will store the names of the assembly files passed in as arguments
    vector<string> assembly_files;

    // Loop through the arguments passed in and look for the -v flag, which indicates an assembly file to load
    for(int idx = 1; idx < argc; idx++){
        if ( string(argv[idx]) == "-v" ){
            assembly_files.push_back(argv[idx+1]);
        }
    }

    // Create a new instance of the VMM class
    VMM myvmm = VMM();

    // Loop through the assembly files and create a new virtual machine for each one
    for(int idx = 0; idx < assembly_files.size(); idx++){
        // Read the assembly file and extract the instruction slice size and binary file path
        vector<string> vm_assembly_file = read_assembly_file("input_files/"+assembly_files[idx]);
        // Create a new VM instance and add it to the VMM
        myvmm.add_vm(VM(stoi(vm_assembly_file[0]),"input_files/"+vm_assembly_file[1]));
    }

    // Run the virtual machines with context switching
    myvmm.run_with_context_switch();

    // Return 0 to indicate successful completion of the program
    return 0;
}
